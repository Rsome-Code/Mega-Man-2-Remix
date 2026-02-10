#include "Flash Tile.cpp"
#pragma once

class VTubeTile : public FlashTile {

	objectHitbox* leftHitbox;
	objectHitbox* rightHitbox;

public:
	VTubeTile(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 32), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		leftHitbox = new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite);
		rightHitbox = new objectHitbox(IntRect(Vector2i(size, 4), Vector2i(1, 16)), true, sprite);

		type = "vertical flash";
	}


	objectHitbox* getLeft() {
		return leftHitbox;
	}
	objectHitbox* getRight() {
		return rightHitbox;
	}

	void update() {
		leftHitbox->updatePos();
		rightHitbox->updatePos();
	}
};