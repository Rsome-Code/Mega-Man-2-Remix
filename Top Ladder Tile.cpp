#include "Ladder Tile.cpp"
#pragma once
class topLadder:public ladderTile {


	shared_ptr<objectHitbox> groundHitbox;

public:
	topLadder(Vector2f loc, shared_ptr<Texture> t) {

		type = "9";

		location = loc;
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(128, 48), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));


		ladder = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 16)), true, sprite));
	
		groundHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite));
	}

	shared_ptr<objectHitbox> getGround() {
		return groundHitbox;
	}

	virtual void deleteInt() {

	}
};