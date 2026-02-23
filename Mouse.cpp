#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <SFML/Window/Mouse.hpp>
#include "Render Logic.cpp"
#pragma once
using namespace std;
using namespace sf;

class mouse {
	Vector2i position;

	bool active;

public:
	Vector2i getPosition(shared_ptr<renderer> instance, Vector2f wSize) {
		Vector2i temp = Mouse::getPosition(*instance->getWindow());
		float xScale = 1920 / wSize.x;
		float yScale = 1080 / wSize.y;
		position = Vector2i(temp.x * xScale, temp.y * yScale);

		active = instance->getWindow()->hasFocus();

		return position;
	}

	

	bool checkActive() {
		if (position.x > 1920 || position.x < 0 || position.y > 1080 || position.y < 0 || !active) {
			return false;
		}

		return true;
		
	}


};