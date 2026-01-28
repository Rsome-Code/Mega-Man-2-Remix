#include "enemy.cpp"
#include "physics object.cpp"
#include "hitbox detector.cpp"

#pragma once

class PhysicsEnemy : public enemy {
protected:
	physicsObject* phys;
	bool grounded = true;

public:



	PhysicsEnemy(Texture* t, Vector2f i) {
		phys = new physicsObject("physEnemy", t, IntRect(0,0,0,0), i, Vector2f(4,4));
		sprite = phys;
		
		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite);

		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite);


		deathTimer = new animTimer(deathAnim, 16, false);
		initialPos = i;
		act = false;
		display = false;
		initial();


		setCode();
	}

	PhysicsEnemy(Vector2f i) {
		phys = new physicsObject();
		phys->setPosition(i);
		phys->setScale(Vector2f(4, 4));
		sprite = phys;

		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite);

		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite);


		deathTimer = new animTimer(deathAnim, 16, false);
		initialPos = i;
		act = false;
		display = false;
		initial();

		setCode();

	}

	void tileCollision(list<tile*>* tileList) {
		//grounded = false;
		bool groundCon;
		bool alreadyGround = grounded;
		if (grounded) {
			groundCon = false;
		}
		for (tile* t : *tileList) {
			bool thisSide = false;
			bool thisGround = false;
			if (t->getGround() != NULL && phys->getVVelocity() < 0 && !grounded) {
				thisGround = groundCheck(t);
			}
			else if (alreadyGround) {
				if (t->getGround() != NULL){
					//thisGround = groundConfirm(t);
					if (groundConfirm(t)) {
						groundCon = true;
					}
				}
			}
			
			if (!thisGround) {


				if (t->getRight() != NULL) {
					if (checkRight(t)) {
						thisSide = true;
					}

				}
				if (t->getLeft() != NULL) {
					if (checkLeft(t)) {
						thisSide = true;
					}
				}
				if (t->getCeiling() != NULL && !thisSide) {
					checkCeiling(t);
				}
			}
			
			
		}

		if (alreadyGround) {
			if (!groundCon) {
				grounded = false;
			}
		}
	}

	virtual bool checkRight(tile* t) {
		if (hitboxDetect::hitboxDetection(t->getRight(), hit)) {
			sprite->setPosition(Vector2f(t->getRight()->getPosition().x + t->getRight()->getSize().x + 1, sprite->getPosition().y));
			hit->updatePos();
			if (phys->getHVelocity() > 0) {
				phys->setHVelocity(0);
				
			}
			hitRight();
			hitRight(t);

			return true;
		}
		return false;
	}

	virtual bool checkLeft(tile* t) {
		if (hitboxDetect::hitboxDetection(t->getLeft(), hit)) {
			sprite->setPosition(Vector2f(t->getLeft()->getPosition().x - hit->getSize().x - 1, sprite->getPosition().y));
			hit->updatePos();
			if (phys->getHVelocity() < 0) {
				phys->setHVelocity(0);

			}
			hitLeft();
			hitLeft(t);
			return true;
		}
		return false;
	}


	virtual void hitRight(){}
	virtual void hitRight(tile* t) {}
	virtual void hitLeft(){}
	virtual void hitLeft(tile* t) {}


	bool groundConfirm(tile* t) {
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			//grounded = true;
			return true;
		}
		
	}

	bool groundCheck(tile* t) {
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			grounded = true;
			sprite->setPosition(Vector2f(sprite->getPosition().x, t->getGround()->getPosition().y - hit->getSize().y));
			hit->updatePos();
			phys->setVVelocity(0);
			return true;
		}
		return false;
	}

	void checkCeiling(tile* t) {
		if (hitboxDetect::hitboxDetection(t->getCeiling(), hit)) {
			phys->setVVelocity(0);
			sprite->setPosition(Vector2f(sprite->getPosition().x, t->getCeiling()->getPosition().y + t->getCeiling()->getSize().y +10));
			hit->updatePos();
		}
	}

};