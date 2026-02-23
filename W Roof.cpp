#include "Flash Tile.cpp"
#pragma once

class WRoofFlash : public FlashTile {

	shared_ptr<objectHitbox> groundHitbox;

public:
	WRoofFlash(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;

		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 288), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		groundHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite));

		type = "w roof flash";
	}

	shared_ptr<objectHitbox> getGround() {
		return groundHitbox;
	}

	void update() {
		groundHitbox->updatePos();
	}

	virtual void deleteHits() {

	}
};