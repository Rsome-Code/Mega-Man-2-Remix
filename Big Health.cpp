#include "Item.cpp"

#pragma once

class BigHealth : public Item {
	float increase = 6;

public:

	BigHealth(Texture* t, Vector2f pos) {
		sprite = new objectSprite("health", t, IntRect(19, 1, 16, 16), pos, Vector2f(4,4), 1);
		anim = new animation(list<IntRect>{IntRect(19, 1, 16, 16), IntRect(36, 1, 16, 16)}, sprite);
		timer = new animTimer(anim, 10, true);
		hit = new objectHitbox(IntRect(0, 0, 16, 16), false, sprite);
	}

	int getIncrease() {
		return increase;
	}

	void setCode() {
		code = "health-big";
	}

	void eachFrame(float* deltaT, objectSprite* p) {
		timer->run(deltaT);
	}
};