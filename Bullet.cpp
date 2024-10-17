#include "movable object.cpp"
#include "object hitbox.cpp"
#include "enemy.cpp"
#pragma once

class bullet {
protected:
	movable* sprite;
	bool shooting = false;

public:
	bullet() {}

	virtual void eachFrame(float* deltaT) = 0;
	virtual void start(bool r) = 0;

	bool getShooting() {
		return shooting;
	}

	objectSprite* getSprite() {
		return sprite;
	}


	virtual objectHitbox* getHitbox() = 0;
	virtual void onHit() {};
	virtual int checkDamage(enemy* en) = 0;
};