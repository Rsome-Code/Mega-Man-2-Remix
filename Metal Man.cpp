#include "master.cpp"
#include "tile.cpp"
#pragma once

class MetalMan : public Master {

	IntRect idle = IntRect(1, 27, 24, 24);
	IntRect jumpFrame = IntRect(173, 19, 23, 28);
	animation* walkAnim;
	animTimer* walkTimer;

	animation* throwAnim;
	animTimer* throwTimer;

	list<tile*> tiles;

	bool convRight;

	void ini() {
		Texture* t = new Texture();
		t->loadFromFile("assets\\metal man.png");
		phys->setTexture(t);

		phys->setRect(jumpFrame);
		sprite = phys;

		introAnim = new animation(list<IntRect>{idle, IntRect(34, 28, 28, 23), IntRect(64, 27, 23, 24)}, sprite);
		//introAnim->setOffsetList(list<Vector2f>{});
		introTimer = new animTimer(introAnim, 8, false);

		walkAnim = new animation(list<IntRect>{IntRect(97, 29, 24, 22), IntRect(127, 28, 16, 23), IntRect(145, 29, 21, 22)}, sprite);
		walkTimer = new animTimer(walkAnim, 8, true);

		throwAnim = new animation(list<IntRect>{IntRect(208, 11, 21, 36), IntRect(236, 242, 21, 27)}, sprite);
		throwTimer = new animTimer(throwAnim, 8, false);
		convRight = false;
	}

	void initial() {
		tiles = {};
		ini();
	}
	void initial(list<tile*>* tiles) {
		this->tiles = *tiles;
		ini();
	}

	float conveyorSwap = 2;
	float swap_left = conveyorSwap;

	void swapConveyors() {
		for (tile* t : tiles) {
			t->setMoveRight(convRight);
		}
	}

	void conveyorCheck(float* deltaT) {
		swap_left -= *deltaT;
		if (swap_left <= 0) {
			swap_left = conveyorSwap;
			swapConveyors();
		}
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		conveyorCheck(deltaT);

	}
};