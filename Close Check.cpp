#include <iostream>
#include <stack>
#include "Render Logic.cpp"
#include <SFML/Graphics.hpp>
#pragma once

using namespace std;

static class CloseCheck {


public:
	static bool checkClosed(shared_ptr<renderer> instance) {
		sf::Event event;
		while (instance->getWindow()->getWindow()->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				return true;
		}

		return false;
	}

};