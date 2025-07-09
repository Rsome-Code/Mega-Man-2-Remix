#include "movable object.cpp"
#include "object hitbox.cpp"
#include "enemy.cpp"
#pragma once

class bullet {
protected:
	movable* sprite;
	bool shooting = false;
	float shootTime;
	float shootTemp;

	objectSprite* origin;
	objectHitbox* hitbox;
	float speed = 1750;
	bool right;

	float direction;

public:
	bullet() {}

	virtual bool eachFrame(float* deltaT) = 0;
	virtual void start(bool r) = 0;
	virtual void secondFire() {};
	virtual bool release(bool r) { return false; };

	bool getShooting() {
		return shooting;
	}

	objectSprite* getSprite() {
		return sprite;
	}


	virtual objectHitbox* getHitbox() { return hitbox; };
	virtual void onHit(object* e) {
		if (e->getHP() > 0) {
			shootReset();
		}
	};
	virtual int checkDamage(object* en) { return NULL; };
	virtual void hold(float* deltaT) {};
	virtual float getHoldTime() { return NULL; };
	virtual float getMaxHoldTime() { return NULL; };
	
	virtual void deflect() {
		
		if (right) {
			direction = 330;
		}
		else {
			direction = 210;
		}
	}

	void setDirection(float num) {
		direction = num;
	}

	void shootReset() {
		sprite->setPosition(Vector2f(0, 0));
		hitbox->updatePos();
		shooting = false;
		uniqueReset();
	}

	virtual void uniqueReset() {};
	
};