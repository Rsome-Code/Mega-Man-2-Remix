#include "lava tile.cpp"
#pragma once

class VerticalLava : public LavaTile {

public:
	VerticalLava(Vector2f loc, shared_ptr<Texture> t, float z) {
		this->z = z;

		location = loc;

		location = loc;
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(80, 0), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));

		deathBox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), sprite));

		type = 11;
		animSetup(3);
	}
};