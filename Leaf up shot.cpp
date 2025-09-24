#include "enemy bullet.cpp"
#pragma once

class LeafUpShot :public EnemyBullet {

	float speed = 1400;

public:
	
	LeafUpShot(Texture* t, Vector2f pos){

		mov = new movable(string("eBullet"), t, IntRect(337, 335, 15, 15), Vector2f(pos), Vector2f(4, 4));
		sprite = mov;
		hit = new objectHitbox(IntRect(0, 0, 15, 15), mov);

	}

	void eachFrame(float* deltaT) {
		mov->move(270, deltaT, speed);
		hit->updatePos();

	}
};