#include "item bullets.cpp"
#include "animation timer.cpp"
#pragma once

class Item1Bullet : public ItemBullet {

	shared_ptr<animation> flyAnim;
	shared_ptr<animTimer> flyTimer;

	float time = 5;

	int moveSpeed = 50;

	float flashTime = 0.1;
	float flashTime_left = 0;

public:

	Item1Bullet(shared_ptr<Texture> t, Vector2f pos) {
		phys = shared_ptr<physicsObject> (new physicsObject(t, IntRect(396, 287, 22, 13), pos, Vector2f(4, 4)));

		flyAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(396, 287, 22, 13), IntRect(421, 287, 22, 13)}, phys));
		flyTimer = shared_ptr<animTimer> (new animTimer(flyAnim, 8, true));

		hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 22, 2), phys));
		code = "item 1";

		exploSetup();
	
	}

	bool eachFrame(float* deltaT, list<shared_ptr<tile>> tiles) {
		hitbox->updatePos();
		phys->move(270, deltaT, moveSpeed);

		flyTimer->run(deltaT);

		time -= *deltaT;

		if (time <= 2) {
			flash(deltaT);
		}

		if (time <= 0) {
			display = true;
			return true;
		}
		return false;
	}

	void flash(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			display = !display;
			flashTime_left = flashTime;
		}
	}
};