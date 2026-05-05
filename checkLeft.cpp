#include "rail platform checkpoint.cpp"
#pragma once

class CheckLeft : public RailCheckpoint {
public:
	CheckLeft(Vector2f pos) {
		setCode("rail checkpoint-left");

		shared_ptr<Texture> tex = shared_ptr<Texture>(new Texture());
		tex->loadFromFile("assets/arrows.png");

		sprite = shared_ptr<objectSprite>(new objectSprite(tex, IntRect(66, 0, 5,5), pos, Vector2f(4, 4)));

		setup();


	}

};