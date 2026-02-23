#include "physics enemy.cpp"
#include "blocky block.cpp"
#include "repair block.cpp"
#pragma once

class Blocky : public PhysicsEnemy {
	using PhysicsEnemy::PhysicsEnemy;
	int moveSpeed = 50;

	int angle = 180;

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	enum State {
		moving, stunned, restoring
	};
	State state = moving;

	float stunTime = 1.5;
	float stunTime_left = stunTime;

	bool dam = false;

	bool blink = false;
	float blinkTime = 0.2;
	float blinkTime_left = blinkTime;
	float openTime = 0.6;
	float openTime_left = openTime;

	list<IntRect> openLoop = { IntRect(516, 11, 16, 64), IntRect(533, 11, 20, 64), IntRect(554, 11, 20, 64) };
	list<IntRect> closeLoop = { IntRect(575, 11, 16, 64), IntRect(592, 11, 20, 64), IntRect(613, 11, 20, 64) };

	float yTarget = 0;

public:

	void flashmanPallete() {
		phys->setRect(IntRect(398, 11, 16, 64));
		openLoop = { IntRect(398, 11, 16, 64), IntRect(415, 11, 20, 64), IntRect(436, 11, 20, 64) };
		closeLoop = { IntRect(457, 11, 16, 64), IntRect(474, 11, 20, 64), IntRect(495, 11, 20, 64) };

	}

	virtual ~Blocky() {

	}

	void initial() {
		phys->setPosition(initialPos);
		phys->setRect(IntRect(516, 11, 16, 64));

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 64), phys));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 16 * 4, 16, 16), phys));
		hp = 2;
		state = moving;

		anim = shared_ptr<animation>(new animation(openLoop, phys));
		timer = shared_ptr<animTimer> (new animTimer(anim, 8, true));
		setCode("blocky");
		phys->enableGravity(true);
		offSetList();
	}

	void onDamage() {
		dam = true;
	}


	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
		if (state == moving) {
			blinkLoop(deltaT);
			timer->run(deltaT);
			phys->move(angle, deltaT, moveSpeed);
			phys->eachFrame(deltaT);
			tileCollision(tileList);

			if (grounded) {
				phys->setVVelocity(0);
			}

			
			if (dam) {
				yTarget = phys->getPosition().y;
				state = stunned;
				shoot(bList);

				phys->setRect(IntRect(533, 76, 16, 16));
				hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), phys));
				hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(-9999, -9999, 0, 0), phys));
				dam = false;
			}
		}

		else if (state == stunned) {
			phys->eachFrame(deltaT);
			tileCollision(tileList);

			stunTime_left -= *deltaT;
			if (stunTime_left <= 0) {
				stunTime_left = stunTime;

				replaceStart(bList);
				state = restoring;

			}
		}
		else {
			if (replaceLoop(deltaT, bList)) {
				state = moving;
				hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 64), phys));
				hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 16 * 4, 16, 16), phys));
				phys->setPosition(Vector2f(phys->getPosition().x, yTarget));
			}
		}
	}

	void blinkLoop(float* deltaT) {
		if (!blink) {
			openTime_left -= *deltaT;
			if (openTime_left <= 0) {
				openTime_left = openTime;
				anim->setAnim(closeLoop);
				anim->thisFrame();
				blink = true;
			}
		}
		else {
			blinkTime_left -= *deltaT;
			if (blinkTime_left <= 0) {
				blinkTime_left = blinkTime;
				anim->setAnim(openLoop);
				anim->thisFrame();
				blink = false;
			}
		}
	}

	void shoot(list<shared_ptr<EnemyBullet>>* bList) {


		shared_ptr<BlockyBlock> block1 = shared_ptr<BlockyBlock>(new BlockyBlock(sprite->getTexture(), Vector2f(phys->getPosition().x, yTarget), 900, faceRight));
		shared_ptr<BlockyBlock> block2 = shared_ptr<BlockyBlock>(new BlockyBlock(sprite->getTexture(), Vector2f(phys->getPosition().x, yTarget + (32*4)), 600, faceRight));
		shared_ptr<BlockyBlock> block3 = shared_ptr<BlockyBlock>(new BlockyBlock(sprite->getTexture(), Vector2f(phys->getPosition().x, yTarget + (48 * 4)), 400, faceRight));
		bList->push_back(block1);
		bList->push_back(block2);
		bList->push_back(block3);
	}

	void replaceStart(list<shared_ptr<EnemyBullet>>* bList) {
		int startPos = 40 * 4;
		shared_ptr<RepairBlock> rep = shared_ptr<RepairBlock> (new RepairBlock(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y + (startPos)), yTarget));
		shared_ptr<RepairBlock> rep1 = shared_ptr<RepairBlock> (new RepairBlock(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y + ((startPos) + 32*4)), yTarget + (32*4)));
		shared_ptr<RepairBlock> rep2 = shared_ptr<RepairBlock> (new RepairBlock(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y + ((startPos)) + 48 * 4), yTarget + (48*4)));
		bList->push_back(rep);
		bList->push_back(rep1);
		bList->push_back(rep2);
	}

	bool replaceLoop(float* deltaT, list<shared_ptr<EnemyBullet>>* bList) {
		bool done = true;
		for (shared_ptr<EnemyBullet> bul : *bList) {
			if (bul->getCode() == "repair block") {
				done = false;
			}
		}
		return done;
	}


};