#include "master.cpp"
#include "leaf up shot.cpp"
#include "leaf shield bullet.cpp"
#include "leaf float.cpp"
#pragma once

class WoodMan : public Master {
	using Master::Master;
	enum State {
		IDLE, BEATING, JUMPING, IDLESHOOT, IDLEJUMP, SHOOTING
	};

	

	IntRect idle = IntRect(1, 22, 32, 31);
	IntRect duck = IntRect(38, 23, 32, 30);
	IntRect jump = IntRect(75, 22, 32, 29);
	IntRect beatRight = IntRect(112, 23, 32, 30);
	IntRect beatLeft = IntRect(145, 23, 32, 30);

	IntRect throw1 = IntRect(187, 26, 29, 27);
	IntRect throw2 = IntRect(221, 21, 36, 32);

	animation* beatAnim;
	animTimer* beatTimer;

	animation* throwAnim;
	animTimer* throwTimer;

	animation* idleAnim;

	float jumpForce = 1000;

	float moveAngle = 180;
	float speed = 300;

	int maxUpshots = 4;
	int upShots = 0;

	float upShotDelay = 0.3;
	float upShotDelay_left = upShotDelay;

	float floatPos;
	float floatMult = 200;

	float delayBeforeShot = 0.5;
	float delayBeforeShot_left = delayBeforeShot;

	float jumpDelay = 0.25;
	float jumpDelay_left = jumpDelay;

	float idleTime = 4;
	float idleTime_left = idleTime;

	LeafUpShot* lastShot = NULL;

	vector<Vector2f> corners;
	State state = BEATING;

	vector<LeafShieldBullet*> shield;

	bool newState = true;

public:
	void initial() {

		deathAnimation = new DeathAnim(sprite, "Leaf Shield");

		Texture* t = new Texture();
		t->loadFromFile("assets\\wood man.png");
		phys->setTexture(t);

		phys->setRect(IntRect(295, 22, 32, 29));

		introAnim = new animation(list<IntRect>{idle, beatRight, beatLeft, beatRight, beatLeft, beatRight, beatLeft, beatRight, beatLeft}, sprite);
		introTimer = new animTimer(introAnim, 12, false);

		beatAnim = new animation(list<IntRect>{beatRight, beatLeft}, sprite);
		beatTimer = new animTimer(beatAnim, 12, true);

		hit = new objectHitbox(IntRect(0, 0, 32, 31), sprite);
		hurt = new objectHitbox(IntRect(0, 0, 32, 31), sprite);

		idleAnim = new animation(list<IntRect> {idle}, sprite);

		throwAnim = new animation(list <IntRect>{ throw1, throw2 }, sprite);
		throwTimer = new animTimer(throwAnim, 4, false);

		masterInitial(string("wood man"));
		
		setCode("wood man");
		damage = 5;
		
		floatPos = phys->getPosition().x - 600;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		if (state == BEATING) {
			if (newState) {
				newState = false;
				corners = { Vector2f(sprite->getPosition().x + 8, sprite->getPosition().y + 8) , Vector2f(sprite->getPosition().x  + sprite->getSize().x - 8, sprite->getPosition().y + 8), Vector2f(sprite->getPosition().x + 8 , sprite->getPosition().y + sprite->getSize().y -8), Vector2f(sprite->getPosition().x + sprite->getSize().x -8, sprite->getPosition().y + sprite->getSize().y-8) };
				deployShield(bList);
			}
			if (beatLoop(deltaT, bList)) {
				state = IDLESHOOT;
				newState = true;
			}
			hp = 1;
		}

		else if (state == IDLESHOOT) {
			if (idleShootLoop(deltaT, bList)) {
				state = SHOOTING;
			}
		}

		else if (state == SHOOTING) {
			if (shootLoop(deltaT, bList)) {
				state = IDLEJUMP;
				throwAnim->reset();
				newState = true;
				grounded = false;
			}
		}

		else if (state == IDLEJUMP) {
			jumpDelay_left -= *deltaT;
			if (jumpDelay_left <= 0) {
				jumpDelay_left = jumpDelay;
				jumpF();
				state = JUMPING;
			}
		}

		else if (state == JUMPING) {
			if (jumpLoop(deltaT, tileList)) {
				state = IDLE;
				newState = true;
				
			}
		}

		else if (state == IDLE) {
			if (idleLoop(deltaT, bList)) {
				state = BEATING;
				newState = true;
			}
		}

	}

	bool idleShootLoop(float* deltaT, list<EnemyBullet*>* bList) {
		delayBeforeShot_left -= *deltaT;
		if (delayBeforeShot_left <= 0){
			delayBeforeShot_left = delayBeforeShot;
			return true;
		}
		else {
			idleLoop(deltaT, bList);
		}
		return false;
	}

	bool shootLoop(float* deltaT, list<EnemyBullet*>* bList) {

		throwTimer->run(deltaT);
		if (throwTimer->isFinished()) {

			for (LeafShieldBullet* b : shield) {
				b->shoot();
				b->setRight(faceRight);
			}
			return true;
		}
		return false;
	}

	bool idleLoop(float* deltaT, list<EnemyBullet*>* bList) {
		idleAnim->thisFrame();
		return bList->empty();

	}

	void jumpF() {
		phys->setVVelocity(jumpForce);

		if (faceRight == true){
			moveAngle = 0;
		}
		else {
			moveAngle = 180;
		}
	}

	bool jumpLoop(float* deltaT, list<tile*>* tileList) {

			phys->eachFrame(deltaT);
			if (phys->getVVelocity() < 0) {
				tileCollision(tileList);
			}
			phys->move(moveAngle, deltaT, speed);
			return grounded;
		
		return false;
	}

	bool beatLoop(float* deltaT, list<EnemyBullet*>* bList) {
		if (upShots < maxUpshots) {
			shootUpwards(*deltaT, bList);
		}
		else if (lastShot->getSprite()->getCameraPosition().y < 0){
			spawnFloatLeaves(bList);
			upShots = 0;
			
			return true;
		}
		beatTimer->run(deltaT);
		return false;
	}

	void shootUpwards(float deltaT, list<EnemyBullet*>* bList) {
		upShotDelay_left -= deltaT;
		if (upShotDelay_left <= 0) {
			upShotDelay_left = upShotDelay;
			lastShot = new LeafUpShot(sprite->getTexture(), sprite->getPosition());
			bList->push_back(lastShot);
			upShots++;
		}
	}

	void spawnFloatLeaves(list<EnemyBullet*>* bList) {
		for (int i = 0; i < 4; i++) {
			bList->push_back(new LeafFloat(sprite->getTexture(), Vector2f(floatPos + (i * floatMult), sprite->getPosition().y - 600)));
		}
	}

	void deployShield(list<EnemyBullet*>* bList) {
		for (int i = 0; i < 4; i++) {
			LeafShieldBullet* bull = new LeafShieldBullet(sprite->getTexture(), corners[i]);
			bull->setCorner(i);
			bList->push_back(bull);
			shield.push_back(bull);
		}
	}

	int atomicDam() {
		return 3;
	}
};