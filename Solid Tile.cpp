#include "Tile.cpp"
#include "Object Hitbox.cpp"
#pragma once
class solidTile:public tile {
protected:
	shared_ptr<objectHitbox> groundHitbox;
	shared_ptr<objectHitbox> leftHitbox;
	shared_ptr<objectHitbox> rightHitbox;
	shared_ptr<objectHitbox> ceilingHitbox;

public:
	solidTile() {}
	solidTile(Vector2f loc, shared_ptr<Texture> t, int tileNum) {
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;
		tileNumber = tileNum;
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));
		setTileNum(tileNum);

		groundHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite));
		ceilingHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite));
		leftHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite));
		rightHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(size, 4), Vector2i(1, 16)), true, sprite));

		type = "5";
	}


	shared_ptr<objectHitbox> getGround() {
		return groundHitbox;
	}
	shared_ptr<objectHitbox> getCeiling() {
		return ceilingHitbox;
	}
	shared_ptr<objectHitbox> getLeft() {
		return leftHitbox;
	}
	shared_ptr<objectHitbox> getRight() {
		return rightHitbox;
	}

	void update() {
		groundHitbox->updatePos();
		leftHitbox->updatePos();
		rightHitbox->updatePos();
		ceilingHitbox->updatePos();
	}

	virtual void deleteInt() {

	}
};