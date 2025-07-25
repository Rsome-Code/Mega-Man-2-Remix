#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <iterator>
#include <list>
#pragma once


using namespace std;
using namespace sf;


class UISprite {

protected:Sprite thisOne;
	Texture* texture;
	Vector2i tRect;
	Vector2i rectSize;
	Vector2f scale;
	Vector2f cameraPosition;
	string type;
	list<UISprite*>::iterator i;
	Vector2f cameraScale;



public: UISprite(string type, Texture* texture, Vector2i rect, Vector2i rectSize, Vector2f position, Vector2f scale) {
	this->texture = texture;
	this->type = type;
	loadTexture();
	setRect(rect, rectSize);
	setScale(scale);
	setCameraPosition(position);
}
public: UISprite(string type, Texture* texture, Vector2i rect, Vector2i rectSize, Vector2f position) {
	this->texture = texture;
	this->type = type;
	loadTexture();
	setRect(rect, rectSize);
	setCameraPosition(position);
	setScale(Vector2f(1, 1));
}
public: UISprite(string type, Texture* texture, IntRect rect, Vector2f position) {
	this->texture = texture;
	this->type = type;
	loadTexture();
	setRect(rect.getPosition(), rect.getSize());
	setCameraPosition(position);
	setScale(Vector2f(1, 1));
}
public: UISprite(string type, Texture* texture, IntRect rect, Vector2f position, Vector2f scale) {
	this->texture = texture;
	this->type = type;
	loadTexture();
	setRect(rect.getPosition(), rect.getSize());
	setCameraPosition(position);
	setScale(scale);
}
public: UISprite() {
	//cout << "huh?";
}
protected: void loadTexture() {
	thisOne.setTexture(*texture);
}
public: Texture* getTexture() {
	return texture;
}
	  void loadSprite() {
		  thisOne.setTextureRect(sf::IntRect(tRect, rectSize));
	  }

	  void setScale(Vector2f scale) {
		  this->scale = scale;
		  thisOne.setScale(scale);
	  }
	  Vector2f getSize() {
		  return Vector2f (rectSize.x * scale.x, rectSize.y * scale.y);
	  }
	  void setCameraPosition(Vector2f c) {
		  this->cameraPosition = c;
		  thisOne.setPosition(c);
	  }

public: void setRect(Vector2i r, Vector2i s) {
	this->tRect = r;
	this->rectSize = s;
	applyRect(IntRect(r, s));
}
public: void setRect(IntRect r) {
	this->tRect = r.getPosition();
	this->rectSize = r.getSize();
	applyRect(r);
}
public: void applyRect(IntRect rectangle) {
	tRect = rectangle.getPosition();
	rectSize = rectangle.getSize();
	thisOne.setTextureRect(rectangle);
}

public: Sprite* getSprite() {
	return &this->thisOne;
}
public: Vector2f getCameraPosition() {
	return cameraPosition;
}
public: Vector2f getScale() {
	return scale;
}
public: string getType() {
	return type;
}
	  virtual void setIterator(list<UISprite*>::iterator j) {
		  this->i = j;
	  }

	  void swap() {
		  int change = texture->getSize().x;
		  int sizeChange = rectSize.x;
		  setRect(Vector2i((change - tRect.x) - sizeChange , tRect.y), Vector2i(rectSize));
	  }
	  virtual void setVisualOffset(Vector2f off) {
		  
	  }

	  IntRect getRect() {
		  return IntRect(tRect, rectSize);
	  }

	  void setCameraScale(Vector2f s) {
		  cameraScale = s;
		  thisOne.setScale(s);
	  }

	  Vector2f getCameraScale() {
		  return cameraScale;
	  }

	  void setTexture(Texture* t) {
		  this->texture = t;
		  loadTexture();
	  }

};