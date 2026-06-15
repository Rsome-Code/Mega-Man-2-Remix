#include "enemy bullet.cpp"
#pragma once

class DragonFire : public EnemyBullet {

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	Vector2f startMove;

	Vector2f startPos;

	bool starting;
public:

	DragonFire(shared_ptr<Texture> tex, Vector2f pos, int ang) {
		mov = shared_ptr<movable>(new movable(tex, IntRect(997, 71, 24, 16), pos, Vector2f(4, 4)));
		sprite = mov;
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(997, 71, 24, 16), IntRect(1029, 73, 19, 12)}, mov));
		anim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4)});
		timer = shared_ptr<animTimer>(new animTimer(anim, 8, true));

		speed = 200;
		angle = ang;

		damage = 7;

		startMove = pos;
		starting = false;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(1, 1, 22, 14), mov));
	}

	DragonFire(shared_ptr<Texture> tex, Vector2f pos) {
		mov = shared_ptr<movable>(new movable(tex, IntRect(997, 71, 24, 16), pos, Vector2f(8, 8)));
		sprite = mov;

		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(997, 71, 24, 16), IntRect(1029, 73, 19, 12)}, mov));
		anim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 2 * 4)});
		timer = shared_ptr<animTimer>(new animTimer(anim, 8, true));

		speed = 200;
		angle = Angle::left;

		damage = 7;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 24, 16), mov));
		starting = false;
	}

	void setStartMove(Vector2f startPos) {
		startMove = startPos;
		starting = true;

		this->startPos = mov->getPosition();
	}

	float startTime = 0.8;
	float startTime_left = startTime;

	void startingLoop(float* deltaT) {
		//mov->move(Maths::getAngle(mov->getPosition(), startMove), deltaT, speed);
		//if (Maths::inProximity(mov->getPosition(), startMove, 10)) {
		//	starting = false;
		//}

		float distance = Maths::getDistance(mov->getPosition(), startMove);

		float startDistance = Maths::getDistance(startPos, startMove);

		startTime_left -= *deltaT;

		float newDist = Maths::map(0, startTime, 0, startDistance, startTime_left);

		newDist = startDistance - newDist;

		mov->setPosition(startPos);

		mov->move(Maths::getAngle(mov->getPosition(), startMove), newDist);


		if (Maths::inProximity(mov->getPosition(), startMove, 10) || startTime_left <= 0) {
			starting = false;
		}
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList, shared_ptr<player> p) {

		if (starting) {

			startingLoop(deltaT);

		}
		else {
			mov->move(angle, deltaT, speed);
			timer->run(deltaT);
		}

		hit->updatePos();
	}
};