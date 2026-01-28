#include "temp physics enemy.cpp"
#pragma once

class Scworm : public TempPhysicsEnemy {
	using TempPhysicsEnemy::TempPhysicsEnemy;
	animation* anim;
	animTimer* timer;

	int speed = 40;

	int moveAngle = 0;

public:
	void initial() {

		phys->setRect(IntRect(335, 538, 11, 16));
		phys->enableGravity(true);
		phys->setPosition(initialPos);

		anim = new animation(list<IntRect>{IntRect(335, 538, 11, 16), IntRect(357, 538, 11, 16)}, sprite);
		timer = new animTimer(anim, 8, true);

		phys->setVVelocity(800);

		moveAngle = (rand() % 2);

		if (moveAngle == 0) {
			moveAngle = 0;
		}
		else {
			moveAngle = 180;
		}

		hit = new objectHitbox(IntRect(0, 0, 11, 16), sprite);
		hurt = hit;
		hp = 2;
		damage = 2;

		code = "scworm";

		grounded = false;
	}

	int metalDam() {
		return 2;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		timer->run(deltaT);

		if (grounded) {
			objectHitbox* checkPoint;
			if (moveAngle == 0) {
				checkPoint = new objectHitbox(IntRect(sprite->getSize().x/2, 0, 5.5, 16), sprite);
			}
			else {
				checkPoint = new objectHitbox(IntRect(0, 0, 5.5, 16), sprite);
			}
			phys->move(moveAngle, deltaT, speed);
			bool onGround = false;
			for (tile* t : *tileList) {
				if (t->getGround() != NULL) {
					if (hitboxDetect::hitboxDetection(checkPoint, t->getGround())) {
						onGround = true;
					}
				}
			}
			if (!onGround) {
				swap();
			}
			delete checkPoint;
		}
		else {
			phys->eachFrame(deltaT);
			tileCollision(tileList);
		}
	}

	void swap() {
		if (moveAngle == 0) {
			moveAngle = 180;
		}
		else {
			moveAngle = 0;
		}
	}


};