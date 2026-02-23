#include "Flash Tile.cpp"
#pragma once

class WWallFlash : public FlashTile {

	shared_ptr<objectHitbox> leftHitbox;

public:
	WWallFlash(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;

		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 208), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		leftHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite));

		type = "w wall flash";
	}

	shared_ptr<objectHitbox> getLeft() {
		return leftHitbox;
	}

	void update() {
		leftHitbox->updatePos();
	}
	
	virtual void deleteHits() {

	}
};