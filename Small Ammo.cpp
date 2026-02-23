#include "Item.cpp"

#pragma once

class SmallAmmo : public Item {
	float increase = 2;

public:

	SmallAmmo(shared_ptr<Texture> t, Vector2f pos) {
		phys = shared_ptr<physicsObject> (new physicsObject("ammo", t, IntRect(53, 5, 8,8), pos, Vector2f(4, 4), 1));
		sprite = phys;
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(53, 5, 8, 8), IntRect(62, 5, 8, 8)}, sprite));
		timer = shared_ptr<animTimer> (new animTimer(anim, 10, true));
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 12), false, sprite));
	}

	int getIncrease() {
		return increase;
	}

	void setCode() {
		code = "ammo-small";
	}


};