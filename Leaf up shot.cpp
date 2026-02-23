#include "enemy bullet.cpp"
#pragma once

class LeafUpShot :public EnemyBullet {

	float speed = 1400;

public:
	
	LeafUpShot(shared_ptr<Texture> t, Vector2f pos){

		mov = shared_ptr<movable>(new movable(string("eBullet"), t, IntRect(337, 335, 15, 15), Vector2f(pos), Vector2f(4, 4)));
		sprite = mov;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 15, 15), mov));
		damage = 5;
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		mov->move(270, deltaT, speed);
		hit->updatePos();

	}
};