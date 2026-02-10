#include "Flash Tile.cpp"
#pragma once

class BottomRightCornerFlash : public FlashTile {

	objectHitbox* rightHitbox;
	objectHitbox* ceilingHitbox;
public:
	BottomRightCornerFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 80), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);


		ceilingHitbox = new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite);

		rightHitbox = new objectHitbox(IntRect(Vector2i(size, 4), Vector2i(1, 16)), true, sprite);

		type = "bottom right corner flash";
	}


	objectHitbox* getCeiling() {
		return ceilingHitbox;
	}

	objectHitbox* getRight() {
		return rightHitbox;
	}

	void update() {

		rightHitbox->updatePos();
		ceilingHitbox->updatePos();
	}
};