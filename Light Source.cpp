#include <SFML/Graphics.hpp>
#include "colour.cpp"
#pragma once

using namespace sf;
using namespace std;

class LightSource {
	Colour colour;
	float range;
	float brightness;
	Vector2f position;


public:
	LightSource(Colour col, float range, float brightness) {
		colour = col;
		this->range = range;
		this->brightness = brightness;
	}

	void updatePos(Vector2f pos) {
		position = pos;
	}
	Colour getColour() {
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