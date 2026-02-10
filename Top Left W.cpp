#include "Flash Tile.cpp"
#pragma once

class TopLeftWFlash : public FlashTile {



public:
	TopLeftWFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 176), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);



		type = "top left w flash";
	}

};