#include "enemy bullet.cpp"
#pragma once

class LeafFloat : public EnemyBullet {

	enum State {
		LEFT, RIGHT
	};

	State state = LEFT;

	float angle = 90;
	float angleSpeed = 175;
	

public:

	LeafFloat(Texture* t, Vector2f pos) {
		mov = new movable(string("eBullet"), t, IntRect(337, 335, 15, 15), Vector2f(pos), Vector2f(4, 4));
		sprite = mov;
		hit = new objectHitbox(IntRect(0, 0, 15, 15), mov);
		speed = 300;
		damage = 4;
	}

	void eachFrame(float* deltaT, list<tile*>* tileList) {
		if (state == LEFT) {
			floatLeft(deltaT);
		}
		else if (state == RIGHT) {
			floatRight(deltaT);
		}

		mov->move(angle, deltaT, speed);
		hit->updatePos();
	}

	void floatLeft(float* deltaT) {
		angle += (angleSpeed * *deltaT);
		if (angle >= 180) {
			state = RIGHT;
		}
	}

	void floatRight(float* deltaT) {
		angle -= (angleSpeed * *deltaT);
		if (angle <= 0) {
			state = LEFT;
		}
	}
};