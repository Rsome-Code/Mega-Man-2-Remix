#include "Flash Tile.cpp"
#pragma once

class BottomLeftCornerWFlash : public FlashTile {


	objectHitbox* leftHitbox;

	objectHitbox* ceilingHitbox;

public:
	BottomLeftCornerWFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 256), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		ceilingHitbox = new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite);
		leftHitbox = new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite);


		type = "bottom left corner w flash";
	}


	objectHitbox* getCeiling() {
		return ceilingHitbox;
	}
	objectHitbox* getLeft() {
		return leftHitbox;
	}


	void update() {

		leftHitbox->updatePos();

		ceilingHitbox->updatePos();
	}
};