#include "lava tile.cpp"
#pragma once

class HorizontalLava : public LavaTile {

public:
	HorizontalLava(Vector2f loc, shared_ptr<Texture> t, float z) {
		this->z = z;

		location = loc;

		location = loc;
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 0), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		deathBox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), sprite));

		type = 10;

		animSetup(3);
	}
};