#include <SFML/graphics.hpp>
#include "Vector2f.cpp"
#include "abstract window.cpp"
#include "text.cpp"
#include "rectangle shape.cpp"
#pragma once

class RenderWindow: public AbstractWindow {

	std::shared_ptr<sf::RenderWindow> window;

public:

	RenderWindow() {
		window = std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow());
	}

	RenderWindow(Vector2f size, std::string name, Type type) {
		if (type == windowed) {
			window = std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(size.x, size.y), "Executable", sf::Style::Default));
		}
		else if (type == boarderlessFullscreen) {
			window = std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(size.x, size.y), "Executable", sf::Style::Fullscreen));
		}
		else if (type == fullscreen) {
			window = std::shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(size.x, size.y), "Executable", sf::Style::Fullscreen));
		}
	}

	std::shared_ptr <sf::RenderWindow> getWindow() {
		return window;
	}

	Vector2i getSize() {
		return Vector2i(window->getSize().x, window->getSize().y);
	}

	void close() {
		window->close();
	}

	void draw(shared_ptr<sf::Text> t) {
		window->draw(*t);
	}

	void draw(sf::Sprite* s) {
		window->draw(*s);
	}

	void draw(shared_ptr<sf::RectangleShape> re) {
		window->draw(*re);
	}
	void draw(shared_ptr<RectangleShape> re) {
		window->draw(*re->getShape());
	}
	

	void display() {
		window->display();
	}
	void clear() {
		window->clear();
	}

	bool isOpen() {
		return window->isOpen();
	}

	bool hasFocus() {
		return window->hasFocus();
	}
};