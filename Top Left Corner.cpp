#include "Flash Tile.cpp"
#pragma once

class TopLeftCornerFlash : public FlashTile {

	objectHitbox* groundHitbox;
	objectHitbox* leftHitbox;

public:
	TopLeftCornerFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 0), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		groundHitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite);

		leftHitbox = new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite);


		type = "top left corner flash";
	}

	objectHitbox* getGround() {
		return groundHitbox;
	}

	objectHitbox* getLeft() {
		return leftHitbox;
	}


	void update() {
		groundHitbox->updatePos();
		leftHitbox->updatePos();
	}
};