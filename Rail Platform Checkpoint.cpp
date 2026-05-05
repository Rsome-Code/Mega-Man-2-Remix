#include "gObject.cpp"
#pragma once

class RailCheckpoint :public GameObject {

protected:


	shared_ptr<objectHitbox> hit;

public:
	void setup() {
		shared_ptr<Texture> tex = shared_ptr<Texture>(new Texture());
		tex->loadFromFile("assets/arrows.png");
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 5, 5), sprite));

	}

	void eachFrame() {
		display = false;
	}

	shared_ptr<objectHitbox> getHitbox() {
		return hit;
	}
};