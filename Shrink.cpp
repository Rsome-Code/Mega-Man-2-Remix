#include "temp enemy.cpp"
#include "maths.cpp"
#pragma once

class Shrink : public TempEnemy {
	using TempEnemy::TempEnemy;

	enum State {
		moving, idle
	};

	State state = idle;

	int fallSpeed = 100;
	int moveSpeed = 500;

	int angle;

	float moveTime = 0.6;
	float moveTime_left = moveTime;

	float fallTime = 1;
	float fallTime_left = 0;

	shared_ptr<animation> moveAnim;
	shared_ptr<animTimer> moveTimer;

	shared_ptr<animation> fallAnim;
	shared_ptr<animTimer> fallTimer;

public:

	virtual ~Shrink() {

	}

	Shrink(shared_ptr<Texture> t, Vector2f i) {

		mov = shared_ptr<movable>(new movable(t));
		sprite = mov;
		//deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite));

		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite));

		offSetList();

		deathTimer = shared_ptr<animTimer> (new animTimer(deathAnim, 16, false));
		initialPos = i;
		act = false;
		display = false;
		spawned = false;


	}

	void initial() {
		
		mov->setPosition(initialPos);
		
		mov->setRect(IntRect(39, 660, 27, 30));
		mov->setScale(Vector2f(4, 4));

		hp = 3;
		damage = 2;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 27, 30), sprite));
		hurt = hit;
		offSetList();
		state = idle;

		setCode("shrink");
		
		fallAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(72, 660, 27, 30), IntRect(105, 660, 27, 30)}, sprite));
		fallTimer = shared_ptr<animTimer> (new animTimer(fallAnim, 8, true));

		moveAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(39, 660, 27, 30), IntRect(72, 660, 27, 30), IntRect(1, 663, 27, 24)}, sprite));
		moveTimer = shared_ptr<animTimer> (new animTimer(moveAnim, 8, false));
	}

	int metalDam() {
		return 10;
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
		spawned = true;
		checkDirection(p->getSprite());

		if (state == moving) {
			moveTimer->run(deltaT);
			moveTimer->run(deltaT);
			moveTime_left -= *deltaT;
			if (moveTime_left <= 0) {
				moveTime_left = moveTime;
				state = idle;
				moveAnim->reset();
				moveTimer->reset();
			}

			mov->move(angle, deltaT, moveSpeed);
		}

		else {
			fallTimer->run(deltaT);
			fallTime_left -= *deltaT;
			if (fallTime_left <= 0) {
				fallTime_left = fallTime;
				moveStart(p->getSprite());
				state = moving;
			}
			mov->move(90, deltaT, fallSpeed);
		}
	}

	void setFacing(bool r) {
		if (r != faceRight) {
			moveAnim->swapAll();
			fallAnim->swapAll();
		}
		faceRight = r;
	}

	void moveStart(shared_ptr<objectSprite> pl) {
		angle = Maths::getAngle(sprite->getMiddlePos(), pl->getMiddlePos());
	}
};