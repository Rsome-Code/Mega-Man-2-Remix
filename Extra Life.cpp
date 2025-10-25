#include "item.cpp"
#pragma once

class ExtraLife : public Item {

public:
	ExtraLife(Texture* t, Vector2f position) {
		phys = new physicsObject("Extra Life", t, IntRect(139, 2, 16, 15), position, Vector2f(4, 4));
		sprite = phys;
		hit = new objectHitbox(IntRect(0, 0, 16, 15), sprite);
		setCode();
	}

	void setCode() {
		code = "Extra Life";
	}




	int getIncrease() { return 1; }
};