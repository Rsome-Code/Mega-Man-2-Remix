#include "Flash Tile.cpp"
#pragma once

class WWallFlash : public FlashTile {

	objectHitbox* leftHitbox;

public:
	WWallFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 208), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		leftHitbox = new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite);

		type = "w wall flash";
	}

	objectHitbox* getLeft() {
		return leftHitbox;
	}

	void update() {
		leftHitbox->updatePos();
	}

};