#include "anim tile.cpp"
#pragma once

class WaterAnim : public AnimTile {

	shared_ptr<objectHitbox> waterHit;

public:
	WaterAnim(Vector2f loc, shared_ptr<Texture> t, int VNumber, int animNumber, int z) {
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(64, 16 * VNumber), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));
		location = loc;

		list<IntRect> animList = { IntRect(64, 16 * VNumber, 16, 16) };
		for (int i = 1; i < animNumber; i++) {
			animList.push_back(IntRect(64 + (16 * i), 16 * VNumber, 16, 16));
		}
		anim = shared_ptr<animation>(new animation(animList, sprite));
		timer = shared_ptr<animTimer> (new animTimer(anim, 8, true));

		type = "water-" + to_string(VNumber);

		this->z = z;
		
		waterHit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), sprite));
	}

	shared_ptr<objectHitbox> getWaterBox() {
		return waterHit;
	}
};
