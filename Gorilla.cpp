#include "physics enemy.cpp"
#pragma once

class Gorilla : public PhysicsEnemy {
	using PhysicsEnemy::PhysicsEnemy;
	animation* hangAnim;
	animTimer* hangTimer;
	animation* stand;
	animation* jump;
	animation* startHang;

	bool hanging = true;
	bool jumping = false;

	float hangTime = 3;
	float hangTime_left = hangTime;

	float jumpForce = 1000;
	float moveSpeed = 500;

	void initial() {
		phys->setRect(IntRect(1, 500, 33, 42));
		phys->setPosition(initialPos);
		sprite = phys;

		list<Vector2f> testOffset = list<Vector2f>{ Vector2f(8 * 4,0 * 4),Vector2f(-3 * 4, 0 * 4),Vector2f(-12 * 4, 0 * 4), Vector2f(-3 * 4, 0 * 4) };
		hangAnim = new animation(list<IntRect>{IntRect(35, 503, 33, 47), IntRect(69, 500, 33, 50), IntRect(103, 503, 33, 47), IntRect(69, 500, 33, 50)}, phys);
		
		hangAnim->setOffsetList(testOffset);

		hangTimer = new animTimer(hangAnim, 5, true);
		stand = new animation(IntRect(181, 511, 40, 37), phys);
		jump = new animation(IntRect(139, 503, 39, 42), phys);
		startHang = new animation(IntRect(1, 500, 33, 42), phys);

		hit = new objectHitbox(IntRect(0, 0, 40, 37), phys);
		hurt = new objectHitbox(IntRect(0, 0, 40, 37), phys);

		hp = 6;
		damage = 6;

		setCode("gorilla");

		offSetList();
		grounded = false;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		checkDirection(p->getSprite());
		if (hanging) {
			hangLoop(deltaT);
		}
		else if (jumping) {
			jumpLoop(deltaT, tileList);
		}
		else {
			standLoop();
		}
	}

	void standLoop() {

	}

	void standAction() {
		stand->thisFrame();
	}

	void hangLoop(float* deltaT){
		hangTimer->run(deltaT);
		hangTime_left -= *deltaT;

		if (hangTime_left <= 0) {
			jumping = true;
			hanging = false;
			jumpAction();
		}
	}

	void jumpLoop(float* deltaT, list<tile*>* tileList) {
		phys->eachFrame(deltaT);
		tileCollision(tileList);
		if (grounded) {
			jumping = false;
			standAction();
		}
	}

	void jumpAction() {
		phys->enableGravity(true);
		if (!faceRight) {
			moveSpeed = -moveSpeed;
		}
		phys->setVVelocity(jumpForce);
		phys->setHVelocity(moveSpeed);
		jump->thisFrame();
	}

	void setFacing(bool right) {
		if (right) {
			faceRight = right;
			if (!hangAnim->getSwapped()) {
				hangAnim->swapAll();
				stand->swapAll();
				jump->swapAll();
				startHang->swapAll();
			}
		}
		else {
			if (hangAnim->getSwapped()) {
				hangAnim->swapAll();
				stand->swapAll();
				jump->swapAll();
				startHang->swapAll();
			}
		}
	}
};
