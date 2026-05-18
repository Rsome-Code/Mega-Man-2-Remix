#include "enemy.cpp"
#pragma once

class HeadSpike : public enemy {
	using enemy::enemy;
	
public:
	void initial() {
		sprite->setRect(IntRect(423, 390, 14, 15));
		sprite->setScale(Vector2f(4, 4));
		sprite->setPosition(initialPos);
		hp = 10000000;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 14, 15), sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(-999999999, -99999999, 0,0), sprite));
		damage = 4;
		setCode("head spike");


	}

	enum State {
		up, down, stopUp, stopDown
	};

	State state = stopDown;

	float stopTime = 0.5;
	float stopTime_left = stopTime;

	

	int upSpeed = 80;
	int downSpeed = 200;

	virtual bool eachFrame(float* deltaT, shared_ptr<player> p, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* enemyList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {
		flashTime_left -= *deltaT;
		if (hp > 0) {
			if (damaged) {
				display = false;

				if (flashTime_left <= 0) {

					damaged = false;
					display = true;
				}
			}

			if (!frozen) {
				hit->updatePos();
				hurt->updatePos();
				alive(p, deltaT, tileList, enemyList, bList);
				//alive(p, deltaT, tileList, enemyList, bList, soundCol);
			}
		}
		else {
			uniqueDeathLoop(deltaT);

			return death(deltaT, enemyList);
		}
		return false;
	}
	

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		if (state == stopDown) {
			stopTime_left -= *deltaT;

			if (stopTime_left <= 0) {
				stopTime_left = stopTime;
				state = up;
			}
		}

		if (state == up) {
			mov->move(Angle::up, deltaT, upSpeed);

			if (mov->getPosition().y <= initialPos.y - (15 * 4 )) {
				mov->setPosition(Vector2f(mov->getPosition().x, initialPos.y - (15 * 4)));
				state = stopUp;
			}
		}

		if (state == stopUp) {
			stopTime_left -= *deltaT;

			if (stopTime_left <= 0) {
				stopTime_left = stopTime;
				state = down;
			}
		}

		if (state == down) {
			mov->move(Angle::down, deltaT, downSpeed);
			if (mov->getPosition().y >= initialPos.y) {
				mov->setPosition(Vector2f(mov->getPosition().x, initialPos.y));
				state = stopDown;
			}
		}

	}

};