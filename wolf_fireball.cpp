#include "enemy Bullet.cpp"

#pragma once

class WolfFireBall : public EnemyBullet {

	float turnSpeed = 600;
	float maxAngle = 250;
public:


	WolfFireBall(shared_ptr<Texture> t, Vector2f pos) {
		mov = shared_ptr<movable>(new movable("bull", t, IntRect(594, 758, 16, 16), pos, Vector2f(4, 4)));
		sprite = mov;
		angle = 110;
		speed = 3000;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), mov));
		damage = 5;
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		hit->updatePos();
		angle = angle + (*deltaT * turnSpeed);
		if (angle >= maxAngle) {
			angle = maxAngle;
		}
		mov->move(angle, deltaT, speed);
		
	}

};