#include "Flash Tile.cpp"
#pragma once

class BottomRightCornerFlash : public FlashTile {

	shared_ptr<objectHitbox> rightHitbox;
	shared_ptr<objectHitbox> ceilingHitbox;
public:
	BottomRightCornerFlash(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;

		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 80), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));


		ceilingHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite));

		rightHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(size, 4), Vector2i(1, 16)), true, sprite));

		type = "bottom right corner flash";
	}


	shared_ptr<objectHitbox> getCeiling() {
		return ceilingHitbox;
	}

	shared_ptr<objectHitbox> getRight() {
		return rightHitbox;
	}

	void update() {

		rightHitbox->updatePos();
		ceilingHitbox->updatePos();
	}

	virtual void deleteHits() {

	}
};