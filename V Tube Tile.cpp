#include "Flash Tile.cpp"
#pragma once

class VTubeTile : public FlashTile {

	shared_ptr<objectHitbox> leftHitbox;
	shared_ptr<objectHitbox> rightHitbox;

public:
	VTubeTile(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;

		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 32), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		leftHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite));
		rightHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(size, 4), Vector2i(1, 16)), true, sprite));

		type = "vertical flash";
	}


	shared_ptr<objectHitbox> getLeft() {
		return leftHitbox;
	}
	shared_ptr<objectHitbox> getRight() {
		return rightHitbox;
	}

	void update() {
		leftHitbox->updatePos();
		rightHitbox->updatePos();
	}

	virtual void deleteHits() {

	}
};