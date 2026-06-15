#include "enemy.cpp"
#include "physics object.cpp"
#include "hitbox detector.cpp"

#pragma once

class PhysicsEnemy : public enemy {
	using enemy::enemy;
protected:
	shared_ptr<physicsObject> phys;
	bool grounded = true;

	//shared_ptr<objectHitbox> ground;

public:



	virtual ~PhysicsEnemy() {
		//delete phys;
	}

	PhysicsEnemy(shared_ptr<Texture> t, Vector2f i) {
		phys = shared_ptr<physicsObject> (new physicsObject("physEnemy", t, IntRect(0,0,0,0), i, Vector2f(4,4)));
		sprite = phys;
		
		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite));

		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite));


		deathTimer = shared_ptr<animTimer> (new animTimer(deathAnim, 16, false));
		initialPos = i;
		act = false;
		display = false;
		initial();

	//	makeGroundBox();
		setCode();
	}

	PhysicsEnemy(Vector2f i) {
		phys = shared_ptr<physicsObject> (new physicsObject());
		phys->setPosition(i);
		phys->setScale(Vector2f(4, 4));
		sprite = phys;
		
		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite));

		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite));


		deathTimer = shared_ptr<animTimer> (new animTimer(deathAnim, 16, false));
		initialPos = i;
		act = false;
		display = false;
		initial();

		//makeGroundBox();
		setCode();

	}

	//void makeGroundBox() {
		//if (hit != NULL) {
		//	ground = shared_ptr<objectHitbox>(new objectHitbox(IntRect(hit->getRelativeRect().getPosition().x, hit->getRelativeRect().height, hit->getRelativeRect().width, 2), sprite));
		//}
	//}



	//Why is this here???
	void tileCollision(list<shared_ptr<tile>>* tileList, float* deltaT) {
		//grounded = false;
		bool groundCon;
		bool alreadyGround = grounded;
		if (grounded) {
			groundCon = false;
		}
		for (shared_ptr<tile> t : *tileList) {
			bool thisSide = false;
			bool thisGround = false;
			if (t->getGround() != NULL && phys->getVVelocity() < 0 && !grounded) {
				thisGround = groundCheck(t);
			}
			else if (alreadyGround) {
				if (t->getGround() != NULL){
					//thisGround = groundConfirm(t);
					if (groundConfirm(t)) {
						thisGround = true;
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
			else {
								
				//phys->setVVelocity(0);
				//phys->addVerticalForce(-phys->getGravity(), deltaT);
			}
		}
		else if (grounded) {
			//phys->setVVelocity(0);
			//phys->addVerticalForce(-phys->getGravity(), deltaT);
		}
	}
	void tileCollision(list<shared_ptr<tile>>* tileList) {
		//grounded = false;
		bool groundCon;
		bool alreadyGround = grounded;
		if (grounded) {
			groundCon = false;
		}
		for (shared_ptr<tile> t : *tileList) {
			bool thisSide = false;
			bool thisGround = false;
			if (t->getGround() != NULL && phys->getVVelocity() < 0 && !grounded) {
				thisGround = groundCheck(t);
			}
			else if (alreadyGround) {
				if (t->getGround() != NULL) {
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

	virtual bool checkRight(shared_ptr<tile> t) {
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

	virtual bool checkLeft(shared_ptr<tile> t) {
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
	virtual void hitRight(shared_ptr<tile> t) {}
	virtual void hitLeft(){}
	virtual void hitLeft(shared_ptr<tile> t) {}


	bool groundConfirm(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getGround(), shared_ptr<objectHitbox>(new objectHitbox(IntRect(hit->getRelativePosition().x, hit->getRelativePosition().y + hit->getRelativeRect().height * 4, hit->getRelativeRect().width, 2), sprite)))) {
		//if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			//grounded = true;
			return true;
		}
		return false;
		
	}


	bool groundCheck(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			grounded = true;
			sprite->setPosition(Vector2f(sprite->getPosition().x, t->getGround()->getPosition().y - hit->getSize().y));
			hit->updatePos();
			phys->setVVelocity(0);
			return true;
		}
		return false;
	}

	void checkCeiling(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getCeiling(), hit)) {
			phys->setVVelocity(0);
			sprite->setPosition(Vector2f(sprite->getPosition().x, t->getCeiling()->getPosition().y + t->getCeiling()->getSize().y +10));
			hit->updatePos();
		}
	}


	//shared_ptr<objectHitbox> getGround() {
	//	return ground;
	//}
};