#include "flash tile.cpp"
#pragma once

class HTubeTile : public FlashTile {

	objectHitbox* groundHitbox;

	objectHitbox* ceilingHitbox;

public:
	HTubeTile(Vector2f loc, Texture* t) {
		location = loc;

		sprite = new objectSprite("Tile", t, Vector2i(64, 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		groundHitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite);
		ceilingHitbox = new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite);

		type = "horizontal flash";
		
	}

	objectHitbox* getGround() {
		return groundHitbox;
	}
	objectHitbox* getCeiling() {
		return ceilingHitbox;
	}


	void update() {
		groundHitbox->updatePos();

		ceilingHitbox->updatePos();
	}
	
};