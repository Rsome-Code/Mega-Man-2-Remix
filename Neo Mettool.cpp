#include "physics enemy.cpp"
#include "sniper bullet.cpp"
#pragma once

class NeoMettool : public PhysicsEnemy {

	using PhysicsEnemy::PhysicsEnemy;

	shared_ptr<Sound> shootSound;

	shared_ptr<animation> hideAnim;
	shared_ptr<animTimer> hideTimer;

	shared_ptr<animation> startAnim;
	shared_ptr<animTimer> startTimer;

	shared_ptr<animation> runAnim;
	shared_ptr<animTimer> runTimer;

	float runTime = 1;
	float runTime_left = runTime;

	int runSpeed = 500;

	float runDelay = 0.5;
	float runDelay_left = runDelay;

	int runAngle = 0;

	enum State {
		hiding, gettingUp, running
	};
	State state = hiding;

	void initial() {
		hp = 1;
		damage = 3;

		phys->setPosition(initialPos);

		phys->setRect(IntRect(249, 465, 18, 11));
		phys->setScale(Vector2f(4, 4));

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 18, 19), false, sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(-9999999999, -9999999999, 0,0 ), false, sprite));

		hideAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(274, 462, 18, 19), IntRect(249, 465, 18, 11)}, sprite));
		hideTimer = shared_ptr<animTimer>(new animTimer(hideAnim, 8, false));
		
		startAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(249, 465, 18, 11), IntRect(274, 462, 18, 14), IntRect(299, 457, 18, 19)}, sprite));
		startTimer = shared_ptr<animTimer>(new animTimer(startAnim, 8, false));

		runAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(299, 457, 18, 19), IntRect(324, 457, 18, 19)}, sprite));
		runTimer = shared_ptr<animTimer>(new animTimer(runAnim, 8, true));


		startAnim->setOffsetList(list<Vector2f>{ Vector2f(0, 8*4), Vector2f(0, 5*4), Vector2f(0, 0*4) });
		hideAnim->setOffsetList(list<Vector2f>{ Vector2f(0, 5*4), Vector2f(0, 8*4) });
		runAnim->setOffsetList(list<Vector2f>{ Vector2f(0, 0*4), Vector2f(0, 0*4) });

		code = "neo mettool";

		phys->enableGravity(true);

		offSetList();
		state = hiding;
	}

	void setFacing(bool right) {
		if (faceRight != right) {
			faceRight = right;

			hideAnim->swapAll();
			startAnim->swapAll();
			runAnim->swapAll();


		}
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		if (!grounded) {
			phys->eachFrame(deltaT);
		}
		else {
			phys->setVVelocity(0);
		}
		tileCollision(tileList, deltaT);

		
	

		if (state == hiding) {
			hideTimer->run(deltaT);
			checkPlayer(p, bList);
			checkDirection(p->getSprite());
		}

		else if (state == gettingUp) {
			startTimer->run(deltaT);
			runDelay_left = runDelay_left - *deltaT;
			if (runDelay_left <= 0) {
				runDelay_left = runDelay;
				startTimer->reset();
				startAnim->reset();

				runDelay_left = runDelay;
				state = running;
				shoot(bList);
				checkDirection(p->getSprite());
				if (faceRight) {
					runAngle = 0;
				}
				else {
					runAngle = 180;
				}
			}
		}

		else if (state == running) {

			runTimer->run(deltaT);

			phys->move(runAngle, deltaT, runSpeed);

			runTime_left = runTime_left - *deltaT;
			if (runTime_left <= 0) {
				runTime_left = runTime;
				state = hiding;
				hideAnim->thisFrame();

				//hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 18, 11), false, sprite));
				hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(-9999999999, -9999999999, 0, 0), false, sprite));
				//grounded = false;
			
			}
		}


		

	}

	void checkPlayer(shared_ptr<player> p, list<shared_ptr<EnemyBullet>>* bList) {
		if (Maths::getDistance(phys->getPosition(), p->getPosition()) < 400) {
			state = gettingUp;

			

			hideTimer->reset();
			hideAnim->reset();

			//hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 18, 19), false, sprite));
			hurt = hit;
			//grounded = false;
			
			
		}
	}

	void loadSound(shared_ptr<SoundCollection> soundCol) {
		shootSound = soundCol->getShoot();
	}

	void shoot(list<shared_ptr<EnemyBullet>>* eBullets) {
		int range = 30;
		int ang;
		if (faceRight) {
			ang = 0;
		}
		else {
			ang = 180;
		}
		for (int i = 0; i < 3; i++) {
			int tempAng = Maths::map(0, 2, ang - range, ang + range, i);

			eBullets->push_back(shared_ptr<SniperBullet>(new SniperBullet(sprite->getTexture(), Vector2f(sprite->getPosition().x + 9, sprite->getPosition().y + 9), tempAng)));
		}
		shootSound->play();
	}

};