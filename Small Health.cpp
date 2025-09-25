#include "Item.cpp"
#pragma once

class SmallHealth : public Item {
	float increase = 2;

public:

	SmallHealth(Texture* t, Vector2f pos) {
		phys = new physicsObject("health", t, IntRect(1, 5, 8,8), pos, Vector2f(4, 4), 1);
		sprite = phys;
		anim = new animation(list<IntRect>{IntRect(1, 5, 8,8), IntRect(10, 5, 8,8)}, sprite);
		timer = new animTimer(anim, 10, true);
		hit = new objectHitbox(IntRect(0, 0, 8, 8), false, sprite);
	}

	int getIncrease() {
		return increase;
	}

	void setCode() {
		code = "health-small";
	}


};