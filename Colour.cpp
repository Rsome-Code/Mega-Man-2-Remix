#include <SFML/graphics.hpp>
#pragma once

class Colour {

public:
	int red;
	int blue;
	int green;
	int alpha = 255;

	int r;
	int b;
	int g;
	int a;

	sf::Color col;

private:
	void setLetters() {
		r = red;
		b = blue;
		g = green;
		a = alpha;
	}

public:
	Colour(int r, int g, int b) {
		red = r;
		green = g;
		blue = b;

		setLetters();
	}
	Colour(int r, int g, int b, int a) {
		red = r;
		green = g;
		blue = b;
		alpha = a;
		setLetters();
	}
	Colour() {
		red = 0;
		green = 0;
		blue = 0;
		setLetters();
	}

	Colour(int i) {
		//Do opposite of toInteger()

		sf::Color colo = sf::Color(i);
		red = colo.r;
		green = colo.g;
		blue = colo.b;
		alpha = colo.a;
		setLetters();
	}

	static Colour White() {
		return Colour(255, 255, 255, 255);
	}



	static Colour Black() {
		return Colour(0, 0, 0, 255);
	}

	static Colour Red() {
		return Colour(255, 0, 0, 255);
	}

	static Colour Green() {
		return Colour(0, 255, 0);
	}

	static Colour Blue() {
		return Colour(0, 0, 255);
	}

	static Colour Cyan() {
		//return Colour(sf::Color::Cyan.r, sf::Color::Cyan.g, sf::Color::Cyan.b);
		return Colour(0, 255, 255);
	}

	static const Colour Yellow;      //!< Yellow predefined color
	static const Colour Magenta;     //!< Magenta predefined color
	static const Colour Transparent; //!< Transparent (black) predefined color

	int toInteger() {
		//Convert red, green, blue, alpha to binary
		//Then order them as above and translate to a single integer

		int c = r;
		c = (c << 8) | g;
		c = (c << 8) | b;
		c = (c << 8) | a;
		return c;
	}


	sf::Color getColour() {
		return sf::Color(red, green, blue, alpha);
	}

};