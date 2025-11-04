#include "enemy bullet.cpp"
#include "hitbox detector.cpp"
#pragma once

class SniperBullet : public EnemyBullet {
	using EnemyBullet::EnemyBullet;

public:
	SniperBullet(Texture* t, Vector2f position, float angle) {
		damage = 5;
		spriteSetup(t, position);
		sprite = mov;
		hit = new objectHitbox(IntRect(0, 0, 8, 8), mov);

		this->angle = angle;
		speed = 600;
	}

	virtual void spriteSetup(Texture* t, Vector2f position) {
		mov = new movable(t, IntRect(673, 443, 8, 8), position, Vector2f(4, 4));
		
	}

	void eachFrame(float* deltaT, list<tile*>* tileList) {
		mov->move(angle, deltaT, speed);
		hit->updatePos();

		animate(deltaT);
	}

	virtual void animate(float* deltaT) {

	}

};