#include <SFML/Graphics.hpp>
#include "Colour.cpp"
#pragma once
using namespace sf;
using namespace std;

class text {
	shared_ptr<Text> render;
	string content;
	float numContent;
	float size;
	Vector2f position;
	shared_ptr<Font> font;
	Colour colour;

public:

	virtual ~text() {

		//delete font;
		//delete colour;
	}

	text(string content, Vector2f position, float size, shared_ptr<Font> font, Colour colour) {
		this->content = content;
		this->position = position;
		this->size = size;
		this->font = font;
		this->colour = colour;

		render = shared_ptr<Text> (new Text());
		render->setFont(*font);
		render->setCharacterSize(size); 
		render->setFillColor(colour.getColour()); 
		
		render->setString(content);
		render->setPosition(position);
	}
	text(int content, Vector2f position, float size, shared_ptr<Font> font, Colour colour) {
		numContent = content;
		this->content = to_string(content);
		this->position = position;
		this->size = size;
		this->font = font;
		this->colour = colour;

		render->setFont(*font);
		render->setCharacterSize(size); // in pixels
		render->setFillColor(colour.getColour()); // set the text color
		//render->setStyle(sf::Text::Bold | sf::Text::Underlined); // set text style
		render->setString(this->content);
		render->setPosition(position);
	}

	text() {
		render = shared_ptr<Text>(new Text());
	}

	void setLineSpacing(float newLineSpacing) {
		render->setLineSpacing(newLineSpacing);
	}

	void setFillColour(Colour newColour) {
		colour = newColour;
		render->setFillColor(colour.getColour());
	}

	void setFont(shared_ptr<Font> newFont) {
		font = newFont;
		render->setFont(*font);
	}

	shared_ptr<Font> getFont() {
		return font;
	}

	string getString(){
		return content;
	}

	void setString(string newString) {
		content = newString;
		render->setString(content);
	}

	void setPosition(Vector2f newPos) {
		position = newPos;
		render->setPosition(position);
	}

	void setSize(float new1) {
		size = new1;
		render->setCharacterSize(size); // in pixels
	}

	void setScale(Vector2f sc) {
		render->setScale(sc);
	}

	Vector2f getScale() {
		return render->getScale();
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

	shared_ptr<Text> getRender() {
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

	FloatRect getRect() {
		return render->getGlobalBounds();
	}

};