#include "enemy bullet.cpp"
#pragma once

class BossBoomerang: public EnemyBullet {

	float flyTime = 0.5;
	float flyTime_left = flyTime;

	float haltTime = 0.4;
	float haltTime_left = haltTime;

	bool move = true;

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

public:
	BossBoomerang(shared_ptr<Texture> t, Vector2f startPos, float angle) {
		mov = shared_ptr<movable>(new movable(t, IntRect(0, 146, 8, 6), startPos, Vector2f(4, 4)));
		sprite = mov;
		this->angle = angle;
		speed = 600;

		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(0, 146, 8, 6), IntRect(10, 145, 6, 8)}, mov));
		timer = shared_ptr<animTimer> (new animTimer(anim, 8, true));

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 8), mov));

		damage = 3;
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList, shared_ptr<player> p) {
		timer->run(deltaT);

		if (move) {
			mov->move(angle, deltaT, speed);
			flyTime_left -= *deltaT;
			if (flyTime_left <= 0) {
				move = false;
			}
		}

		else {
			haltTime_left -= *deltaT;
			if (haltTime_left <= 0) {
				move = true;
				angle = Maths::getAngle(mov->getMiddlePos(), p->getSprite()->getMiddlePos());
				flyTime_left = 10;
			}
		}

		hit->updatePos();
	}
}; 