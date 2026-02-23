#include "Flash Tile.cpp"
#pragma once

class BottomLeftWFlash : public FlashTile {

public:
	BottomLeftWFlash(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;

		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 224), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		type = "bottom left w flash";
	}

};