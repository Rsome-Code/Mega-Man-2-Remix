#include "physics object.cpp"
#include "object hitbox.cpp"
#pragma once

class ItemBullet {
protected:
	shared_ptr<physicsObject> phys;

	shared_ptr<objectHitbox> hitbox;
	string code;

	bool display = true;


public:

	virtual bool eachFrame(float* deltaT) {
		return false;
	}

	shared_ptr<objectHitbox> getHit() {
		return hitbox;
	}

	shared_ptr<physicsObject> getSprite() {
		if (display) {
			return phys;
		}
		return NULL;
	}

	string getCode() {
		return code;
	}
};