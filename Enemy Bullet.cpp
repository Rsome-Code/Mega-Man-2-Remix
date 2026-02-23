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
	shared_ptr<objectHitbox> hit;
	shared_ptr<movable> mov;
	int damage = 1;
	float angle;
	float speed = 0;
	CollisionType collType = NONE;

public:

	virtual ~EnemyBullet() {

	}

	EnemyBullet() {
		setCode("eBullet");
	}

	virtual void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {};

	virtual void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList, shared_ptr<player> p) {};

	int getDamage() {
		return damage;
	}

	shared_ptr<objectHitbox> getHitbox() {
		return hit;
	}

	CollisionType getCollType() {
		return collType;
	}
	bool checkHit(shared_ptr<objectHitbox> pHit) {
		return hitboxDetect::hitboxDetection(pHit, hit);
	}
};