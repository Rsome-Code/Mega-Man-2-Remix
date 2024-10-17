#include "tile.cpp"
#pragma once
class ladderTile :public tile {

protected:
	objectHitbox* ladder;

public:
	ladderTile(){}
	ladderTile(Vector2f loc, Texture* t) {
		location = loc;
		sprite = new objectSprite("Tile", t, Vector2i(128, 48), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);
		ladder = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 16)), true, sprite);
	}

	objectHitbox* getLadder() {
		return ladder;
	}
};