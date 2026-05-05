#include "Vector2f.cpp"
#include <SFML/graphics.hpp>
#include "IntRect.cpp"
#include "colour.h"

class RectangleShape {

	sf::RectangleShape thisOne;

public:
	RectangleShape(Vector2f position, Vector2f size) {
		thisOne.setPosition(position.x, position.y);
		thisOne.setSize(sf::Vector2f(size.x, size.y));
	}
	RectangleShape(IntRect rect) {
		thisOne.setPosition(rect.getPosition().x, rect.getPosition().y);
		thisOne.setSize(sf::Vector2f(rect.getSize().x, rect.getSize().y));
	}

	void setColour(Colour c) {
		thisOne.setFillColor(sf::Color(c.red, c.green, c.blue, c.alpha));
	}

	sf::RectangleShape* getShape() {
		return &this->thisOne;
	}

	void setScale(Vector2f scale) {
		thisOne.setScale(scale.x, scale.y);
	}
};