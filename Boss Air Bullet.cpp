#include "enemy bullet.cpp"
#pragma once

class BossAirBullet : public EnemyBullet {
	
	Vector2f position;

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	bool right = false;

public:

	BossAirBullet(shared_ptr<Texture> t, Vector2f pos, Vector2f startPos, bool faceRight){

		position = pos;
		mov = shared_ptr<movable>(new movable(t, IntRect(3, 132, 12, 15), startPos, Vector2f(4, 4)));
		sprite = mov;
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(3, 132, 12, 15), IntRect(20, 131, 14, 16), IntRect(39, 131, 12, 16)}, mov));
		timer = shared_ptr<animTimer>(new animTimer(anim, 8, true));

		code = "boss air bullet";

		right = faceRight;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 12, 15), mov));

		collType = DESTROY;
	
	}

	enum State{
		starting, moving
	};

	State state = starting;

	int maxSpeed = 800;
	float speed = 0;
	float accel = 800;

	float untilMove = 1;
	float untilMove_left = untilMove;

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {

		timer->run(deltaT);

		hit->updatePos();

		if (state == starting) {

			if (!Maths::inProximity(mov->getPosition(), position, 5)) {
				mov->move(Maths::getAngle(mov->getPosition(), position), deltaT, maxSpeed);
			}
				
			untilMove_left -= *deltaT;
			if (untilMove_left <= 0) {
				state = moving;
			}
			

		}

		else if (state == moving) {
			if (right) {
				mov->move(Angle::right, deltaT, speed);
			}

			else {
				mov->move(Angle::left, deltaT, speed);
			}

			speed += (accel * *deltaT);
		}

		


	}
};