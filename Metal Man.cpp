#include "master.cpp"
#include "tile.cpp"
#include "boss blade.cpp"
#pragma once

class MetalMan : public Master {
	using Master::Master;
	IntRect idle = IntRect(1, 27, 24, 24);
	IntRect jumpFrame = IntRect(173, 19, 23, 28);
	animation* jumpAnim;
	animation* walkAnim;
	animTimer* walkTimer;

	animation* throwAnim;
	animTimer* throwTimer;



	bool convRight;

	enum State {
		walk, jumpShoot, shooting, jumpAcross
	};
	enum JumpType {
		low, medium, high
	};
	State state;
	JumpType jumpType = low;


	float untilShoot = 2.5;
	float untilShoot_left = untilShoot;

	int throwNum = 1;

	float throwDelay = 0.4;
	float throwDelay_left = throwDelay;

	Sound* throwSound = NULL;

	void ini() {
		Texture* t = new Texture();
		t->loadFromFile("assets\\metal man.png");
		phys->setTexture(t);

		phys->setRect(jumpFrame);
		sprite = phys;

		introAnim = new animation(list<IntRect>{idle, IntRect(34, 28, 28, 23), IntRect(64, 27, 23, 24)}, sprite);
		//introAnim->setOffsetList(list<Vector2f>{});
		introTimer = new animTimer(introAnim, 8, false);

		walkAnim = new animation(list<IntRect>{IntRect(97, 29, 24, 22), IntRect(127, 28, 16, 23), IntRect(145, 29, 21, 22), IntRect(127, 28, 16, 23)}, sprite);
		walkAnim->setOffsetList(list<Vector2f>{Vector2f(-1 * 4, 2 * 4), Vector2f(4 * 4, 1 * 4), Vector2f(3 * 4, 2 * 4), Vector2f(4 * 4, 1 * 4)});
		walkTimer = new animTimer(walkAnim, 8, true);

		throwAnim = new animation(list<IntRect>{IntRect(208, 11, 21, 36), IntRect(236, 20, 21, 27)}, sprite);
		throwAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, -8 * 4), Vector2f(0 * 4, 0 * 4)});
		throwTimer = new animTimer(throwAnim, 8, false);
		convRight = false;
		State state = walk;

		code = "metal man";
		acrossAngle = 180;

		jumpAnim = new animation(list<IntRect>{IntRect(jumpFrame)}, phys);
		jumpAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, -4 * 4)});
		masterInitial("metal blade");

		hit = new objectHitbox(IntRect(0,0, 24, 24), phys);
		hurt = hit;

		deathAnim->setSprite(sprite);
		acrossShot = false;

		damage = 5;
	}

	void initial() {
		ini();
	}


	float conveyorSwap = 5;
	float swap_left = conveyorSwap;

	int acrossSpeed = 600;
	int acrossAngle = 180;
	float acrossShootDelay = 0.5;
	float acrossShootDelay_left = acrossShootDelay;
	bool acrossShot = false;

	void swapConveyors(list<tile*>* tileList) {
		convRight = !convRight;
		for (tile* t : *tileList) {
			t->setMoveRight(convRight);
		}

	}

	void conveyorCheck(float* deltaT, list<tile*>* tileList) {
		swap_left -= *deltaT;
		if (swap_left <= 0) {
			swap_left = conveyorSwap;
			swapConveyors(tileList);
		}
	}

	int quickDam() {
		return 4;
	}
	int metalDam() {
		return 14;
	}

	bool firstFrame = true;
	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {

		

		if (throwSound == NULL) {
			throwSound = p->getMetalBlade()->getSound();
		}

		conveyorCheck(deltaT, tileList);

		if (state == walk) {
			
			walkTimer->run(deltaT);
			untilShoot_left -= *deltaT;
			if (p->getController()->checkB()) {
				untilShoot_left = 0;
			}
			if (untilShoot_left <= 0) {
				untilShoot_left = untilShoot;
				state = jumpShoot;
				jumpType = JumpType(rand() % 3);
				jump();
			}
			if (distanceCheck(p->getSprite())) {
				state = jumpAcross;
				acrossShootDelay_left = acrossShootDelay;
				phys->setVVelocity(jumpForce * 2);
				
			}

			
		}

		else if (state == jumpShoot) {
			jumpAnim->thisFrame();
			phys->eachFrame(deltaT);
			tileCollision(tileList);
			throwDelay_left -= *deltaT;
			if (throwDelay_left <= 0) {
				throwDelay_left = 0.3;
				//throwBlade(bList);
				
				if (throwNum > 0) {

					phys->setVVelocity(0);
					state = shooting;
					throwTimer->reset();
					throwAnim->reset();
					throwNum--;
				}
				
				
			}
			if (grounded) {
				state = walk;
				throwDelay_left = throwDelay;
				walkAnim->thisFrame();
			}
		}

		else if (state == shooting) {
			throwAnim->thisFrame();
			throwTimer->run(deltaT);
			
			phys->eachFrame(deltaT);
			
			if (throwTimer->isFinished()) {
				
				throwBlade(bList, p->getSprite());
				state = jumpShoot;
			}

		}

		else if (state == jumpAcross) {
			jumpAnim->thisFrame();
			phys->eachFrame(deltaT);
			tileCollision(tileList);

			phys->move(acrossAngle, deltaT, acrossSpeed);

			acrossShootDelay_left -= *deltaT;
			if (acrossShootDelay_left <= 0) {
				acrossShootDelay_left = acrossShootDelay;
				if (!acrossShot) {
					throwBlade(bList, p->getSprite());
					acrossShot = true;
				}
				
			}

			if (grounded) {
				setFacing(!faceRight);
				state = walk;
				acrossShot = false;
				walkAnim->thisFrame();
			}
		}

	}

	void setFacing(bool r) {
		if (r != faceRight) {
			walkAnim->swapAll();
			jumpAnim->swapAll();
			acrossAngle = (acrossAngle + 180) % 360;
			throwAnim->swapAll();

			if (r) {
				walkAnim->setOffsetList(list<Vector2f>{Vector2f(1 * 4, 2 * 4), Vector2f(4 * 4, 1 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(4 * 4, 1 * 4)});
			}
			else {
				walkAnim->setOffsetList(list<Vector2f>{Vector2f(-1 * 4, 2 * 4), Vector2f(4 * 4, 1 * 4), Vector2f(3 * 4, 2 * 4), Vector2f(4 * 4, 1 * 4)});
			}
		}
		faceRight = r;
	}

	void throwBlade(list<EnemyBullet*>* bList, objectSprite* p) {
		throwSound->play();
		phys->setVVelocity(bounceForce);

		BossBlade* temp = new BossBlade(sprite->getTexture(), sprite->getPosition(), p->getMiddlePos());
		bList->push_back(temp);

	}
	int jumpForce = 1000;
	int bounceForce = 400;
	void jump() {
		phys->setVVelocity((jumpForce * (jumpType/1.6)) + jumpForce);
		throwNum = jumpType + 1;
	}

	bool distanceCheck(objectSprite* p) {
		if (fabs(p->getMiddlePos().x - sprite->getMiddlePos().x) < (48 * 4)) {
			return true;
		}
		return false;
	}
};