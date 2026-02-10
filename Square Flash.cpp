#include "Flash Tile.cpp"
#pragma once

class SquareFlash : public FlashTile {

	objectHitbox* groundHitbox;
	objectHitbox* leftHitbox;
	objectHitbox* rightHitbox;
	objectHitbox* ceilingHitbox;

public:

	SquareFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 128), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		groundHitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite);
		ceilingHitbox = new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite);
		leftHitbox = new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite);
		rightHitbox = new objectHitbox(IntRect(Vector2i(size, 4), Vector2i(1, 16)), true, sprite);

		type = "square flash";
	}

	objectHitbox* getGround() {
		return groundHitbox;
	}
	objectHitbox* getCeiling() {
		return ceilingHitbox;
	}
	objectHitbox* getLeft() {
		return leftHitbox;
	}
	objectHitbox* getRight() {
		return rightHitbox;
	}

	void update() {
		groundHitbox->updatePos();
		leftHitbox->updatePos();
		rightHitbox->updatePos();
		ceilingHitbox->updatePos();
	}
};