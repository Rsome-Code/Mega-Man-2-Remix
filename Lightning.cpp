#include "enemy bullet.cpp"
#pragma once

class Lightning : public EnemyBullet {

public:

	Lightning(shared_ptr<Texture> tex, Vector2f pos, float angle) {

		mov = shared_ptr<movable>(new movable(tex, IntRect(215, 272, 16, 11), pos, Vector2f(4,4)));
		sprite = mov;
		damage = 3;

		this->angle = angle;

		speed = 500;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 11), mov));
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {

		mov->move(angle, deltaT, speed);
		hit->updatePos();

	}

};