#include "temp physics enemy.cpp"
#pragma once

class GearSaw : public TempPhysicsEnemy {
	using TempPhysicsEnemy::TempPhysicsEnemy;

	animation* anim;
	animTimer* timer;

	int speed = 400;
	int angle;

	bool start = false;
	bool firstFrame = false;

public:
	void initial() {
		phys->setRect(IntRect(399, 491, 32, 32));
		sprite = phys;

		hit = new objectHitbox(IntRect(0, 0, 32, 32), sprite);
		hurt = hit;

		anim = new animation(list<IntRect>{IntRect(399, 491, 32, 32), IntRect(432, 491, 32, 32)}, sprite);
		timer = new animTimer(anim, 8, true);
		phys->enableGravity(true);
		grounded = false;

		angle = 180;

		hp = 5;
		damage = 3;
		start = false;
		code = "gear saw";

		offSetList();
	}

	void hitLeft() {
		setFacing(false);
	}
	void hitRight() {
		setFacing(true);
	}

	void moveNow() {
		start = true;
		firstFrame = true;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		timer->run(deltaT);
		
		if (firstFrame) {
			checkDirection(p->getSprite());

		}
		if (start) {
			phys->eachFrame(deltaT);
			tileCollision(tileList);
			if (grounded) {
				phys->move(angle, deltaT, speed);
			}
		}
	}

	void setFacing(bool r) {
		faceRight = r;
		if (faceRight) {
			angle = 0;
		}
		else {
			angle = 180;
		}
	}



};