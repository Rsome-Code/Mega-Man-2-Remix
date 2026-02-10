#include "Flash Tile.cpp"
#pragma once

class TopRightWFlash : public FlashTile {

public:
	TopRightWFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 192), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		type = "top right w flash";
	}

};