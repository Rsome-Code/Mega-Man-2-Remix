#include <iostream>
#include <stack>
#include "Render Logic.cpp"
#include <SFML/Graphics.hpp>

using namespace std;

static class CloseCheck {



	static bool checkClosed(renderer* instance) {
		sf::Event event;
		while (instance->getWindow()->getWindow()->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				return true;
		}

		return false;
	}

};