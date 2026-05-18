#include "temp enemy.cpp"
#pragma once

class Gremlin : public TempEnemy {
	using TempEnemy::TempEnemy;

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	int moveSpeed = 100;
	int upSpeed = 600;

	float startTimer = 1;
	float startTimer_left = startTimer;	

	float upTimer = 0.4;
	float upTimer_left = upTimer;

public:

	void initial() {

		mov->setPosition(initialPos);

		mov->setRect(IntRect(439, 389, 15, 15));
		mov->setScale(Vector2f(4, 4));

		hp = 1;
		damage = 2;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 15), mov));
		hurt = hit;

		setCode();

		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(439, 389, 16, 15), IntRect(456, 389, 16, 16), IntRect(473, 389, 16, 16)}, sprite));
		timer = shared_ptr<animTimer>(new animTimer(anim, 4, true));

		offSetList();
	}

	void setCode() {
		code = "gremlin";
	}

	void setRight(bool b) {
		startRight = b;
	}

	bool startRight = false;

	enum State {
		start, moveUp, follow
	};
	State state = start;
	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {
		timer->run(deltaT);

		if (state == start) {

			if (startRight) {
				rightLoop(deltaT);
			}
			else {
				leftLoop(deltaT);
			}

			startTimer_left -= *deltaT;
			if (startTimer_left <= 0) {
				state = moveUp;
			}
		}

		else if (state == moveUp) {

			mov->move(Angle::up, deltaT, upSpeed);

			upTimer_left -= *deltaT;
			if (upTimer_left <= 0) {
				state = follow;
			}
			
		}

		else if (state == follow) {
			float angle = Maths::getAngle(mov->getPosition(), p->getPosition());
			mov->move(angle, deltaT, moveSpeed);
		}

	}

	void rightLoop(float* deltaT) {
		mov->move(Angle::right, deltaT, moveSpeed);
	}
	void leftLoop(float* deltaT) {
		mov->move(Angle::left, deltaT, moveSpeed);
	}
};