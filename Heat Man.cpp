#include "master.cpp"
#include "fire ball.cpp"
#pragma once

class HeatMan : public Master {
	using Master::Master;

	enum State {
		shooting, charging, moving
	};
	
	State state;

	IntRect fall = IntRect(199, 21, 24, 29);

	IntRect stand = IntRect(97, 28, 29, 25);

	list<IntRect> igList = list<IntRect>{ IntRect(38, 33, 32, 20), IntRect(71, 27, 24, 26), stand };
	list<IntRect> flameList = list<IntRect>{ IntRect(129, 22, 32, 31), IntRect(163, 21, 31, 32) };

	float shootDelay = 2;
	float shootDelay_left = shootDelay;

	float chargeTime = 1.5;
	float chargeTime_left = chargeTime;

	float moveSpeed = 800;

	shared_ptr<animation> ignitionAnim;
	shared_ptr<animTimer> ignitionTimer;
	
	shared_ptr<animation> flameAnim;
	shared_ptr<animTimer> flameTimer;

	shared_ptr<animation> throwAnim;
	shared_ptr<animTimer> throwTimer;

	shared_ptr<animation> moveStartAnim;
	shared_ptr<animTimer> moveStartTimer;

	shared_ptr<animation> moveAnim;
	shared_ptr<animTimer> moveTimer;

	shared_ptr<animation> idleAnim;
	
	float untilIdle = 0.5;
	float untilIdle_left = 0;

	bool invincible = false;

	float moveTarget;

	bool isMoving = false;

	bool flaming = false;

public:
	void introSetup() {
		list <IntRect> introList;
		introList = igList;

		for (int i = 0; i < 4; i++) {
			for (IntRect rec : flameList) {
				introList.push_back(rec);
			}
		}

		introAnim = shared_ptr<animation>(new animation(introList, sprite));
		introAnim->setOffsetList(list<Vector2f>{Vector2f(-1 * 4, 5 * 4), Vector2f(3 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(-1 * 4, -6 * 4), Vector2f(0 * 4, -7 * 4), Vector2f(-1 * 4, -6 * 4), Vector2f(0 * 4, -7 * 4), Vector2f(-1 * 4, -6 * 4), Vector2f(0 * 4, -7 * 4), Vector2f(-1 * 4, -6 * 4), Vector2f(0 * 4, -7 * 4), Vector2f(-1 * 4, -6 * 4), Vector2f(0 * 4, -7 * 4), Vector2f(-1 * 4, -6 * 4), Vector2f(0 * 4, -7 * 4)});
		introTimer = shared_ptr<animTimer> (new animTimer(introAnim, 6, false));
		
	}

	void initial() {
		state = shooting;

		introSetup();

		deathAnimation = shared_ptr<DeathAnim>(new DeathAnim(sprite, "Atomic Fire"));

		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\heat man.png");
		phys->setTexture(t);

		phys->setRect(fall);
		phys->setScale(Vector2f(4, 4));
		mov = phys;

		ignitionAnim = shared_ptr<animation>(new animation(igList, sprite));
		ignitionAnim->setOffsetList(list<Vector2f>{Vector2f(-1 * 4, 5 * 4), Vector2f(3 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4)});
		ignitionTimer = shared_ptr<animTimer> (new animTimer(ignitionAnim, 6, false));
		
		flameAnim = shared_ptr<animation>(new animation(flameList, sprite));
		flameAnim->setOffsetList(list<Vector2f>{Vector2f(-1 * 4, -6 * 4), Vector2f(0 * 4, -7 * 4)});
		flameTimer = shared_ptr<animTimer> (new animTimer(flameAnim, 6, true));

		throwAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(258, 28, 27, 25), IntRect(286, 25, 38, 28)}, sprite));
		throwAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(-4 * 4, -3 * 4)});
		throwTimer = shared_ptr<animTimer> (new animTimer(throwAnim, 6, false));


		masterInitial(string("heat man"));

		setCode("heat man");
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 24, 24), sprite));
		hurt = hit;

		moveAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(97, 142, 48, 16), IntRect(146, 146, 48, 8), IntRect(195, 145, 48, 10)}, sprite));
		moveAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 4 * 4), Vector2f(0 * 4, 3 * 4), Vector2f(0 * 4, 0 * 4) });
		moveTimer = shared_ptr<animTimer> (new animTimer(moveAnim, 12, true));

		moveStartAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(22, 125, 16, 48), IntRect(39, 133, 24, 32)}, sprite));
		moveStartAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, -16 * 4), Vector2f(0 * 4, -8 * 4)});
		moveStartTimer = shared_ptr<animTimer> (new animTimer(moveStartAnim, 6, false));

		idleAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(2, 28, 29, 25)}, sprite));
		isMoving = false;
		flaming = false;
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		//updateHitSize();

		tileCollision(tileList);

		if (state == shooting) {
			
			checkDirection(p->getSprite());

			untilIdle_left -= *deltaT;
			if (untilIdle_left <= 0) {
				idleAnim->thisFrame();
			}
			invincible = false;
			shootDelay_left -= *deltaT;
			if (shootDelay_left <= 0) {
				untilIdle_left = untilIdle;
				throwTimer->run(deltaT);
				if (throwTimer->isFinished()) {
					shootDelay_left = shootDelay;
					shoot(p, bList);
					throwAnim->reset();
					throwTimer->reset();
				}
			}
			if (damaged) {
				state = charging;
				shootDelay_left = shootDelay/2;
			}
		}

		else if (state == charging) {
			invincible = true;
			chargeLoop(deltaT, p);
		}

		else if (state == moving) {
			invincible = true;
			moveLoop(deltaT);
		}

		if (invincible) {
			hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(4, 4, 0, 0), sprite));
		}
		else {
			hurt = hit;
		}

	}

	void updateHitSize() {
		if (!flaming) {
			hit->setRelativeRect(IntRect(0, 0, sprite->getSize().x, sprite->getSize().y));
		}
		else {
			hit->setRelativeRect(IntRect(0, 0, sprite->getSize().x, sprite->getSize().y-4));
		}
	}

	void moveLoop(float* deltaT) {
		if (moveStartTimer->run(deltaT)) {
			if (moveStartTimer->isFinished()) {
				isMoving = true;
			}
		}
		else if (!isMoving) {
			moveStartAnim->thisFrame();
		}

		if (isMoving) {
			moveTimer->run(deltaT);

			if (faceRight) {
				mov->move(0, deltaT, moveSpeed);
				if (mov->getPosition().x + mov->getSize().x >= moveTarget) {
					state = shooting;
					damaged = false;
					moveStartAnim->reset();
					moveStartTimer->reset();
					isMoving = false;
				}
			}
			else {
				mov->move(180, deltaT, moveSpeed);
				if (mov->getPosition().x <= moveTarget) {
					state = shooting;
					damaged = false;
					moveStartAnim->reset();
					moveStartTimer->reset();
					isMoving = false;
				}
			}
		}
	}
		


	void chargeLoop(float* deltaT, shared_ptr<player> p) {
		if (!ignitionTimer->isFinished()) {
			ignitionTimer->run(deltaT);
			flameAnim->reset();
		}
		else {
			flameTimer->run(deltaT);
			//flameAnim->thisFrame();
			flaming = true;
			chargeTime_left -= *deltaT;
			if (chargeTime_left <= 0) {
				chargeTime_left = chargeTime;
				state = moving;
				flaming = false;
				ignitionTimer -> reset();
				ignitionAnim->reset();
				
				moveTarget = p->getSprite()->getMiddlePos().x;
			}
		}
	}

	void shoot(shared_ptr<player> p, list<shared_ptr<EnemyBullet>>* bList) {
		vector<float> angles = { 30, 45, 60 };
		Vector2f pos;
		if (!faceRight) {
			pos = Vector2f(sprite->getPosition().x, sprite->getMiddlePos().y);
		}
		else {
			pos = Vector2f(sprite->getPosition().x + sprite->getSize().x, sprite->getMiddlePos().y);
		}

		float speed = speedCalc(p, angles[1]);
		if (!faceRight) {
			//angle = (angle + 180) % 360;
			speed = -speed;
		}
		for (int i = 0; i < 3; i++) {
			int angle = angles[i];
			
			for (int j = 0; j < 5; j++) {
				shared_ptr<FireBall> newBall = shared_ptr<FireBall>(new FireBall(angle, speed, phys->getTexture(), pos, float(j * 20)));
				bList->push_back(newBall);
			}
		}
	}

	float speedCalc(shared_ptr<player> p, float angle) {
		
		float dist = (fabs(p->getPosition().x - sprite->getPosition().x)) / cos(double(angle));
		float time = 1.1;

		return dist / time;
	}

	int bubbleDam() {
		return 5;
	}

	void setFacing(bool right) {
		if (right != faceRight) {
			moveAnim->swapAll();
			ignitionAnim->swapAll();
			moveStartAnim->swapAll();
			flameAnim->swapAll();
			throwAnim->swapAll();
			idleAnim->swapAll();
			if (right) {
				throwAnim->setOffsetList(list<Vector2f>{Vector2f(2 * 4, 0 * 4), Vector2f(-5 * 4, -3 * 4)});
				ignitionAnim->setOffsetList(list<Vector2f>{Vector2f(-2 * 4, 5 * 4), Vector2f(2 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4)});
				flameAnim->setOffsetList(list<Vector2f>{Vector2f(-2 * 4, -6 * 4), Vector2f(-2 * 4, -7 * 4)});
			}
			else {
				ignitionAnim->setOffsetList(list<Vector2f>{Vector2f(-1 * 4, 5 * 4), Vector2f(3 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4)});
				flameAnim->setOffsetList(list<Vector2f>{Vector2f(-1 * 4, -6 * 4), Vector2f(0 * 4, -7 * 4)});
				throwAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(-4 * 4, -3 * 4)});
			}
		}

		faceRight = right;
	}
};