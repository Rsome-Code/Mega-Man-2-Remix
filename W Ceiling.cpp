#include "Flash Tile.cpp"
#pragma once

class WCeilingFlash : public FlashTile {

	shared_ptr<objectHitbox> ceilingHitbox;

public:
	WCeilingFlash(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;

		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 272), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		ceilingHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite));

		type = "w ceiling flash";
	}

	shared_ptr<objectHitbox> getCeiling() {
		return ceilingHitbox;
	}

	void update() {
		ceilingHitbox->updatePos();
	}

	virtual void deleteHits() {
		
	}

};