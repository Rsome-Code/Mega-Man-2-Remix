#include "Tile.cpp"
#pragma once

class rightTile : public tile {

	objectHitbox* rightHitbox;
public:
	rightTile(Vector2f loc, Texture* t, int tileNum) {
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;

		tileNumber = tileNum;
		location = loc;
		sprite = new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);
		rightHitbox = new objectHitbox(IntRect(Vector2i(size, 4), Vector2i(1, 16)), true, sprite);
	}

	objectHitbox* getRight() {
		return rightHitbox;
	}

	void update() {
		rightHitbox->updatePos();
	}
};