#include "Gobject.cpp"
#pragma once

class AirTarget2 : public GameObject {
public:

	AirTarget2(Vector2f pos) {
		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture());
		t->loadFromFile("assets//air man.png");

		sprite = shared_ptr<objectSprite>(new objectSprite());
		sprite->setTexture(t);

		sprite->setRect(IntRect(39, 131, 12, 16));
		sprite->setScale(Vector2f(4, 4));
		code = "air target-2";
	}


	void eachFrame() {
		display = false;
	}

};