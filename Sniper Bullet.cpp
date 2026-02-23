#include "enemy bullet.cpp"
#include "hitbox detector.cpp"
#pragma once

class SniperBullet : public EnemyBullet {
	using EnemyBullet::EnemyBullet;


public:
	SniperBullet(shared_ptr<Texture> t, Vector2f position, float angle) {
		damage = 4;
		spriteSetup(t, position);
		sprite = mov;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 8), mov));

		this->angle = angle;
		speed = 600;
	}

	virtual void spriteSetup(shared_ptr<Texture> t, Vector2f position) {
		mov = shared_ptr<movable>(new movable(t, IntRect(673, 443, 8, 8), position, Vector2f(4, 4)));
		
	}

	virtual void spriteSetup(shared_ptr<Texture> t, Vector2f position, IntRect rectP) {
		mov = shared_ptr<movable>(new movable(t, rectP, position, Vector2f(4, 4)));
		sprite = mov;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 8), mov));
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		mov->move(angle, deltaT, speed);
		hit->updatePos();

		animate(deltaT);
	}

	virtual void animate(float* deltaT) {

	}

};