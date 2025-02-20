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
	list<objectSprite*>::iterator objectI;


	bool flipped = false;
	bool flipPos = false;

	Vector2f visualOffset = Vector2f(0, 0);

	
	public: objectSprite(string type, Texture* texture, Vector2i rect, Vector2i rectSize, Vector2f position, Vector2f scale, float cameraSpeed) {
		this->type = type;
		this->texture = texture;

		loadTexture();
		setRect(rect, rectSize);
		setPosition(position);
		setScale(scale);
		cameraPosition = Vector2f(0, 0);
		zAxis = cameraSpeed;
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
}

	Vector2f getRelativeCenter() {
		float x = ((rectSize.x * scale.x)/2);
		float y =  (rectSize.y * scale.y)/2;
		return Vector2f(x, y);
	}

};