#include "Item.cpp"

#pragma once

class BigAmmo : public Item {
	float increase = 6;

public:

	BigAmmo(Texture* t, Vector2f pos) {
		sprite = new objectSprite("ammo", t, IntRect(71, 3, 16, 12), pos, Vector2f(4, 4), 1);
		anim = new animation(list<IntRect>{IntRect(71, 3, 16, 12), IntRect(88, 3, 16, 12)}, sprite);
		timer = new animTimer(anim, 10, true);
		hit = new objectHitbox(IntRect(0, 0, 16, 12), false, sprite);
	}

	int getIncrease() {
		return increase;
	}

	void setCode() {
		code = "ammo-big";
	}

	void eachFrame(float* deltaT, objectSprite* p) {
		timer->run(deltaT);
	}
};