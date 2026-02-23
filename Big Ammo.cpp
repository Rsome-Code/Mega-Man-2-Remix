#include "Item.cpp"

#pragma once

class BigAmmo : public Item {
	float increase = 8;

public:

	BigAmmo(shared_ptr<Texture> t, Vector2f pos) {
		phys = shared_ptr<physicsObject> (new physicsObject("ammo", t, IntRect(71, 3, 16, 12), pos, Vector2f(4, 4), 1));
		sprite = phys;
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(71, 3, 16, 12), IntRect(88, 3, 16, 12)}, sprite));
		timer = shared_ptr<animTimer> (new animTimer(anim, 10, true));
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 12), false, sprite));
		setCode();
	}

	int getIncrease() {
		return increase;
	}

	void setCode() {
		code = "ammo-big";
	}


};