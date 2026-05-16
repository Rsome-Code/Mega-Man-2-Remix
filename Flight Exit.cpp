#include "Gobject.cpp"
#pragma once

class FlightExit :public GameObject {

public:
	FlightExit(Vector2f pos) {
		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture());
		t->loadFromFile("assets/flight exit.png");
		sprite = shared_ptr<objectSprite>(new objectSprite(t, IntRect(0, 0, 33, 27), pos, Vector2f(4, 4)));
		code = "flight exit";
	}

	void eachFrame() {
		display = false;
	}
};