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

		mov = new movable(sprite->getTexture(), IntRect(284, 653, 21, 23), initialPos, Vector2f(4, 4));
		sprite = mov;

		moveAnim = new animation(list<IntRect>{IntRect(284, 653, 21, 23), IntRect(309, 652, 21, 24)}, sprite);
		moveAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0 * 4, -1 * 4)});

		moveTimer = new animTimer(moveAnim, 8, true);

		hp = 6;
		damage = 3;
		speed = slowSpeed;
		setCode("crabbot");

		hit = new objectHitbox(IntRect(0, 0, 21, 23), sprite);
		hurt = hit;
		offSetList();
		deathAnim->setSprite(sprite);
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		moveTimer->run(deltaT);
		checkDirection(tileList);
		mov->move(angle, deltaT, speed);

		if (hurted && !shot) {
			shoot(bList);
			hurted = false;
			shot = true;
		}

	}

	void shoot(list<EnemyBullet*>* bList) {
		Shell* temp = new Shell(sprite->getTexture(), sprite->getMiddlePos(), faceRight);
		bList->push_back(temp);
	}

	int busterDam() {
		return 2;
	}

	void onDamage() {
		hurted = true;
		speed = fastSpeed;
		delete moveAnim;
		moveAnim = new animation(nakedList, sprite);
		moveTimer->setAnim(moveAnim);
		moveAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 12 * 4), Vector2f(0 * 4, 11 * 4)});
		moveAnim->thisFrame();
		if (faceRight) {
			moveAnim->swapAll();
		}

		hit->setRelativeRect(IntRect(0,0, 21, 11));
		//hit = new objectHitbox(IntRect(0, 12*4, 21, 11), sprite);
		//hurt = hit;
		
	}

	
};