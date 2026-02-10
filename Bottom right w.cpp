#include "Flash Tile.cpp"
#pragma once

class BottomRightWFlash : public FlashTile {

public:
	BottomRightWFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 240), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		type = "bottom right w flash";
	}

};