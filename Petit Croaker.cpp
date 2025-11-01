#include "temp physics enemy.cpp"
#pragma once

class PCroaker : public TempPhysicsEnemy {
	using TempPhysicsEnemy::TempPhysicsEnemy;
	animation* jumpAnim;
	animTimer* jumpTimer;

	float idleTime = 1;
	float idleTime_left = 0;

	float jumpForce = 1000;
	float defaultSpeed = 400;
	float HSpeed;
	float angle;
	float left = 180;
	float right = 0;

	enum State {
		idle, jumping
	};
	State state = idle;

public:

	void initial() {
		phys->setRect(IntRect(166, 362, 8, 8));
		jumpAnim = new animation(list<IntRect>{IntRect(166, 362, 8, 8), IntRect(175, 355, 11, 12)}, sprite);
		jumpTimer = new animTimer(jumpAnim, 60, false);

		phys->enableGravity(true);
		offSetList();
		sprite->setScale(Vector2f(4, 4));
		hit = new objectHitbox(IntRect(0, 0, 8,8), sprite);
		hurt = hit;

		hp = 1;
		damage = 2;
		grounded = false;
		setCode("petit croaker");
	}

	void setNextSpeed(float sp) {
		HSpeed = sp;
	}

	float getDSpeed() {
		return defaultSpeed;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {

		if (state == idle) {
			idleTime_left -= *deltaT;
			if (idleTime_left <= 0) {
				idleTime_left = idleTime;
				jump(p);
				state = jumping;
			}
		}

		else if (state == jumping) {
			jumpTimer->run(deltaT);
			phys->eachFrame(deltaT);
			phys->move(angle, deltaT, HSpeed);
			tileCollision(tileList);
			if (grounded) {
				state = idle;
				HSpeed = defaultSpeed;

				jumpAnim->reset();
				jumpAnim->thisFrame();
			}
		}

	}

	void jump(player* p) {
		checkDirection(p->getSprite());

		if (faceRight) {
			angle = right;
		}
		else {
			angle = left;
		}

		phys->setVVelocity(jumpForce);
		grounded = false;
	}

	void setFacing(bool right) {
		if (right != faceRight) {
			jumpAnim->swapAll();
		}

		faceRight = right;
	}

};