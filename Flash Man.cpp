#include "master.cpp"
#include "sniper bullet.cpp"
#include "angles.cpp"
#pragma once

class FlashMan : public Master {
	using Master::Master;
	IntRect idle = IntRect(1, 27, 30, 26);

	list<IntRect> flashList{ idle, IntRect(174, 25, 30, 29), IntRect(209, 29, 29, 24), IntRect(242, 29, 29, 24), IntRect(275, 29, 29, 24), IntRect(341, 29, 29, 24), IntRect(275, 29, 29, 24), IntRect(242, 29, 29, 24), IntRect(209, 29, 29, 24) };
	shared_ptr<animation> flashAnim;
	shared_ptr<animTimer> flashTimer;

	shared_ptr<animation> shootAnim;
	
	shared_ptr<animation> walkAnim;
	shared_ptr<animTimer> walkTimer;

	shared_ptr<animation> jumpAnim;

	int walkSpeed = 300;
	int hopSpeed = 1000;

	float walkTime = 2;
	float walkTime_left = walkTime;

	float shootTime = 0.25;
	float shootTime_left = shootTime;

	shared_ptr<Sound> flashSound;

	void initial() {
		code = "flash man";
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\flash man.png");
		phys->setTexture(t);
		phys->setRect(idle);

		introAnim = shared_ptr<animation>(new animation(flashList, phys));
		introTimer = shared_ptr<animTimer>(new animTimer(introAnim, 8, false));

		introAnim->setOffsetList(list<Vector2f>{ Vector2f(0, 0), Vector2f(0 * 4, -2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4)});

		flashAnim = shared_ptr<animation>(new animation(flashList, phys));
		flashTimer = shared_ptr<animTimer>(new animTimer(flashAnim, 8, false));
		flashAnim-> setOffsetList(list<Vector2f>{ Vector2f(0, 0), Vector2f(0 * 4, -2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4)});

		walkAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(40, 27, 25, 26), IntRect(70, 26, 26, 27)}, phys));
		walkTimer = shared_ptr<animTimer>(new animTimer(walkAnim, 8, true));
		walkAnim->setOffsetList(list<Vector2f>{ Vector2f(4 * 4, 0 * 4), Vector2f(1 * 4, 0 * 4) });

		damageSpritePos = Vector2i(523, 0);

		jumpAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(104, 16, 30, 36)}, phys));
		jumpAnim->setOffsetList(list<Vector2f>{ Vector2f(-1 * 4, -10 * 4) });

		shootAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(378, 28, 37, 25)}, phys));
		shootAnim->setOffsetList(list<Vector2f>{ Vector2f(-7 * 4, 1 * 4) });

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 30, 26), phys));
		hurt = hit;

		masterInitial("time stopper");
		damage = 5;

		grounded = false;
		state = walking;
	}

	void loadSound(shared_ptr<SoundCollection> sb) {
		flashSound = sb->getFlash();
	}

	enum State {
		walking, flashing, shooting
	};
	State state;

	bool freezing = false;

	void hitRight(shared_ptr<tile> t) {
		if (t->getGround() != NULL) {
			if (grounded) {
				shortHop();
			}
		}
		else {
			setFacing(true);
		}
	}

	void hitLeft(shared_ptr<tile> t) {
		if (t->getGround() != NULL) {
			if (grounded) {
				shortHop();
			}
		}
		else {
			setFacing(false);
		}
	}

	void setFacing(bool right) {
		if (right != faceRight) {
			walkAnim->swapAll();
			jumpAnim->swapAll();
			shootAnim->swapAll();
			flashAnim->swapAll();

			if (!right) {
				flashAnim->setOffsetList(list<Vector2f>{ Vector2f(0, 0), Vector2f(0 * 4, -2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4), Vector2f(2 * 4, 2 * 4)});
				walkAnim->setOffsetList(list<Vector2f>{Vector2f(4 * 4, 0 * 4), Vector2f(1 * 4, 0 * 4)});
				shootAnim->setOffsetList(list<Vector2f>{ Vector2f(-7 * 4, 1 * 4) });
				jumpAnim->setOffsetList(list<Vector2f>{ Vector2f(-1 * 4, -10 * 4) });
			}
			else {
				flashAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0 * 4, -2 * 4), Vector2f(-1 * 4, 2 * 4), Vector2f(-1 * 4, 2 * 4), Vector2f(-1 * 4, 2 * 4), Vector2f(-1 * 4, 2 * 4), Vector2f(-1 * 4, 2 * 4), Vector2f(-1 * 4, 2 * 4), Vector2f(-1 * 4, 2 * 4)});
				walkAnim->setOffsetList(list<Vector2f>{	Vector2f(1 * 4, 0 * 4), Vector2f(3 * 4, 0 * 4) });
				shootAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 1 * 4) });
				jumpAnim->setOffsetList(list<Vector2f>{Vector2f(1 * 4, -10 * 4)});
			}

		}
		faceRight = right;
	}

	void shortHop() {
		grounded = false;
		phys->setVVelocity(hopSpeed);
		jumpAnim->thisFrame();
	}

	bool walkLoop(float* deltaT) {
		walkTimer->run(deltaT);

		if (faceRight) {
			phys->move(Angle::right, deltaT, walkSpeed);
		}
		else {
			phys->move(Angle::left, deltaT, walkSpeed);
		}

		walkTime_left -= *deltaT;
		if (walkTime_left <= 0) {
			walkTime_left = walkTime;
			return true;
		}
		return false;

	}

	bool flashingLoop(shared_ptr<player> p, float* deltaT) {

		//list<shared_ptr<bullet>> pBullets = p->getRealBullets();

		//p->enableControls(false);
		

		flashTimer->run(deltaT);
		if (flashTimer->isFinished(deltaT)) {
			return true;
		}
		return false;
	}

	int shootAngle = -10;
	int shootAdd = 0;
	int shootAmount = 6;
	int shootAmount_left = shootAmount;

	bool shootLoop(list<shared_ptr<EnemyBullet>>* bList, float* deltaT) {

		shootTime_left -= *deltaT;

		if (shootTime_left <= 0) {
			shootTime_left = shootTime;

			if (!faceRight) {
				shoot(bList, shootAngle + shootAdd + 180);
			}
			else {
				shoot(bList, shootAngle + shootAdd);
			}

			shootAmount_left -= 1;

			shootAdd = (shootAdd + 10) % (30);

			if (shootAmount_left <= 0) {
				shootAmount_left = shootAmount;

				return true;
			}
		}

		return false;
	}

	void shoot(list<shared_ptr<EnemyBullet>>* bList, int angle) {
		shared_ptr<SniperBullet> newBul;
		if (faceRight) {
			newBul = shared_ptr<SniperBullet>(new SniperBullet(phys->getTexture(), Vector2f(phys->getEndPosition().x, phys->getPosition().y + (7*4)), angle));
		}
		else {
			newBul = shared_ptr<SniperBullet>(new SniperBullet(phys->getTexture(), Vector2f(phys->getPosition().x, phys->getPosition().y + (7 * 4)), angle));
		}

		newBul->spriteSetup(phys->getTexture(), newBul->getPosition(), IntRect(123, 125, 8, 8));

		bList->push_back(newBul);
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		

		if (!grounded) {
		phys->eachFrame(deltaT);
		}

		if (state == walking) {
			if (walkLoop(deltaT)) {
				state = flashing;
				flashAnim->thisFrame();
				checkDirection(p->getSprite());
				flashSound->play();
			}
		}

		else if (state == flashing) {

			

			if (flashingLoop(p, deltaT)) {
				state = shooting;
				checkDirection(p->getSprite());
				flashAnim->reset();
				shootAnim->thisFrame();
				p->enableMoving(false);
			}
		}

		else if (state == shooting) {
			if (shootLoop(bList, deltaT)) {
				p->enableMoving(true);
				state = walking;
				walkAnim->thisFrame();
			}
		}


		tileCollision(tileList);
	}

	int crashDam() {
		return 4;
	}

	//Not sure if this is needed, or if it will cause the player to move during win animation
	void uniqueDeathLoop(float* deltaT, shared_ptr<player> p) {
		//p->enableMoving(true);

	}
};