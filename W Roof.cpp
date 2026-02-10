#include "Flash Tile.cpp"
#pragma once

class WRoofFlash : public FlashTile {

	objectHitbox* groundHitbox;

public:
	WRoofFlash(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 288), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		groundHitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite);

		type = "w roof flash";
	}

	objectHitbox* getGround() {
		return groundHitbox;
	}

	void update() {
		groundHitbox->updatePos();
	}

};