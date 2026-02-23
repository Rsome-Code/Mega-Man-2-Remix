#include "temp enemy.cpp"
#pragma once

class Drill : public TempEnemy {
	using TempEnemy::TempEnemy;

	bool up;
	int angle;
	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	int slowSpeed = 50;
	int fastSpeed = 200;
	int speed = slowSpeed;

public:

	virtual ~Drill() {

	}

	void initial(shared_ptr<Texture> tex, Vector2f startP, shared_ptr<SoundCollection> soundCol) {
		mov->setTexture(tex);
		mov->setRect(IntRect(354, 364, 8, 23));
		mov->setPosition(startP);

		hitSound = soundCol->getHit();

		setCode("drill");
		hp = 5;
		damage = 3;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 23), mov));
		hurt = hit;
		offSetList();
	}

	void setUP(bool u) {
		up = u;

		if (!u) {
			angle = 90;
			mov->setRect(IntRect(mov->getRect().getPosition().x, mov->getRect().getPosition().y + mov->getRect().getSize().y, mov->getRect().getSize().x, mov->getRect().getSize().y));
			anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(354, 387, 8, 23), IntRect(363, 387, 8, 24), IntRect(372, 387, 8, 24)}, mov));
			
		}
		else {
			angle = 270;
			anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(354, 364, 8, 23), IntRect(363, 363, 8, 24), IntRect(372, 363, 8, 24)}, mov));
		}
		timer = shared_ptr<animTimer> (new animTimer(anim, 8, true));
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
		timer->run(deltaT);
		mov->move(angle, deltaT, speed);


		speed = fastSpeed;
		for (shared_ptr<tile> t : *tileList) {
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