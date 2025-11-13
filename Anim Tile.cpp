#include "tile.cpp"
#include "animation timer.cpp"
#pragma once

class AnimTile : public tile {

protected:
	animation* anim;
	animTimer* timer;

public:

	AnimTile() {}

	AnimTile(Vector2f loc, Texture* t, int VNumber, int animNumber, int z) {
		sprite = new objectSprite("Tile", t, Vector2i(64, 16 * VNumber), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);
		location = loc;

		list<IntRect> animList = { IntRect(64, 16 * VNumber, 16, 16) };
		for (int i = 1; i < animNumber; i++) {
			animList.push_back(IntRect(64 + (16 * i), 16 * VNumber, 16, 16));
		}
		anim = new animation(animList, sprite);
		timer = new animTimer(anim, 8, true);

		type = "0-" + to_string(VNumber) + "-" + to_string(animNumber);

		this->z = z;
	}

	void animate(float* deltaT) {
		timer->run(deltaT);
	}
};