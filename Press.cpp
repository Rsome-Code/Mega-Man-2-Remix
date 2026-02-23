#include "physics enemy.cpp"
#pragma once

class Press:public PhysicsEnemy{
	using PhysicsEnemy::PhysicsEnemy;
	void initial() {
		phys->setRect(IntRect(732, 185, 32, 112));
		phys->setPosition(initialPos);
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(4*4, 4*96, 24, 16), phys));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 0, 0), phys));

		hp = 99;
		damage = 5;
		phys->enableGravity(true);
		//setCode("press");
		setCode();
	}

	void setCode() {
		code = "press";
	}

	enum State {
		moveDown, moveUp
	};
	State state = moveUp;

	float delay = 0.5;
	float delay_left = delay;

	int upSpeed = 200;

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		delay_left -= *deltaT;
		if (delay_left <= 0) {

			if (state == moveUp) {
				phys->move(270, deltaT, upSpeed);
				
				for (shared_ptr<tile> t : *tileList) {
					if (t->getCeiling() != NULL) {
						if (hitboxDetect::hitboxDetection(t->getCeiling(), hit)) {
							state = moveDown;
							delay_left = delay;
						}
					}
				}
			}
			else if (state == moveDown) {
				phys->eachFrame(deltaT);
				for (shared_ptr<tile> t : *tileList) {
					if (t->getGround() != NULL) {
						if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
							state = moveUp;
							delay_left = delay;
							phys->setPosition(Vector2f(phys->getPosition().x, t->getGround()->getPosition().y - phys->getSize().y));
							sound->play();
							phys->setVVelocity(0);
						}
					}
				}
			}

		}
	}

	int genericDam() {
		return 0;
	}

};