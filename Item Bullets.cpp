#include "physics object.cpp"
#include "object hitbox.cpp"
#pragma once

class ItemBullet {
protected:
	physicsObject* phys;

	objectHitbox* hitbox;
	string code;

	bool display = true;


public:

	virtual bool eachFrame(float* deltaT) {
		return false;
	}

	objectHitbox* getHit() {
		return hitbox;
	}

	physicsObject* getSprite() {
		if (display) {
			return phys;
		}
		return NULL;
	}

	string getCode() {
		return code;
	}
};