#include "Flash Tile.cpp"
#pragma once

class TopLeftCornerFlash : public FlashTile {

	shared_ptr<objectHitbox> groundHitbox;
	shared_ptr<objectHitbox> leftHitbox;

public:
	TopLeftCornerFlash(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;

		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 0), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		groundHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite));

		leftHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite));


		type = "top left corner flash";
	}

	shared_ptr<objectHitbox> getGround() {
		return groundHitbox;
	}

	shared_ptr<objectHitbox> getLeft() {
		return leftHitbox;
	}


	void update() {
		groundHitbox->updatePos();
		leftHitbox->updatePos();
	}

	virtual void deleteHits() {
		


	}
};