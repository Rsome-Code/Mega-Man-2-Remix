#include "tile.cpp"
#pragma once

class DeathTile : public tile {
protected:
	objectHitbox* deathBox;



public:

	DeathTile(){}

	DeathTile(Vector2f loc, Texture* t, int tileNum, float z) {
		this->z = z;
		tileNumber = tileNum;
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;


		location = loc;
		sprite = new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		deathBox = new objectHitbox(IntRect(0, 0, 16, 16), sprite);

		type = "death";
	}


	objectHitbox* getDeathBox() {
		return deathBox;
	}

};