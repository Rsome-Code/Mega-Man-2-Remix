#include "item.cpp"
#pragma once

class ExtraLife : public Item {

public:
	ExtraLife(shared_ptr<Texture> t, Vector2f position) {
		phys = shared_ptr<physicsObject> (new physicsObject("Extra Life", t, IntRect(139, 2, 16, 15), position, Vector2f(4, 4)));
		sprite = phys;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 15), sprite));
		setCode();
	}

	void setCode() {
		code = "Extra Life";
	}




	int getIncrease() { return 1; }
};