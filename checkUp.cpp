#include "rail platform checkpoint.cpp"
#pragma once

class CheckUp : public RailCheckpoint {
public:
	CheckUp(Vector2f pos) {
		setCode("rail checkpoint-up");

		shared_ptr<Texture> tex = shared_ptr<Texture>(new Texture());
		tex->loadFromFile("assets/arrows.png");

		sprite = shared_ptr<objectSprite>(new objectSprite(tex, IntRect(99, 0, 5,5), pos, Vector2f(4, 4)));

		setup();


	}

};