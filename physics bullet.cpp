#include "enemy bullet.cpp"
#include "physics object.cpp"
#pragma once

class PhysicsBullet : public EnemyBullet {

protected:
	shared_ptr<physicsObject> phys;

	bool grounded = false;

	virtual ~PhysicsBullet() {

	}

	void tileCollision(list<shared_ptr<tile>>* tileList) {

		for (shared_ptr<tile> t : *tileList) {
			bool thisGround = false;
			if (t->getGround() != NULL && phys->getVVelocity() < 0) {
				thisGround = groundCheck(t);
			}
			if (t->getCeiling() != NULL) {
				checkCeiling(t);
			}
			if (!thisGround) {

				if (t->getLeft() != NULL) {
					checkLeft(t);
				}
				if (t->getRight() != NULL) {
					checkRight(t);
				}
			}
		}
	}
	void checkRight(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getRight(), hit)) {
			sprite->setPosition(Vector2f(t->getRight()->getPosition().x + t->getRight()->getSize().x, sprite->getPosition().y));
			if (phys->getHVelocity() < 0) {
				phys->setHVelocity(0);
			}
		}
	}

	void checkLeft(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getLeft(), hit)) {
			sprite->setPosition(Vector2f(t->getLeft()->getPosition().x - hit->getSize().x, sprite->getPosition().y));
			if (phys->getHVelocity() < 0) {
				phys->setHVelocity(0);
			}
		}
	}

	bool groundCheck(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			grounded = true;
			sprite->setPosition(Vector2f(sprite->getPosition().x, t->getGround()->getPosition().y - hit->getSize().y));
			phys->setVVelocity(0);
			return true;
		}
		return false;
	}

	void checkCeiling(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getCeiling(), hit)) {
			phys->setVVelocity(0);
			sprite->setPosition(Vector2f(sprite->getPosition().x, t->getCeiling()->getPosition().y + t->getCeiling()->getSize().y + 10));
		}
	}
};