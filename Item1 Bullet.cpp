#include "item bullets.cpp"
#include "animation timer.cpp"
#pragma once

class Item1Bullet : public ItemBullet {

	animation* flyAnim;
	animTimer* flyTimer;

	float time = 4;

	int moveSpeed = 50;



public:

	Item1Bullet(Texture* t, Vector2f pos) {
		phys = new physicsObject(t, IntRect(396, 287, 22, 13), pos, Vector2f(4, 4));

		flyAnim = new animation(list<IntRect>{IntRect(396, 287, 22, 13), IntRect(421, 287, 22, 13)}, phys);
		flyTimer = new animTimer(flyAnim, 8, true);

		hitbox = new objectHitbox(IntRect(0, 0, 22, 2), phys);
		code = "item 1";
	
	}

	bool eachFrame(float* deltaT) {
		hitbox->updatePos();
		phys->move(270, deltaT, moveSpeed);

		flyTimer->run(deltaT);

		time -= *deltaT;
		if (time <= 0) {
			return true;
		}
		return false;
	}
};