#include <SFML/Graphics.hpp>
#pragma once

using namespace sf;
using namespace std;

class LightSource {
	Color colour;
	float range;
	float brightness;
	Vector2f position;


public:
	LightSource(Color col, float range, float brightness) {
		colour = col;
		this->range = range;
		this->brightness = brightness;
	}

	void updatePos(Vector2f pos) {
		position = pos;
	}
	Color getColour() {
		return colour;
	}
	float getRange() {
		return range;
	}
	Vector2f getPosition() {
		return position;
	}
	float getBrightness() {
		return brightness;
	}
};