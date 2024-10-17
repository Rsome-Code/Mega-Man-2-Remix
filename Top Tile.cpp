#include "Tile.cpp"
#pragma once


class topTile: public tile {
	
	objectHitbox* groundHitbox;
public:
	topTile(Vector2f loc, Texture* t, int tileNum) {
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;

		
		location = loc;
		sprite = new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);
		groundHitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite);
	}

	objectHitbox* getGround() {
		return groundHitbox;
	}

	void update() {
		groundHitbox->updatePos();
	}
};