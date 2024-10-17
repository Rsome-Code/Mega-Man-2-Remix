#include <SFML/Graphics.hpp>
#include <iostream>
#pragma once

using namespace std;

class textureControl {
	string path;
	sf::Vector2u size;
	public:
		sf::Texture texture;

	textureControl() {
		cout << "HUH?";
	}
	textureControl(string path) {
		this->path = path;
		loadTexture();
		setSize();
	}

	void loadTexture(){
		if (!texture.loadFromFile("Assets\\NES - Final Fantasy - Light Warriors.png"))
		{
			cout << "Error";
		}

	}
	void setTexture(string text) {
		this->path = text;
		loadTexture();
		setSize();
	}
	void setSize() {
		this->size = texture.getSize();
	}

	sf::Texture getTexture(){
		return texture;
	}
public:string getPath() {
	return this->path;
}
	
};