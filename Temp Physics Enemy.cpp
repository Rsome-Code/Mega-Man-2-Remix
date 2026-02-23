#include "temp enemy.cpp"
#pragma once

class TempPhysicsEnemy :public TempEnemy {
	using TempEnemy::TempEnemy;
public:
	shared_ptr<physicsObject> phys;
	bool grounded = true;

public:

	virtual ~TempPhysicsEnemy() {

	}

	TempPhysicsEnemy(shared_ptr<Texture> t, Vector2f i) {
		phys = shared_ptr<physicsObject> (new physicsObject("physEnemy", t, IntRect(0, 0, 0, 0), i, Vector2f(4, 4)));
		sprite = phys;

		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite));

		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite));


		deathTimer = shared_ptr<animTimer> (new animTimer(deathAnim, 16, false));
		initialPos = i;
		//act = false;
		//display = false;
		//initial();

	}

	bool yDeath(list<shared_ptr<enemy>>* enemies) {
		return isDead(enemies);
	}

	void tileCollision(list<shared_ptr<tile>>* tileList) {
		for (shared_ptr<tile> t : *tileList) {
			if (t->getGround() != NULL && phys->getVVelocity() < 0) {
				groundCheck(t);
			}

			if (t->getCeiling() != NULL) {
				checkCeiling(t);
			}

			if (t->getLeft() != NULL) {
				checkLeft(t);
			}
			if (t->getRight() != NULL) {
				checkRight(t);
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

	void groundCheck(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			grounded = true;
			sprite->setPosition(Vector2f(sprite->getPosition().x, t->getGround()->getPosition().y - hit->getSize().y));
			phys->setVVelocity(0);
		}
	}

	void checkCeiling(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getCeiling(), hit)) {
			phys->setVVelocity(0);
			sprite->setPosition(Vector2f(sprite->getPosition().x, t->getCeiling()->getPosition().y + t->getCeiling()->getSize().y));
		}
	}

};