#include "temp physics enemy.cpp"
#include "sniper bullet.cpp"
#include <SFML/Audio.hpp>
#pragma once

class SniperJoe : public TempPhysicsEnemy {
	using TempPhysicsEnemy::TempPhysicsEnemy;

	shared_ptr<animation> shootSprite;
	shared_ptr<animation> blockSprite;


	shared_ptr<Sound> shootSound;

	bool faceRight = false;

	float untilShoot = 3;
	float untilShoot_left = untilShoot;

	float shootDelay = 0.5;
	float shootDelay_left = shootDelay;
	int shootNum = 3;
	int shootLeft = 3;


	enum State {
		blocking, shooting
	};
	State state = blocking;

	bool rawSpawn;

public:



	void initial() {
		ini();
		rawSpawn = true;
	}

	void ini() {
		shootSprite = shared_ptr<animation>(new animation(IntRect(648, 443, 22, 24), sprite));
		shootSprite->thisFrame();
		blockSprite = shared_ptr<animation>(new animation(IntRect(623, 443, 24, 24), sprite));
		phys->setScale(Vector2f(4, 4));
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 24, 24), sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(4, 4, 0, 0), sprite));
		state = blocking;
		phys->enableGravity(true);
		phys->setPosition(initialPos);

		offSetList();


		hp = 8;
		damage = 3;
		setCode("sniper joe");
		grounded = false;

	}

	void setSound(shared_ptr<SoundCollection> soundCol) {
		shootSound = soundCol->getShoot();
	}

	void initial(bool b) {
		ini();
		rawSpawn = b;

	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		tileCollision(tileList);
		checkDirection(p->getSprite());

		if (state == blocking) {
			if (!grounded) {
				phys->eachFrame(deltaT);
			}
			hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(4, 4, 0, 0), sprite));
			blockSprite->thisFrame();
			untilShoot_left -= *deltaT;
			if (untilShoot_left <= 0) {
				untilShoot_left = untilShoot;
				state = shooting;
			}
		}

		else {
			hurt = hit;
			shootSprite->thisFrame();
			shootDelay_left -= *deltaT;
			if (shootDelay_left <= 0) {
				shootDelay_left = shootDelay;

				shoot(bList);

				shootLeft -= 1;
				if (shootLeft <= 0) {
					shootLeft = shootNum;
					state = blocking;
				}
			}
		}

	}

	void shoot(list<shared_ptr<EnemyBullet>>* bList) {
		shootSound->play();
		Vector2f sPos;
		int angle;
		if (faceRight) {
			sPos = Vector2f(sprite->getPosition().x + sprite->getSize().x, sprite->getMiddlePos().y);
			angle = 0;
		}
		else {
			sPos = Vector2f(sprite->getPosition().x, sprite->getMiddlePos().y);
			angle = 180;
		}

		shared_ptr<SniperBullet> newB = shared_ptr<SniperBullet> ( new SniperBullet(sprite->getTexture(), sPos, angle));
		bList->push_back(newB);
	}

	bool isDead(list<shared_ptr<enemy>>* tempEList) {
		if (!rawSpawn) {
			return true;
		}
		else {
			act = false;
			display = false;
			sprite->setPosition(Vector2f(-1100, -1000));
			return false;
		}
		
	}

	void setFacing(bool right) {

		if (right != faceRight) {
			shootSprite->swapAll();
			blockSprite->swapAll();
		}



		faceRight = right;
	}
};