#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <iterator>
#include <list>
#include "colour.cpp"
#pragma once


using namespace std;
using namespace sf;


class UISprite {

protected:Sprite thisOne;
	shared_ptr<Texture> texture;
	Vector2i tRect;
	Vector2i rectSize;
	Vector2f scale;
	Vector2f cameraPosition;
	string type;
	list<shared_ptr<UISprite>>::iterator i;
	Vector2f cameraScale = Vector2f(1,1);

	Vector2f visualOffset = Vector2f(0,0);

	


public: UISprite(string type, shared_ptr<Texture> texture, Vector2i rect, Vector2i rectSize, Vector2f position, Vector2f scale) {
	this->texture = texture;
	this->type = type;
	loadTexture();
	setRect(rect, rectSize);
	setScale(scale);
	setCameraPosition(position);
	setCameraScale(scale);
}
public: UISprite(shared_ptr<Texture> texture) {
	this->texture = texture;
	this->type = "eh";
	loadTexture();
	Vector2i size = Vector2i(texture->getSize());
	setRect(Vector2i(0,0), size);
	setScale(Vector2f(1,1));
	setCameraPosition(Vector2f(0,0));
	setCameraScale(scale);
}
public: UISprite(string type, shared_ptr<Texture> texture, Vector2i rect, Vector2i rectSize, Vector2f position) {
	this->texture = texture;
	this->type = type;
	loadTexture();
	setRect(rect, rectSize);
	setCameraPosition(position);
	setScale(Vector2f(1, 1));
	setCameraScale(scale);
}
public: UISprite(string type, shared_ptr<Texture> texture, IntRect rect, Vector2f position) {
	this->texture = texture;
	this->type = type;
	loadTexture();
	setRect(rect.getPosition(), rect.getSize());
	setCameraPosition(position);
	setScale(Vector2f(1, 1));
	setCameraScale(scale);
}
public: UISprite(string type, shared_ptr<Texture> texture, IntRect rect, Vector2f position, Vector2f scale) {
	this->texture = texture;
	this->type = type;
	loadTexture();
	setRect(rect.getPosition(), rect.getSize());
	setCameraPosition(position);
	setScale(scale);
	setCameraScale(scale);
}
public: UISprite(shared_ptr<Texture> texture, IntRect rect, Vector2f position, Vector2f scale) {
	this->texture = texture;

	loadTexture();
	setRect(rect.getPosition(), rect.getSize());
	setCameraPosition(position);
	setScale(scale);
	setCameraScale(scale);
}
public: UISprite() {
	//cout << "huh?";
}

	  UISprite(shared_ptr<UISprite> copy) {
		  this->texture = copy->getTexture();
		  this->type = copy->getType();
		  loadTexture();
		  setRect(copy->getRect());
		  setCameraPosition(copy->getCameraPosition());
		  setScale(copy->getScale());
		  setCameraScale(scale);
	  }

protected: void loadTexture() {
	thisOne.setTexture(*texture);
}
public: shared_ptr<Texture> getTexture() {
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

	  Vector2f getCameraSize() {
		  return Vector2f(rectSize.x * cameraScale.x, rectSize.y * cameraScale.y);
	  }

	  void setCameraPosition(Vector2f c) {
		  this->cameraPosition = c;
		  Vector2i actCamPos = Vector2i(c);
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


	  void setTransparency(int tra) {
		  thisOne.setColor(Colour(thisOne.getColor().r, thisOne.getColor().g, thisOne.getColor().g, tra).getColour());
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
	  virtual void setIterator(list<shared_ptr<UISprite>>::iterator j) {
		  this->i = j;
	  }

	  void swap() {
		  int change = texture->getSize().x;
		  int sizeChange = rectSize.x;
		  setRect(Vector2i((change - tRect.x) - sizeChange , tRect.y), Vector2i(rectSize));
	  }
	  virtual void setVisualOffset(Vector2f off) {
		  visualOffset = off;
	  }

	  virtual Vector2f getVisualOffset() {
		  return visualOffset;
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

	  void setTexture(shared_ptr<Texture> t) {
		  this->texture = t;
		  loadTexture();
	  }

	  void deleteStuff() {
		  delete &thisOne;
		  
		  delete &tRect;
		  delete& rectSize;
		  delete& scale;
		  delete& cameraPosition;
		  delete& type;
		  delete& i;
		  delete& cameraScale;

		  delete& visualOffset;
	  }

};