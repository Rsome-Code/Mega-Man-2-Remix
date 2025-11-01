#include "physics object.cpp"
#include "object hitbox.cpp"
#pragma once

class ItemBullet {
protected:
	physicsObject* phys;

	objectHitbox* hitbox;
	string code;

	


public:

	virtual bool eachFrame(float* deltaT) {
		return false;
	}

	objectHitbox* getHit() {
		return hitbox;
	}

	physicsObject* getSprite() {
		return phys;
	}

	string getCode() {
		return code;
	}
};