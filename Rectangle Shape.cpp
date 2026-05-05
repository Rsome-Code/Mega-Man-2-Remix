#include "Vector2f.cpp"
#include <SFML/graphics.hpp>
#include "IntRect.cpp"
#include "colour.h"
#pragma once

class RectangleShape{

	sf::RectangleShape thisOne;

public: 
	RectangleShape(Vector2f position, Vector2f size) {
		thisOne.setPosition(position.x, position.y);
		thisOne.setSize(sf::Vector2f(size.x, size.y));
	}

	RectangleShape (Vector2f size) {

		thisOne.setSize(sf::Vector2f(size.x, size.y));
	}

	RectangleShape(IntRect rect) {
		thisOne.setPosition(rect.getPosition().x, rect.getPosition().y);
		thisOne.setSize(sf::Vector2f(rect.getSize().x, rect.getSize().y));
	}

	RectangleShape() {

	}
	  
	void setFillColour(Colour c) {
		thisOne.setFillColor(sf::Color(c.red, c.green, c.blue, c.alpha));
	}

	std::shared_ptr<sf::RectangleShape> getShape() {
		return std::shared_ptr<sf::RectangleShape>(&this->thisOne);
	}

	void setScale(Vector2f scale) {
		thisOne.setScale(scale.x, scale.y);
	}

	Vector2f getScale() {
		return Vector2f(thisOne.getScale().x, thisOne.getScale().y);
	}

	Vector2f getPosition() {
		return Vector2f(thisOne.getPosition().x, thisOne.getPosition().y);
	}
	void setPosition(Vector2f pos) {
		thisOne.setPosition(pos.x, pos.y);
	}

	void setSize(Vector2f s) {
		thisOne.setSize(sf::Vector2f(s.x, s.y));
	}

	void setOutlineColour(Colour col) {
		thisOne.setOutlineColor(sf::Color(col.red, col.green, col.blue));
	}

	void setOutlineThickness(int n) {
		thisOne.setOutlineThickness(n);
	}

	Vector2f getSize() {
		return Vector2f(thisOne.getSize().x, thisOne.getSize().y);
	}
};