#include "sniper bullet.cpp"
#include "animation timer.cpp"
#pragma once

class WaterBullet : public SniperBullet {
	using SniperBullet::SniperBullet;
	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

public:
	void spriteSetup(shared_ptr<Texture> t, Vector2f position) {
		mov = shared_ptr<movable>(new movable(t, IntRect(39, 86, 12, 6), position, Vector2f(4, 4)));
		sprite = mov;
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(39, 86, 16, 6), IntRect(56, 87, 11, 6), IntRect(73, 88, 12, 6)}, sprite));
		if (angle == 0) {
			anim->swapAll();
			anim->setOffsetList(list<Vector2f>{ Vector2f(0, 0), Vector2f(5 * 4, 0 * 4), Vector2f(4 * 4, 0 * 4)});
		}
		timer = shared_ptr<animTimer> (new animTimer(anim, 8, true));
		hit->setSprite(mov);
	}

	void animate(float* deltaT) {
		timer->run(deltaT);
	}
};