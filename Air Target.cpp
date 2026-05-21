#include "Gobject.cpp"
#pragma once

class AirTarget : public GameObject {
public:

	AirTarget(Vector2f pos) {
		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture());
		t->loadFromFile("assets//air man.png");

		sprite = shared_ptr<objectSprite>(new objectSprite());
		sprite->setTexture(t);

		sprite->setRect(IntRect(3, 132, 12, 15));
		sprite->setScale(Vector2f(4, 4));
		code = "air target-0";
	}

	void eachFrame() {
		display = false;
	}
};