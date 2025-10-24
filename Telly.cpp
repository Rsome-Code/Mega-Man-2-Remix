#include "temp enemy.cpp"
#pragma once

class Telly : public TempEnemy {
	using TempEnemy::TempEnemy;
	movable* mov;
	animation* spinAnim;
	animTimer* spinTimer;
	float speed = 70;

public:

	void initial(Vector2f pos) {
		
		mov = new movable(sprite->getTexture(), IntRect(429, 627, 16, 16), pos, Vector2f(4, 4));
		sprite = mov;
		spinAnim = new animation(list<IntRect>{IntRect(429, 627, 16, 16), IntRect(446, 627, 16, 16), IntRect(463, 627, 16, 16), IntRect(480, 627, 16, 16), IntRect(497, 627, 16, 16), IntRect(514, 627, 16, 16)}, mov);
		spinTimer = new animTimer(spinAnim, 6, true);
		offSetList();
		hp = 1;
		damage = 2;
		mov->setSpeed(speed);
		hit = new objectHitbox(IntRect(0, 0, 16, 16), mov);
		hurt = hit;
		setCode("telly");
		deathAnim->setSprite(sprite);

		display = true;
		act = true;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		spinTimer->run(deltaT);
		float angle = Maths::getAngle(mov->getPosition(), p->getPosition());
		mov->move(angle, deltaT);

	}

	
};