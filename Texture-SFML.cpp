#include <SFML/Graphics.hpp>
#include "texture-abstract.cpp"
#include <iostream>
#pragma once

using namespace std;

class Texture: public textureControl {
	using textureControl::textureControl;

	sf::Texture texture;

	virtual void initSize() {
		this->size = Vector2i(texture.getSize().x, texture.getSize().y);
	}

	virtual void loadTexture() {
		texture.loadFromFile(this->path);
	}

public:

	sf::Texture* getTexture() {
		return &this->texture;
	}
	
};