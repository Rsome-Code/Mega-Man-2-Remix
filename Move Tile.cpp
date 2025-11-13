#include "solid Tile.cpp"

#pragma once

class MoveTile : public solidTile {

	bool moveRight;

public:

	MoveTile(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(157, 145), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);
		groundHitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite);
		ceilingHitbox = new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite);
		leftHitbox = new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite);
		rightHitbox = new objectHitbox(IntRect(Vector2i(size, 4), Vector2i(1, 16)), true, sprite);

		
	}

	void setMoveRight(bool right) {
		moveRight = right;
		if (right) {
			sprite->setRect(IntRect(189, 145, 16, 16));
			type = "move-right";
		}
		else {
			type = "move-left";
			sprite->setRect(Vector2i(157, 145), Vector2i(16, 16));
		}
	}

	float getMovement() {
		if (moveRight) {
			return 300;
		}
		return -300;
	}
};