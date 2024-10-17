#include "tile.cpp"
#pragma once

class leftTile : public tile {

	objectHitbox* leftHitbox;
public:
	leftTile(Vector2f loc, Texture* t, int tileNum) {
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;


		location = loc;
		sprite = new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);
		leftHitbox = new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite);
	}

	objectHitbox* getLeft() {
		return leftHitbox;
	}

	void update() {
		leftHitbox->updatePos();
	}
};