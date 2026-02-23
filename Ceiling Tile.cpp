#include "Tile.cpp"
#pragma once

class ceilingTile : public tile {

	shared_ptr<objectHitbox> ceilingHitbox;
public:
	ceilingTile(Vector2f loc, shared_ptr<Texture> t, int tileNum) {
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;
		tileNumber = tileNum;
		location = loc;
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));
		setTileNum(tileNum);
		ceilingHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite));

		type = "3";
	}

	shared_ptr<objectHitbox> getCeiling() {
		return ceilingHitbox;
	}

	void update() {
		ceilingHitbox->updatePos();
	}

	virtual void deleteInt() {

	}
};