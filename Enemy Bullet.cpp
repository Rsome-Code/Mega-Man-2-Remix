#include "object.cpp"
#include "movable object.cpp"
#include "camera.cpp"
#include "hitbox detector.cpp"
#include "tile.cpp"
#include "player.cpp"
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

	virtual void eachFrame(float* deltaT, list<tile*>* tileList) {};

	virtual void eachFrame(float* deltaT, list<tile*>* tileList, player* p) {};

	int getDamage() {
		return damage;
	}

	objectHitbox* getHitbox() {
		return hit;
	}

	CollisionType getCollType() {
		return collType;
	}
	bool checkHit(objectHitbox* pHit) {
		return hitboxDetect::hitboxDetection(pHit, hit);
	}
};