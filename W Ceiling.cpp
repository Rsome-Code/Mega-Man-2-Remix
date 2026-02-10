#include "Flash Tile.cpp"
#pragma once

class WCeilingFlash : public FlashTile {

	objectHitbox* ceilingHitbox;

public:
	WCeilingFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 272), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		ceilingHitbox = new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite);

		type = "w ceiling flash";
	}

	objectHitbox* getCeiling() {
		return ceilingHitbox;
	}

	void update() {
		ceilingHitbox->updatePos();
	}

};