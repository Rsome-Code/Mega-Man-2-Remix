#include "spawn on top.cpp"
#include "temp physics enemy.cpp"
#pragma once

class Snapper : public TempPhysicsEnemy {
	using TempPhysicsEnemy::TempPhysicsEnemy;

	int fallSpeed = 200;

	animation* anim;
	animTimer* aTimer;

	enum State {
		falling, jumping, running
	};

	State state = falling;

	float jumpDelay = 0.1;
	float jumpDelay_left = jumpDelay;
	int jumps = 3;

	int jumpForce = 450;

	int runSpeed = 700;
	int runAngle = 180;

public:
	void initial() {
		phys->setPosition(initialPos);
		phys->setRect(IntRect(473, 115, 22, 16));
		mov = phys;

		//offSetList();
		state = falling;

		grounded = false;

		hp = 2;

		anim = new animation(list<IntRect>{IntRect(473, 115, 22, 16), IntRect(497, 109, 27, 22)}, sprite);
		anim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(-3 * 4, -7 * 4)});
		aTimer = new animTimer(anim, 8, true);
		hit = new objectHitbox(IntRect(0, 0, 22, 16), sprite);
		hurt = hit;

		setCode("snapper");

		offSetList();
	}


	int metalDam() {
		return 2;
	}


	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {

		aTimer->run(deltaT);

		if (state == falling) {
			phys->move(90, deltaT, fallSpeed);
			for (tile* t : *tileList) {
				if (t->getGround() != NULL) {
					groundCheck(t);
				}
			}

			if (grounded) {
				state = jumping;
				phys->enableGravity(true);
				checkDirection(p->getSprite());
			}
		}

		else if (state == jumping){
			phys->eachFrame(deltaT);
			tileCollision(tileList);
			if (grounded) {
				
				jumpDelay_left -= *deltaT;
				if (jumpDelay_left <= 0) {
					jumpDelay_left = jumpDelay;
					jump();
					grounded = false;
					if (jumps <= 0) {
						state = running;
						
					}
					jumps -= 1;
					
				}
			}
		}

		else if (state == running) {
			grounded = false;
			for (tile* t : *tileList) {
				if (t->getGround() != NULL) {
					groundCheck(t);
				}
			}
			if (grounded) {
				phys->move(runAngle, deltaT, runSpeed);
			}
			else {
				//phys->enableGravity(true);
				phys->eachFrame(deltaT);
			}

		}

	}

	void setFacing(bool r) {
		if (r != faceRight) {
			anim->swapAll();

			if (r) {
				runAngle = 0;
			}
			else {
				runAngle = 180;
				anim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(-2 * 4, -7 * 4)});
			}
		}
		faceRight = r;
		
	}

	void jump() {
		phys->setVVelocity(jumpForce);
	}
};