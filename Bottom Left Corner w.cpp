#include "Flash Tile.cpp"
#pragma once

class BottomLeftCornerWFlash : public FlashTile {


	shared_ptr<objectHitbox> leftHitbox;

	shared_ptr<objectHitbox> ceilingHitbox;

public:
	BottomLeftCornerWFlash(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;

		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 256), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		ceilingHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite));
		leftHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite));


		type = "bottom left corner w flash";
	}


	shared_ptr<objectHitbox> getCeiling() {
		return ceilingHitbox;
	}
	shared_ptr<objectHitbox> getLeft() {
		return leftHitbox;
	}


	void update() {

		leftHitbox->updatePos();

		ceilingHitbox->updatePos();
	}

	virtual void deleteHits() {
		
	}
};