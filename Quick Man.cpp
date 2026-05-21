#include "master.cpp"
#include "Boss Boomerang.cpp"
#include "angles.cpp"
#pragma once

class QuickMan : public Master {
	using Master::Master;

	IntRect idle = IntRect(1, 25, 24, 28);
	IntRect jumpFrame = IntRect(82, 72, 26, 34);
	IntRect shootFrame = IntRect(118, 73, 28, 33);

	shared_ptr<animation> walkAnim;
	shared_ptr<animTimer> walkTimer;

	shared_ptr<animation> jumpAnim;
	shared_ptr<animation> shootAnim;
	shared_ptr<animTimer> shootTimer;

	bool shot = false;

	float jumpSpeed = 1800;
	float jumpMoveSpeed = 400;
	float hopSpeed = 1200;
	float runSpeed = 600;

	int gravity = 5000;

	enum State {
		running, jumping
	};
	State state;
	float runTime = 3;
	float runTime_left = runTime;

	int maxJumps = 3;
	int jumpsLeft = maxJumps;

	shared_ptr<TimeStopper> timeStopper;
	int timeAmmo;

	bool firstLoop = true;

	void initial() {
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\quick man.png");
		phys->setTexture(t);
		phys->setRect(idle);
		phys->setPosition(initialPos);
		mov = phys;
		introAnim = shared_ptr<animation>(new animation(list<IntRect>{idle, IntRect(33, 25, 29, 28), IntRect(70, 27, 25, 26), IntRect(103, 27, 25, 26), IntRect(169, 27, 25, 26), IntRect(202, 27, 25, 26), IntRect(235, 22, 25, 31), IntRect(271, 24, 25, 29), IntRect(308, 22, 35, 31)}, sprite));
		introAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(-5 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, -3 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, -3 * 4)});
		introTimer = shared_ptr<animTimer> (new animTimer(introAnim, 8, false));

		jumpAnim = shared_ptr<animation>(new animation(list<IntRect>{jumpFrame}, mov));
		shootAnim = shared_ptr<animation>(new animation(list<IntRect>{shootFrame, jumpFrame}, mov));
		shootTimer = shared_ptr<animTimer> (new animTimer(shootAnim, 8, false));

		walkAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(2, 80, 24, 28), IntRect(27, 79, 24, 28), IntRect(53, 80, 24, 28)}, sprite));
		walkTimer = shared_ptr<animTimer> (new animTimer(walkAnim, 8, true));
		code = "quick man";

		masterInitial("quick boomerang");

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 24, 28), sprite));
		hurt = hit;

		phys->setGravity(-gravity);

		damage = 4;

		state = running;

	}

	



	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
			

		if (state == running) {
			if (grounded) {
				walkTimer->run(deltaT);
			}
			else {
				phys->eachFrame(deltaT);
			}
			
			if (faceRight) {
				phys->move(Angle::right, deltaT, runSpeed);
			}
			else {
				phys->move(Angle::left, deltaT, runSpeed);
			}

			runTime_left -= *deltaT;
			if (runTime_left <= 0 && grounded) {
				runTime_left = runTime;
				state = jumping;
				
				jump(p);
			}
		}

		else if (state == jumping) {
			if (grounded) {
				if (jumpsLeft > 0) {
					jump(p);
				}
				else {
					jumpsLeft = maxJumps;
					state = running;
				}
			}
			else {

				if (phys->getVVelocity() > 0 && jumpsLeft == 2) {
					phys->eachFrame(deltaT);
					if (phys->getVVelocity() <= 0) {
						shoot(bList);
					}
				}

				else {
					shootTimer->run(deltaT);
					phys->eachFrame(deltaT);
				}
			}

			jumpMovement(deltaT);

		}

		tileCollision(tileList);

	}

	void shoot(list<shared_ptr<EnemyBullet>>* bList) {
		int ang = 67.5;
		int angChange = 22.5;
		for (int i = 0; i < 3; i++) {
			shared_ptr<BossBoomerang> boomer = shared_ptr<BossBoomerang> (new BossBoomerang(mov->getTexture(), Vector2f(mov->getMiddlePos().x - 8, mov->getMiddlePos().y - 8), ang + (angChange * i)));
			bList->push_back(boomer);
		}

		shootAnim->reset();
		shootAnim->thisFrame();
		shootTimer->reset();
	}


	void jumpMovement(float* deltaT) {
		if (faceRight) {
			phys->move(0, deltaT, jumpMoveSpeed);
		}
		else {
			phys->move(180, deltaT, jumpMoveSpeed);
		}
	}



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

	void jump(shared_ptr<player> p) {
		grounded = false;
		phys->setVVelocity(jumpSpeed);
		jumpsLeft--;

		checkDirection(p->getSprite());
		jumpAnim->thisFrame();

	}
	void shortHop() {
		grounded = false;
		phys->setVVelocity(hopSpeed);
		jumpAnim->thisFrame();
	}

	void setFacing(bool right) {
		if (right != faceRight) {
			walkAnim->swapAll();
			jumpAnim->swapAll();
			shootAnim->swapAll();
		}
		faceRight = right;
	}

	//Do this after time stopper implementation
	bool freezeDam(shared_ptr<player> p) {
		timeStopper = p->getTimeStopper();

		if (timeStopper->getAmmo() < timeAmmo) {
			hp -= 1;
			if (hp == 0) {
				return true;
			}
		}

		timeAmmo = timeStopper->getAmmo();
		return false;
	}



	bool eachFrame(float* deltaT, shared_ptr<player> p, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* enemyList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {
		damPos = Vector2f(sprite->getPosition().x + (1 * 4), sprite->getPosition().y + (1 * 4));
		if (introDone) {
			if (hp > 0) {
				if (act && !frozen) {
					hit->updatePos();
					hurt->updatePos();
					alive(p, deltaT, tileList, enemyList, bList);
				}
			}
			else { return death(deltaT, enemyList); }

			healthBar->update(hp);
		}
		else {
			if (noMusic) {
				bossMusic->play();
				noMusic = false;
			}

			introLoop(deltaT, tileList, p);

		}

		invincibleLoop(deltaT);

		return false;
	}

};