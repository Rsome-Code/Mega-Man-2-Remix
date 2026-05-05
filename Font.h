#include <SFML/graphics.hpp>
#pragma once

class Font {

	sf::Font font;

public:
	Font() {

	}

	Font(std::string path) {
		font.loadFromFile(path);
	}
	
	void setPath(std::string path) {
		font.loadFromFile(path);
	}
	void loadFromFile(std::string path) {
		font.loadFromFile(path);
	}

	sf::Font getFont() {
		return font;
	}
};