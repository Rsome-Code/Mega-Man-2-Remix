#include "enemy.cpp"
#pragma once

class BreakWall : public enemy {
	using enemy::enemy;

	shared_ptr<objectHitbox> left;
	shared_ptr<objectHitbox> right;

public:

	virtual ~BreakWall() {

	}

	void initial() {

		sprite->setRect(IntRect(535, 627, 16, 32));
		sprite->setScale(Vector2f(4, 4));
		sprite->setPosition(initialPos);

		hp = 1;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(10, 10, 0, 0), sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 32), sprite));
		damage = 0;

		left = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 32), sprite));
		right = shared_ptr<objectHitbox>(new objectHitbox(IntRect(8*4, 0, 8, 32), sprite));

		setCode("break wall");
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
		
		if (hitboxDetect::hitboxDetection(p->getHitbox(), left)) {
			p->getSprite()->setPosition(Vector2f(left->getPosition().x - (p->getHitbox()->getSize().x + 17), p->getSprite()->getPosition().y));
			p->updateHitbox();
		}
		if (hitboxDetect::hitboxDetection(p->getHitbox(), right)) {
			p->getSprite()->setPosition(Vector2f(right->getPosition().x + right->getSize().x - 20, p->getSprite()->getPosition().y));
			p->updateHitbox();
		}

	}

	int genericDam() {
		return 0;
	}
	int crashDam() {
		return 1;
	}


	void heatmanPallete() {
		sprite->setRect(IntRect(569, 627, 16, 32));
	}

	shared_ptr<objectHitbox> getLeft() {
		return left;
	}
	shared_ptr<objectHitbox> getRight() {
		return right;
	}
};