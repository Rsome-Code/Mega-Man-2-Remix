#include "Enemy.cpp"
#include "maths.cpp"
#include "temp enemy.cpp"
#pragma once



class SmallBird :public TempEnemy {
	enum State {
		SCATTER, TRACK, TRACKING
	};
	using TempEnemy::TempEnemy;

	movable* mov;

	State state;

	float untilTrack = 0.3;
	float untilTrack_left = untilTrack;
	float movAngle;
	float speed = 200;

	animation* flapAnim;
	animTimer* flapTime;

public:

	void initial() {
		mov = new movable("eh", sprite->getTexture(), IntRect(504, 437, 8, 8), initialPos, Vector2f(4, 4));
		sprite = mov;
		hit = new objectHitbox(IntRect(0, 0, 8, 8), mov);
		hurt = new objectHitbox(IntRect(0, 0, 8, 8), mov);
		state = SCATTER;

		movAngle = rand() % 360;

		flapAnim = new animation(list<IntRect>{IntRect(504, 437, 8, 8), IntRect(513, 437, 8, 8)}, sprite);
		flapTime = new animTimer(flapAnim, 15, true);

		offSetList();

		hp = 1;

		deathAnim->setSprite(mov);
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
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

	void trackStart(player* p) {
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