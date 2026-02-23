#include "tile.cpp"
#pragma once

class leftTile : public tile {

	shared_ptr<objectHitbox> leftHitbox;
public:
	leftTile(Vector2f loc, shared_ptr<Texture> t, int tileNum) {
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;

		tileNumber = tileNum;
		location = loc;
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));
		setTileNum(tileNum);
		leftHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite));

		type = "4";
	}

	shared_ptr<objectHitbox> getLeft() {
		return leftHitbox;
	}

	void update() {
		leftHitbox->updatePos();
	}

	void deleteInt() {


	}


};