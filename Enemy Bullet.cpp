#include "object.cpp"
#include "movable object.cpp"
#include "camera.cpp"
#pragma once

class EnemyBullet : public object {
public:
	enum CollisionType {
		NONE, DESTROY, DESTRUCT
	};

protected:
	objectHitbox* hit;
	movable* mov;
	int damage = 1;
	float angle;
	float speed = 0;
	CollisionType collType = NONE;

public:
	EnemyBullet() {
		setCode("eBullet");
	}

	virtual void eachFrame(float* deltaT) = 0;

	int getDamage() {
		return damage;
	}

	objectHitbox* getHitbox() {
		return hit;
	}

	CollisionType getCollType() {
		return collType;
	}

};