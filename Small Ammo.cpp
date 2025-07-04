#include "Item.cpp"

#pragma once

class SmallAmmo : public Item {
	float increase = 2;

public:

	SmallAmmo(Texture* t, Vector2f pos) {
		sprite = new objectSprite("ammo", t, IntRect(53, 5, 8,8), pos, Vector2f(4, 4), 1);
		anim = new animation(list<IntRect>{IntRect(53, 5, 8, 8), IntRect(62, 5, 8, 8)}, sprite);
		timer = new animTimer(anim, 10, true);
		hit = new objectHitbox(IntRect(0, 0, 16, 12), false, sprite);
	}

	int getIncrease() {
		return increase;
	}

	void setCode() {
		code = "ammo-small";
	}

	void eachFrame(float* deltaT, objectSprite* p) {
		timer->run(deltaT);
	}
};