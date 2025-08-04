#include "object.cpp"
#include "movable object.cpp"
#include "camera.cpp"
#pragma once

class EnemyBullet : public object {
protected:
	objectHitbox* hit;
	movable* mov;
	int damage = 1;
	float angle;
	float speed = 0;

public:
	EnemyBullet() {
		setCode("eBullet");
	}

	virtual void eachFrame(float* deltaT) = 0;

	int getDamage() {
		return damage;
	}

	bool checkOffScreen(list<EnemyBullet*>* objectList) {
		if (sprite->getCameraPosition().x > 1920 || (sprite->getCameraPosition().x - sprite->getSize().x)< 0) {
			return true;
		}
		return false;
	}

};