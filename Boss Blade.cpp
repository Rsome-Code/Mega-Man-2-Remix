#include "enemy bullet.cpp"
#include "animation timer.cpp"
#pragma once

class BossBlade : public EnemyBullet {

	animation* anim;
	animTimer* timer;

	int angle;
	


public:
	BossBlade(Texture* t, Vector2f pos, Vector2f target) {
		mov = new movable(t, IntRect(1, 121, 16, 16), pos, Vector2f(4, 4));
		sprite = mov;
		anim = new animation(list<IntRect>{IntRect(1, 121, 16, 16), IntRect(18, 121, 16, 16)}, mov);
		timer = new animTimer(anim, 8, true);

		angle = Maths::getAngle(pos, target);
		speed = 800;

		hit = new objectHitbox(IntRect(0, 0, 16, 16), sprite);
		damage = 3;
	}

	void eachFrame(float* deltaT, list<tile*>* tileList) {
		timer->run(deltaT);
		hit->updatePos();
		mov->move(angle, deltaT, speed);
	}
};