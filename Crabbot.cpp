#include "grounded enemy.cpp"
#include "shell.cpp"
#pragma once

class Crabbot : public GroundedEnemy {
	using GroundedEnemy::GroundedEnemy;

	float slowSpeed = 200;
	float fastSpeed = 500;
	float speed;

	bool hurted = false;
	bool shot = false;

	list<IntRect> nakedList = { IntRect(359, 665, 21, 11), IntRect(384, 664, 21, 12) };

	void initial() {

		mov = shared_ptr<movable>(new movable(sprite->getTexture(), IntRect(284, 653, 21, 23), initialPos, Vector2f(4, 4)));
		sprite = mov;

		moveAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(284, 653, 21, 23), IntRect(309, 652, 21, 24)}, sprite));
		moveAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0 * 4, -1 * 4)});

		moveTimer = shared_ptr<animTimer> (new animTimer(moveAnim, 8, true));

		hp = 6;
		damage = 3;
		speed = slowSpeed;
		setCode("crabbot");

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 21, 23), sprite));
		hurt = hit;
		offSetList();
		deathAnim->setSprite(sprite);
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
		moveTimer->run(deltaT);
		checkDirection(tileList);
		mov->move(angle, deltaT, speed);

		if (hurted && !shot) {
			shoot(bList);
			hurted = false;
			shot = true;
		}

	}

	void shoot(list<shared_ptr<EnemyBullet>>* bList) {
		shared_ptr<Shell> temp = shared_ptr<Shell> (new Shell(sprite->getTexture(), sprite->getMiddlePos(), faceRight));
		bList->push_back(temp);
	}

	int busterDam() {
		return 2;
	}

	void onDamage() {
		hurted = true;
		speed = fastSpeed;

		moveAnim = shared_ptr<animation>(new animation(nakedList, sprite));
		moveTimer->setAnim(moveAnim);
		moveAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 12 * 4), Vector2f(0 * 4, 11 * 4)});
		moveAnim->thisFrame();
		if (faceRight) {
			moveAnim->swapAll();
		}

		hit->setRelativeRect(IntRect(0,0, 21, 11));
		//hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 12*4, 21, 11), sprite));
		//hurt = hit;
		
	}

	
};