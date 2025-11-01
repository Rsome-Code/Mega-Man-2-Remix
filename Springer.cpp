#include "grounded enemy.cpp"
#include "hitbox detector.cpp"
#pragma once

class Springer : public GroundedEnemy {
	using GroundedEnemy::GroundedEnemy;
	animation* springAnim;
	animTimer* springTimer;



	IntRect moveS = IntRect(146, 648, 16, 11);

	float slowSpeed = 200;
	float fastSpeed = 600;
	float speed = slowSpeed;


	float springTime = 1.5;
	float springTime_left = 0;

	list<Vector2f> leftOffset = list<Vector2f>{ Vector2f(0 * 4, -8 * 4), Vector2f(-8 * 4, -21 * 4), Vector2f(0 * 4, -8 * 4), Vector2f(0 * 4, -21 * 4) };

	enum State {
		move, spring
	};

	State state = move;

	void initial() {
		moveAnim = new animation(list<IntRect>{moveS}, sprite);
		mov->setRect(moveS);
		mov->setPosition(initialPos);
		mov->setScale(Vector2f(4, 4));
		springAnim = new animation(list<IntRect>{IntRect(164, 640, 16, 19), IntRect(181, 627, 24, 32), IntRect(206, 640, 16, 19), IntRect(224, 627, 24, 32)}, sprite);

		springAnim->setOffsetList(leftOffset);
		springTimer = new animTimer(springAnim, 8, true);
		hit = new objectHitbox(IntRect(0, 0, 16, 11), mov);
		hurt = hit;
		setCode("springer");
		offSetList();
		hp = 1;
		damage = 3;
		//faceRight = false;
		//setFacing(true);
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {

		springTime_left -= *deltaT;

		if (springTime_left <= 0) {
			hit->setRelativePosition(Vector2i(hit->getRelativePosition().x, 0));
			moveAnim->thisFrame();
			mov->setVisualOffset(Vector2f(0, 0));
			moveLoop(deltaT, tileList, p);
			
		}
		else {
		//	hit->setRelativePosition(Vector2f(hit->getRelativePosition().x, -20000));
			springLoop(deltaT);
			
		}
			
		
	}

	void springLoop(float* deltaT) {
		springTimer->run(deltaT);
	}

	void moveLoop(float* deltaT, list<tile*>* tileList, player* p) {

		checkDirection(tileList);
		mov->move(angle, deltaT, speed);
		

		if (p->getGrounded()) {
			
			if (hit->getPosition().y < (p->getHitbox()->getPosition().y + p->getHitbox()->getSize().y) && (p->getHitbox()->getPosition().y + p->getHitbox()->getSize().y) < hit->getPosition().y + hit->getSize().y) {
				speed = fastSpeed;
			}
			else {
				speed = slowSpeed;
			}
			

		}
		else {
			speed = slowSpeed;
		}
	}



	void playerHit(){
		state = spring;
		springTime_left = springTime;
	}

	int genericDam() {
		return 0;
	}
	int bubbleDam() {
		return 1;
	}
};