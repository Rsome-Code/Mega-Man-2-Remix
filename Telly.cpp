#include "temp enemy.cpp"
#pragma once

class Telly : public TempEnemy {
	using TempEnemy::TempEnemy;
	shared_ptr<movable> mov;
	shared_ptr<animation> spinAnim;
	shared_ptr<animTimer> spinTimer;
	float speed = 70;

public:

	virtual ~Telly() {

	}

	void initial(Vector2f pos) {
		
		mov = shared_ptr<movable>(new movable(sprite->getTexture(), IntRect(429, 627, 16, 16), pos, Vector2f(4, 4)));
		sprite = mov;
		spinAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(429, 627, 16, 16), IntRect(446, 627, 16, 16), IntRect(463, 627, 16, 16), IntRect(480, 627, 16, 16), IntRect(497, 627, 16, 16), IntRect(514, 627, 16, 16)}, mov));
		spinTimer = shared_ptr<animTimer> (new animTimer(spinAnim, 6, true));
		offSetList();
		hp = 1;
		damage = 2;
		mov->setSpeed(speed);
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), mov));
		hurt = hit;
		setCode("telly");
		deathAnim->setSprite(sprite);

		display = true;
		act = true;
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
		spinTimer->run(deltaT);
		float angle = Maths::getAngle(mov->getPosition(), p->getPosition());
		mov->move(angle, deltaT);

	}

	
};