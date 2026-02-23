#include "tile.cpp"
#pragma once
class ladderTile :public tile {

protected:
	shared_ptr<objectHitbox> ladder;

public:
	ladderTile(){}
	ladderTile(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(128, 48), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		ladder = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 16)), true, sprite));
		type = "8";
	}

	shared_ptr<objectHitbox> getLadder() {
		return ladder;
	}

	void deleteInt() {

	}
};