#include <SFML/Graphics.hpp>
#pragma once
using namespace sf;
using namespace std;

class text {
	Text* render;
	string content;
	float numContent;
	float size;
	Vector2f position;
	Font* font;
	const::Color* colour;

public:
	text(string content, Vector2f position, float size, Font* font, const::Color* colour) {
		this->content = content;
		this->position = position;
		this->size = size;
		this->font = font;
		this->colour = colour;

		render = new Text();
		render->setFont(*font);
		render->setCharacterSize(size); // in pixels
		render->setFillColor(*colour); // set the text color
		//render->setStyle(sf::Text::Bold | sf::Text::Underlined); // set text style
		render->setString(content);
		render->setPosition(position);
	}
	text(int content, Vector2f position, float size, Font* font, const::Color* colour) {
		numContent = content;
		this->content = to_string(content);
		this->position = position;
		this->size = size;
		this->font = font;
		this->colour = colour;

		render->setFont(*font);
		render->setCharacterSize(size); // in pixels
		render->setFillColor(*colour); // set the text color
		//render->setStyle(sf::Text::Bold | sf::Text::Underlined); // set text style
		render->setString(this->content);
		render->setPosition(position);
	}

	void setSize(float new1) {
		size = new1;
		render->setCharacterSize(size); // in pixels
	}
	float getSize() {
		return size;
	}
	Vector2f getPosition() {
		return position;
	}
	void setRenderSize(float s) {
		render->setCharacterSize(s);
	}

	Text* getRender() {
		return render;
	}

	void setContent(string con) {
		content = con;
		render->setString(this->content);
	}
	void setContent(int con) {
		numContent = con;
		this->content = to_string(con);
		render->setString(this->content);
	}


};