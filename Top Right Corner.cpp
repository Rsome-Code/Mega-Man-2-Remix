#include "Flash Tile.cpp"
#pragma once

class TopRightCornerFlash : public FlashTile {

	objectHitbox* groundHitbox;
	objectHitbox* rightHitbox;

public:
	TopRightCornerFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 48), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		groundHitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite);

		rightHitbox = new objectHitbox(IntRect(Vector2i(size, 4), Vector2i(1, 16)), true, sprite);

		type = "top right corner flash";
	}

	objectHitbox* getGround() {
		return groundHitbox;
	}


	objectHitbox* getRight() {
		return rightHitbox;
	}

	void update() {
		groundHitbox->updatePos();

		rightHitbox->updatePos();

	}
};