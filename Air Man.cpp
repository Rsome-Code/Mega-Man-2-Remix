#include "master.cpp"
#include "boss air bullet.cpp"
#pragma once

class AirMan : public Master {
	using Master::Master;

	IntRect idle = IntRect(1, 23, 32, 30);
	list<IntRect> blow = { IntRect(83, 25, 37, 28), IntRect(124, 25, 37, 28) };
	list<IntRect> poseList = { idle, blow.front(), blow.back(), idle, blow.front(), blow.back(), idle, blow.front(), blow.back(), idle, blow.front(), blow.back(), idle };

	shared_ptr<animation> idleAnim;

	shared_ptr<animation> blowAnim;
	shared_ptr<animTimer> blowTimer;

	shared_ptr<animation> jumpAnim;

	shared_ptr<animation> shootAnim;

	shared_ptr<Sound> shootSound;

	enum State {
		blowing, jumping
	};

	State state;

	void initial() {
		code = "air man";
		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture());
		t->loadFromFile("assets\\air man.png");

		phys->setTexture(t);
		phys->setRect(idle);

		introAnim = shared_ptr<animation>(new animation(poseList, phys));
		introAnim->setOffsetList(list<Vector2f>{ Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4)});
		introTimer = shared_ptr<animTimer>(new animTimer(introAnim, 8, false));

		blowAnim = shared_ptr<animation>(new animation(blow, phys));
		blowAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4)});
		blowTimer = shared_ptr<animTimer>(new animTimer(blowAnim, 8, true));

		jumpAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(210, 25, 37, 30)}, phys));

		shootAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(40, 25, 38, 28)}, phys));
		shootAnim->setOffsetList(list<Vector2f>{Vector2f(-5 * 4, 2 * 4)});

		idleAnim = shared_ptr<animation>(new animation(list<IntRect>{idle}, phys));

		damage = 4;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 32, 30), phys));
		hurt = hit;

		masterInitial("air shooter");

		state = blowing;
		blowSpeed = 10;
		shootNum_left = shootNum;
		jumps_left = jumps;
	}

	void setFacing(bool r) {

		if (faceRight != r) {
			jumpAnim->swapAll();
			idleAnim->swapAll();
			shootAnim->swapAll();
			blowAnim->swapAll();

			if (r) {
				//introAnim->setOffsetList(list<Vector2f>{ Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4)});
				blowAnim->setOffsetList(list<Vector2f>{Vector2f(-4 * 4, 2 * 4), Vector2f(-4 * 4, 2 * 4)});
				shootAnim->setOffsetList(list<Vector2f>{Vector2f(-5 * 4, 2 * 4)});
			}
			else {
				//introAnim->setOffsetList(list<Vector2f>{ Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4)});
				blowAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 2 * 4), Vector2f(0 * 4, 2 * 4)});
				shootAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 2 * 4)});
			}
		}

		faceRight = r;


	}

	float blowSpeed = minBlowSpeed;
	int minBlowSpeed = 10;
	int maxBlowSpeed = 700;
	float blowAccel = 400;

	int shootNum = 3;
	int shootNum_left = shootNum;

	void blowLoop(float* deltaT, shared_ptr<player> p) {
		if (faceRight) {
			p->getSprite()->move(Angle::right, deltaT, blowSpeed);
		}
		else {
			p->getSprite()->move(Angle::left, deltaT, blowSpeed);
		}

		blowSpeed += (blowAccel * *deltaT);
		if (blowSpeed > maxBlowSpeed) {
			blowSpeed = maxBlowSpeed;
		}
	}



	void shootCheck(list<shared_ptr<EnemyBullet>>* bList, list<shared_ptr<GameObject>>* obList) {
		bool bulletsOnScreen = false;
		for (shared_ptr<EnemyBullet> bul : *bList) {
			if (bul->getCode() == "boss air bullet") {
				bulletsOnScreen = true;
			}
		}

		if (!bulletsOnScreen) {
			shootNum_left--;
			if (shootNum_left >= 0) {
				shoot(bList, obList);
			}
			else {
				state = jumping;
				shootNum_left = shootNum;
			}
		}
	}

	int bulletNum = 10;

	

	void shoot(list<shared_ptr<EnemyBullet>>* bList, list<shared_ptr<GameObject>>* obList) {

		shootAnim->thisFrame();

		bool keepShooting = true;

		int i = 0;

		while (keepShooting){
			Vector2f bulPos = posDecide(obList, i);
			if (bulPos != Vector2f(0, 0)) {
				bList->push_back(shared_ptr<BossAirBullet>(new BossAirBullet(phys->getTexture(), bulPos, sprite->getPosition(), faceRight)));
			}
			else {
				keepShooting = false;
			}

			i++;
		}

	}

	int pattern = 0;

	Vector2f posDecide(list<shared_ptr<GameObject>>* obList, int targNum) {
		if (pattern == 1) {
			for (shared_ptr<GameObject> ob : *obList) {
				if (ob->getCode() == "air target-1") {
					if (targNum <= 0) {
						return ob->getPosition();
					}
					targNum--;
				}
			}
			
		}
		else if (pattern == 2) {
			for (shared_ptr<GameObject> ob : *obList) {
				if (ob->getCode() == "air target-2") {
					if (targNum <= 0) {
						return ob->getPosition();
					}
					targNum--;
				}
			}
		}
		else if (pattern == 0) {
			for (shared_ptr<GameObject> ob : *obList) {
				if (ob->getCode() == "air target-0") {
					if (targNum <= 0) {
						return ob->getPosition();
					}
					targNum--;
				}
			}
		}

		pattern = rand() % 3;

		return Vector2f(0, 0);
	}


	int shortJumpForce = 500;
	int longJumpForce = 1300;

	int jumps = 2;
	int jumps_left = jumps;

	void jump() {
		if (jumps_left == 1) {
			shortJump();
		}
		else {
			longJump();
		}

		jumpAnim->thisFrame();
	}

	void shortJump() {
		phys->setVVelocity(shortJumpForce);
	}

	void longJump() {
		phys->setVVelocity(longJumpForce);
	}

	int jumpSpeed = 600;

	void jumpMove(float* deltaT) {
		if (faceRight) {
			phys->move(Angle::right, deltaT, jumpSpeed);
		}
		else {
			phys->move(Angle::left, deltaT, jumpSpeed);
		}
	}

	void jumpCheck() {
		if (grounded) {
			jumps_left--;
			if (jumps_left >= 0) {
				jump();
			}
			else {
				setFacing(!faceRight);
				state = blowing;
				blowSpeed = minBlowSpeed;
				jumps_left = jumps;
			}
		}
	}



	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<GameObject>>* obList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {

		if (state == blowing) {

			if (grounded) {
				blowTimer->run(deltaT);

				blowLoop(deltaT, p);


				shootCheck(bList, obList);
			}

		}

		else if (state == jumping) {

			jumpCheck();

			jumpMove(deltaT);

			//phys->eachFrame(deltaT);

			
		}
		phys->eachFrame(deltaT);
		tileCollision(tileList);
		
		hit->updatePos();

	}
};