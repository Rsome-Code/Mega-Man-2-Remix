#include "Enemy.cpp"
#include "maths.cpp"
#include "temp enemy.cpp"
#pragma once



class SmallBird :public TempEnemy {
	enum State {
		SCATTER, TRACK, TRACKING
	};
	using TempEnemy::TempEnemy;

	shared_ptr<movable> mov;

	State state;

	float untilTrack = 0.3;
	float untilTrack_left = untilTrack;
	float movAngle;
	float speed = 200;

	shared_ptr<animation> flapAnim;
	shared_ptr<animTimer> flapTime;

public:

	virtual ~SmallBird() {

	}

	void initial() {
		mov = shared_ptr<movable>(new movable("eh", sprite->getTexture(), IntRect(504, 437, 8, 8), initialPos, Vector2f(4, 4)));
		sprite = mov;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 8), mov));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 8), mov));
		state = SCATTER;

		movAngle = rand() % 360;

		flapAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(504, 437, 8, 8), IntRect(513, 437, 8, 8)}, sprite));
		flapTime = shared_ptr<animTimer> (new animTimer(flapAnim, 15, true));

		offSetList();

		hp = 1;

		deathAnim->setSprite(mov);
		damage = 3;
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
		if (state == SCATTER || state == TRACKING) {
			if (state == SCATTER) {
				scatterLoop(deltaT);
			}
			moveLoop(deltaT);
		}
		else {
			trackStart(p);
		}


		flapTime->run(deltaT);
	}

	void moveLoop(float* deltaT) {
		mov->move(movAngle, deltaT, 800);
	}

	void trackStart(shared_ptr<player> p) {
		movAngle = Maths::getAngle(mov->getPosition(), p->getPosition());
		state = TRACKING;
	}


	void scatterLoop(float* deltaT) {
		untilTrack_left -= *deltaT;

		if (untilTrack_left <= 0) {
			state = TRACK;
		}
	}
};