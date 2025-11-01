#include "temp physics enemy.cpp"
#pragma once

class FlyGuy : public TempPhysicsEnemy {
	using TempPhysicsEnemy::TempPhysicsEnemy;

	/*float targetX;
	float distX;
	float currentAngle = 270;
	float horiSpeed;
	float horiSpeedMult = 2;
	float vertSpeed;
	float maxVertSpeed = 500;
	float arcVertDist = 100;
	float arcVertTarget;
	*/
	float superSpeed = 5000;
	float superTime = 0.5;
	float superTime_left = superTime;

	float upSpeed = 500;
	float upDistance = 400;
	float upStartPos;

	float chargeTime = 2;
	float chargeTime_left = chargeTime;

	bool first = true;
	
	enum State {
		falling, charging, flyUp, flyArc
	};
	State state = falling;

	animation* chargeAnim;
	animTimer* chargeTimer;

	animation* flyAnim;
	animTimer* flyTimer;

	float fallSpeed = 500;
	float fallDistance = 200;

public:

	void initial() {
		setCode("fly guy");
		state = falling;
		phys->setRect(IntRect(137, 214, 28, 38));
		phys->enableGravity(true);

		hit = new objectHitbox(IntRect(0, 0, 27, 37), phys);
		hurt = new objectHitbox(IntRect(0, 0, 27, 37), phys);
		hp = 4;
		damage = 3;
		grounded = false;

		flyAnim = new animation(list<IntRect>{IntRect(197, 214, 32, 38), IntRect(235, 214, 32, 38)}, sprite);
		flyTimer = new animTimer(flyAnim, 12, true);

		chargeAnim = new animation(list<IntRect>{IntRect(137, 214, 28, 38), IntRect(167, 214, 28, 38), IntRect(197, 214, 32, 38), IntRect(235, 214, 32, 38)}, sprite);
		chargeTimer = new animTimer(chargeAnim, 12, true);
		

		offSetList();
		deathAnim->setSprite(sprite);
	}

	void setSound(SoundCollection* soundCol) {
		sound = soundCol->getLand();
	}
	

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		checkDirection(p->getSprite());
		//tileCollision(tileList);
		
		if (state == falling) {

			for (tile* t : *tileList) {
				if (t->getGround() != NULL) {
					groundCheck(t);
				}
			}

			if (phys->getCameraPosition().y < -phys->getSize().y - (fallDistance)) {
				phys->setPosition(Vector2f(p->getSprite()->getMiddlePos().x - (phys->getSize().x / 2), phys->getPosition().y));
			}
			if (!first) {
				phys->move(90, deltaT, fallSpeed);
			}

			phys->eachFrame(deltaT);

			if (grounded) {
				sound->play();
				first = false;
				upStartPos = sprite->getPosition().y;
				state = charging;
				//phys->enableGravity(false);
			}
		}
		else if (state == charging) {
			for (tile* t : *tileList) {
				if (t->getGround() != NULL) {
					groundCheck(t);
				}
			}
			chargeTimer->run(deltaT);
			chargeTime_left -= *deltaT;
			if (chargeTime_left <= 0) {
				chargeTime_left = chargeTime;
				state = flyUp;
				grounded = false;
			}
		}
		else if (state == flyUp) {
			flyTimer->run(deltaT);
			phys->move(270, deltaT, upSpeed);
			

			if (phys->getPosition().y <= upStartPos - upDistance) {
				state = flyArc;

				phys->setVVelocity(1000);
			}
		}
		else if (state == flyArc) {
			phys->move(270, deltaT, superSpeed);
			superTime_left -= *deltaT;
			if (superTime_left<=0) {
				
				superTime_left = superTime;
				phys->setPosition(Vector2f(p->getSprite()->getMiddlePos().x - (phys->getSize().x/2), phys->getPosition().y));
				state = falling;
			}

		}
		/*else if (state == flyArc) {
			arcSim(deltaT);

			if (right) {
				phys->move(0, deltaT, horiSpeed);
				if (phys->getPosition().x > targetX) {
					state = falling;
				}
			}
			else{
				phys->move(180, deltaT, horiSpeed);
				if (phys->getPosition().x < targetX) {
					state = falling;
				}
			}
		}*/

	}

	void setFacing(bool right) {

		if (right != faceRight) {
			flyAnim->swapAll();
			chargeAnim->swapAll();

			if (right) {
				chargeAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0, 0), Vector2f(-4*4, 0), Vector2f(-4*4, 0)});
				flyAnim->setOffsetList(list<Vector2f>{Vector2f(-4 * 4, 0), Vector2f(-4 * 4, 0)});
			}
			else {
				chargeAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0) });
				flyAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0, 0)});
			}

		}
		faceRight = right;
		
	}

	void setSoundPoint(Sound** s) {
		sound = *s;
	}

	/*void arcSim(float* deltaT) {
		phys->move(currentAngle, deltaT, vertSpeed);

		
		if (vertSpeed <= 0 || phys->getPosition().y <= arcVertTarget) {
			currentAngle = 90;
		}
		else {
			vertSpeed = Maths::map(0, arcVertDist, 0, maxVertSpeed, fabs(phys->getPosition().y - arcVertTarget));
		}
	}*/

	/*void calcArc(player* p) {
		targetX = p->getPosition().x;
		distX = fabs(targetX - phys->getPosition().x);
		vertSpeed = maxVertSpeed;
		horiSpeed = distX * horiSpeedMult;
		arcVertTarget = arcVertDist - phys->getPosition().y;
		currentAngle = 270;
	}*/
};