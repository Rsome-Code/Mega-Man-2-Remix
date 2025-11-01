#include "master.cpp"
#pragma once

class BubbleMan : public Master{

	IntRect idle = IntRect(1, 23, 28, 28);

	animation* swimAnim;
	animTimer* swimTimer;

	animation* swimShoot;
	animTimer* shootTimer;

	animation* bubbleShootAnim;
	animTimer* bubbleTimer;

	void initial() {
		Texture* t = new Texture();
		t->loadFromFile("assets\\wood man.png");
		phys->setTexture(t);

		phys->setRect(IntRect(148, 158, 29, 33));

		introAnim = new animation(list<IntRect>{idle, IntRect(39, 48, 25, 30), IntRect(71, 81, 28, 27)}, sprite);
		introTimer = new animTimer(introAnim, 8, false);

		swimAnim = new animation(list<IntRect>{IntRect(148, 22, 29, 33), IntRect(182, 22, 28, 29)}, sprite);
		swimTimer = new animTimer(swimAnim, 8, true);

		swimShoot = new animation(list<IntRect>{IntRect(218, 23, 28, 33), IntRect(251, 23, 28, 30)}, sprite);
	}

};