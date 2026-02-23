#include "enemy bullet.cpp"
#include "animation timer.cpp"
#pragma once

class BossBlade : public EnemyBullet {

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	int angle;
	


public:
	BossBlade(shared_ptr<Texture> t, Vector2f pos, Vector2f target) {
		mov = shared_ptr<movable>(new movable(t, IntRect(1, 121, 16, 16), pos, Vector2f(4, 4)));
		sprite = mov;
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(1, 121, 16, 16), IntRect(18, 121, 16, 16)}, mov));
		timer = shared_ptr<animTimer> (new animTimer(anim, 8, true));

		angle = Maths::getAngle(pos, target);
		speed = 800;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), sprite));
		damage = 3;
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		timer->run(deltaT);
		hit->updatePos();
		mov->move(angle, deltaT, speed);
	}
};