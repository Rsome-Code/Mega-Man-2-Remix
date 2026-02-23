#include "Item.cpp"

#pragma once

class BigHealth : public Item {
	float increase = 8;

public:

	BigHealth(shared_ptr<Texture> t, Vector2f pos) {
		phys = shared_ptr<physicsObject> (new physicsObject("health", t, IntRect(19, 1, 16, 16), pos, Vector2f(4,4), 1));
		sprite = phys;
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(19, 1, 16, 16), IntRect(36, 1, 16, 16)}, sprite));
		timer = shared_ptr<animTimer> (new animTimer(anim, 10, true));
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), false, sprite));
		setCode();
	}

	int getIncrease() {
		return increase;
	}

	void setCode() {
		code = "health-big";
	}


};