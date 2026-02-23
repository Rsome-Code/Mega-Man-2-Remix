#include "flash tile.cpp"
#pragma once

class HTubeTile : public FlashTile {

	shared_ptr<objectHitbox> groundHitbox;

	shared_ptr<objectHitbox> ceilingHitbox;

public:
	HTubeTile(Vector2f loc, shared_ptr<Texture> t) {
		location = loc;

		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		groundHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite));
		ceilingHitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, size), Vector2i(16, 1)), true, sprite));

		type = "horizontal flash";
		
	}

	shared_ptr<objectHitbox> getGround() {
		return groundHitbox;
	}
	shared_ptr<objectHitbox> getCeiling() {
		return ceilingHitbox;
	}


	void update() {
		groundHitbox->updatePos();

		ceilingHitbox->updatePos();
	}

	virtual void deleteHits() {

	}
	
};