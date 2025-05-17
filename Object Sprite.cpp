#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <iterator>
#include "UI Sprite.cpp"


#pragma once


using namespace std;
using namespace sf;



class objectSprite:public UISprite{

	//textureControl textureList[64];


protected:
	Vector2f position;
	float zAxis;
	Image image;
	bool flipped = false;
	bool flipPos = false;
	Vector2f visualOffset = Vector2f(0, 0);


	list<RectangleShape*> pixels;

public: 
	objectSprite(string type, Texture* texture, Vector2i rect, Vector2i rectSize, Vector2f position, Vector2f scale, float z) {
		this->type = type;
		this->texture = texture;

		loadTexture();
		setRect(rect, rectSize);
		setPosition(position);
		setScale(scale);
		cameraPosition = Vector2f(0, 0);
		zAxis = z;
	}
 
	objectSprite(string type, Texture* texture, IntRect rect, Vector2f position, Vector2f scale, float z) {
		this->type = type;
		this->texture = texture;

		loadTexture();
		setRect(rect);
		setPosition(position);
		setScale(scale);
		cameraPosition = Vector2f(0, 0);
		zAxis = z;
	}

	//Must also load an image of the texture for lighting to work
	objectSprite(string type, Texture* texture, Image im, Vector2i rect, Vector2i rectSize, Vector2f position, Vector2f scale, float z) {
		this->type = type;
		this->texture = texture;

		loadTexture();
		setRect(rect, rectSize);
		setPosition(position);
		setScale(scale);
		cameraPosition = Vector2f(0, 0);
		zAxis = z;
		image = im;
		pixelSetup();
	}

	objectSprite(string type, Texture* texture, Image im, IntRect rect, Vector2f position, Vector2f scale, float z) {
		this->type = type;
		this->texture = texture;

		loadTexture();
		setRect(rect);
		setPosition(position);
		setScale(scale);
		cameraPosition = Vector2f(0, 0);
		zAxis = z;
		image = im;
		pixelSetup();
	}

	objectSprite(objectSprite* s) {
		this->type = s->getType();
		this->texture = s->getTexture();

		loadTexture();
		setRect(s->getRect().getPosition(), s->getRect().getSize());
		setPosition(s->getPosition());
		setScale(s->getScale());
		cameraPosition = Vector2f(0, 0);
		zAxis = s->getZ();;
		  
	}

public: objectSprite(string type, Texture* texture, Vector2i rect, Vector2i rectSize, Vector2f position, Vector2f scale) {
	this->type = type;
	this->texture = texture;

	loadTexture();
	setRect(rect, rectSize);
	setPosition(position);
	setScale(scale);
	cameraPosition = Vector2f(0, 0);
	zAxis = 1;
}
public:
	objectSprite() {
		cout << "huh?";
		this->zAxis = 1;

	}

	objectSprite(Vector2f f) {
		this->zAxis = 1;
		setPosition(f);
		texture = new Texture();
		loadTexture();
		setScale(Vector2f(1, 1));
		cameraPosition = Vector2f(0, 0);

	}

	void pixelSetup() {

		pixels.clear();

		int startX = tRect.x;
		int startY = tRect.y;
		int endX = tRect.x + rectSize.x;
		int endY = tRect.y + rectSize.y;

		cout << image.getSize().x;
		cout << ", ";
		cout << image.getSize().y;
		cout << ", ";
		image.getPixel(900, 300);

		for (int y = startY; y < endY; y++) {
			for (int x = startX; x < endX; x++) {
				Color col = image.getPixel(x, y);
				auto alpha = col.a;
				if (alpha == 255) {
					RectangleShape* r = new RectangleShape();
					r->setPosition(Vector2f((x - tRect.x) * 4, (y - tRect.y) * 4));
					r->setSize(Vector2f(4, 4));
					pixels.push_back(r);
				}

				
			}
		}
	}

	list<RectangleShape*> getPixels() {
		return pixels;
	}

	/*void updatepixels() {
		for (RectangleShape* r : pixels) {
			r->setPosition(Vector2f(r->getPosition().x + cameraPosition.x, r->getPosition().y + cameraPosition.y));
		}
	}*/

	void setFullColour(const Color* c) {
		for (RectangleShape* r : pixels) {
			r->setFillColor(*c);
		}
	}

public: void setPosition(Vector2f position) {
		this->position = position;
	}

	void setCameraPosition(Vector2f c) {
		this->cameraPosition = c;
		thisOne.setPosition(c);
	}


public: Vector2f getPosition() {
		return position;
	}



	  void setVisualOffset(Vector2f f) {
		  visualOffset = f;
	  }
	  Vector2f getVisualOffset() {
		  return visualOffset;
	  }




	  void setZ(float z) {
		  zAxis = z;
	  }
	  float getZ() {
		  return zAxis;
	  }













	  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
public:void flip() {
	if (!flipped) {
		setScale(Vector2f(-this->scale.x, this->scale.y));
		this->setPosition(Vector2f(this->position.x + (this->rectSize.x * -this->scale.x), this->position.y));
		flipped = true;
		
	}
	else {
		setScale(Vector2f(-this->scale.x, this->scale.y));
		this->setPosition(Vector2f (this->position.x - (this->rectSize.x * this->scale.x), this->position.y));
		flipped = false;
		
	}
}
public:void setFlipPos(bool f) {
	flipPos = f;
}
public:float getFAmount() {
	return this->rectSize.x * -this->scale.x;
}
public:bool getFlipped() {
	return flipped;
}
public:void setFlipped(bool flip) {
	this->flipped = flip;
}
public:int getRectX() {
	return rectSize.x;
}*/

	Vector2f getRelativeCenter() {
		float x = ((rectSize.x * scale.x)/2);
		float y =  (rectSize.y * scale.y)/2;
		return Vector2f(x, y);
	}

};