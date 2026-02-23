#include "temp physics enemy.cpp"
#pragma once

class Chicken :public TempPhysicsEnemy {
	using TempPhysicsEnemy::TempPhysicsEnemy;

	shared_ptr<animation> runAnim;
	shared_ptr<animTimer> runTimer;
	float runSpeed = 500;

	float jumpTime = 1.5;
	float jumpTime_left = jumpTime;
	float jumpForce = 1200;

public:

	virtual ~Chicken() {

	}

	void initial(Vector2f pos) {
		initialPos = pos;
		sprite->setPosition(pos);
		phys->setRect(IntRect(245, 311, 34, 37));
		display = true;
		act = true;

		grounded = false;
		phys->enableGravity(true);
		runAnim = shared_ptr<animation>(new animation(list<IntRect> {IntRect(245, 311, 34, 37), IntRect(289, 308, 27, 40), IntRect(350, 311, 40, 37), IntRect(289, 308, 27, 40)}, sprite));
		runAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(3 * 4, 0 * 4), Vector2f(-2 * 4, 0 * 4), Vector2f(3 * 4, 0 * 4) });
		runTimer = shared_ptr<animTimer> (new animTimer(runAnim, 10, true));

		hp = 5;
		damage = 4;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 34, 37), sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 34, 37), sprite));

		setCode("chicken");
	}

	void initial() {
		setCode("chicken");
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
		move(deltaT);

		if (phys->getAcceleration().y < 0) {
			floorCheck(tileList);
		}

		runTimer->run(deltaT);

		checkJump(deltaT);
		
	}

	void checkJump(float* deltaT) {
		jumpTime_left -= *deltaT;
		if (jumpTime_left <= 0) {
			jumpTime_left = jumpTime;
			jump();
		}
	}

	void jump() {
		phys->setVVelocity(jumpForce);
	}

	

	void floorCheck(list<shared_ptr<tile>>* tileList) {
		for (shared_ptr<tile> t : *tileList) {
			if (t->getGround() != NULL) {
				groundCheck(t);
			}
		}
	}
	void move(float* deltaT) {
		phys->move(180, deltaT, runSpeed);

		phys->eachFrame(deltaT);
		
	}
};