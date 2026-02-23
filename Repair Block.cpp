#include "physics bullet.cpp"
#include "animation timer.cpp"

#pragma once

class RepairBlock : public EnemyBullet {

	int moveSpeed = 500;
	int angle = 270;
	float yTarget;

public:
	RepairBlock(shared_ptr<Texture> t, Vector2f pos, float yTarget) {
		mov = shared_ptr<movable>(new movable(t, IntRect(567, 76, 16, 16), pos, Vector2f(4, 4)));
		sprite = mov;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), mov));


		this->yTarget = yTarget;
		code = "repair block";

	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		mov->move(angle, deltaT, moveSpeed);
		if (mov->getPosition().y <= yTarget) {
			mov->setPosition(Vector2f(-9999, -9999));
		}
	}

};