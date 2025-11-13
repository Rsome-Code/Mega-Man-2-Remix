#include "temp enemy.cpp"
#pragma once

class Drill : public TempEnemy {
	using TempEnemy::TempEnemy;

	bool up;
	int angle;
	animation* anim;
	animTimer* timer;

	int slowSpeed = 50;
	int fastSpeed = 200;
	int speed = slowSpeed;

public:
	void initial(Texture* tex, Vector2f startP, SoundCollection* soundCol) {
		mov->setTexture(tex);
		mov->setRect(IntRect(354, 364, 8, 23));
		mov->setPosition(startP);

		hitSound = soundCol->getHit();

		setCode("drill");
		hp = 5;
		damage = 3;
		hit = new objectHitbox(IntRect(0, 0, 8, 23), mov);
		hurt = hit;
		offSetList();
	}

	void setUP(bool u) {
		up = u;

		if (!u) {
			angle = 90;
			mov->setRect(IntRect(mov->getRect().getPosition().x, mov->getRect().getPosition().y + mov->getRect().getSize().y, mov->getRect().getSize().x, mov->getRect().getSize().y));
			anim = new animation(list<IntRect>{IntRect(354, 387, 8, 23), IntRect(363, 387, 8, 24), IntRect(372, 387, 8, 24)}, mov);
			
		}
		else {
			angle = 270;
			anim = new animation(list<IntRect>{IntRect(354, 364, 8, 23), IntRect(363, 363, 8, 24), IntRect(372, 363, 8, 24)}, mov);
		}
		timer = new animTimer(anim, 8, true);
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		timer->run(deltaT);
		mov->move(angle, deltaT, speed);


		speed = fastSpeed;
		for (tile* t : *tileList) {
			if (t->getCeiling() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getCeiling(), hit)) {
					speed = slowSpeed;
				}
			}
			if (t->getGround() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
					speed = slowSpeed;
				}
			}
		}
	}
};