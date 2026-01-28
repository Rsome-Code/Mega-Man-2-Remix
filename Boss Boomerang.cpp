#include "enemy bullet.cpp"
#pragma once

class BossBoomerang: public EnemyBullet {

	float flyTime = 0.5;
	float flyTime_left = flyTime;

	float haltTime = 0.4;
	float haltTime_left = haltTime;

	bool move = true;

	animation* anim;
	animTimer* timer;

public:
	BossBoomerang(Texture* t, Vector2f startPos, float angle) {
		mov = new movable(t, IntRect(0, 146, 8, 6), startPos, Vector2f(4, 4));
		sprite = mov;
		this->angle = angle;
		speed = 600;

		anim = new animation(list<IntRect>{IntRect(0, 146, 8, 6), IntRect(10, 145, 6, 8)}, mov);
		timer = new animTimer(anim, 8, true);

		hit = new objectHitbox(IntRect(0, 0, 8, 8), mov);

		damage = 3;
	}

	void eachFrame(float* deltaT, list<tile*>* tileList, player* p) {
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