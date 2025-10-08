#include "movable object.cpp"
#include "enemy bullet.cpp"
#include "hitbox detector.cpp"

#pragma once

class Carrot : public EnemyBullet {

public:
	Carrot(Texture* t, Vector2f position, float angle) {
		damage = 4;
		mov = new movable("eBullet", t, IntRect(109, 574, 12, 13), position, Vector2f(4, 4));
		sprite = mov;
		this->angle = angle;
		speed = 800;
		hit = new objectHitbox(IntRect(0, 0, 12, 13), mov);
	}

	void eachFrame(float* deltaT) {
		mov->move(angle, deltaT, speed);
		hit->updatePos();
	}
	bool checkHit(objectHitbox* pHit) {
		return hitboxDetect::hitboxDetection(pHit, hit);
	}

	int getDamage() {
		return damage;
	}


};