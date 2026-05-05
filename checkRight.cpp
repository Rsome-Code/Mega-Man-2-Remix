#include "rail platform checkpoint.cpp"
#pragma once

class CheckRight : public RailCheckpoint {
public:
	CheckRight(Vector2f pos) {
		setCode("rail checkpoint-right");

		shared_ptr<Texture> tex = shared_ptr<Texture>(new Texture());
		tex->loadFromFile("assets/arrows.png");

		sprite = shared_ptr<objectSprite>(new objectSprite(tex, IntRect(0, 0, 5,5), pos, Vector2f(4, 4)));

		setup();


	}

};