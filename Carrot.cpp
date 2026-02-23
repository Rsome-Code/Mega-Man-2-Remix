#include "movable object.cpp"
#include "enemy bullet.cpp"
#include "hitbox detector.cpp"

#pragma once

class Carrot : public EnemyBullet {

public:
	Carrot(shared_ptr<Texture> t, Vector2f position, float angle) {
		damage = 4;
		mov = shared_ptr<movable>(new movable("eBullet", t, IntRect(109, 574, 12, 13), position, Vector2f(4, 4)));
		sprite = mov;
		this->angle = angle;
		speed = 800;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 12, 13), mov));
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		mov->move(angle, deltaT, speed);
		hit->updatePos();
	}


	int getDamage() {
		return damage;
	}


};