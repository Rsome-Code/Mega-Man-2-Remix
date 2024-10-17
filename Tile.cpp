#include "Object Sprite.cpp"
#include "Object Hitbox.cpp"
#include "Object.cpp"
#pragma once

class tile:public object {
protected:
	
	Vector2f location;
	float size = 16 * 4;
	float z;
	int tileNumber;


public:
	tile() {

	}
	tile(Vector2f loc, Texture* t, int tileNum, float z) {
		this->z = z;
		tileNumber = tileNum;
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;
		

		location = loc;
		sprite = new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);
	}

	objectSprite* getSprite(){
		return sprite;
	}

	Vector2f getLocation() {
		return location;
	}

	int getTileNum() {
		return tileNumber;
	}
	void setTileNum(int i) {
		int tY = i / 4;
		int tX = i % 4;
		tileNumber = i;


		sprite ->setRect(Vector2i(tX * 16, tY * 16), Vector2i(16, 16));
	}

	float getZ() {
		return z;
	}

	virtual objectHitbox* getGround() { return NULL; };
	virtual objectHitbox* getCeiling() { return NULL; };
	virtual objectHitbox* getLeft() { return NULL; };
	virtual objectHitbox* getRight() { return NULL; };
	virtual objectHitbox* getLadder() { return NULL; };

	virtual void update() {};
};