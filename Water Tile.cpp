#include "tile.cpp"

#pragma once

class WaterTile : public tile {

	objectHitbox* waterHit;

public:
	WaterTile(Vector2f loc, Texture* t, int tileNum) {
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;

		tileNumber = tileNum;
		location = loc;
		sprite = new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);


		type = "water";
		waterHit = new objectHitbox(IntRect(0, 0, 16, 16), sprite);
	}

	objectHitbox* getWaterBox() {
		return waterHit;
	}
};