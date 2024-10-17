#include "UI Sprite.cpp"
#pragma once
//#include "Object Sprite.cpp"
//#include "Movable Object.cpp"

class UIHitbox {
protected:IntRect relativeRect;
protected:Vector2f cameraPos;
protected:Vector2f relativePos;
private:UISprite* sprite;
protected:bool visible;
protected:Vector2f scale;
		 bool scaled = false;


	// rect is the relative position of the hitbox
public:
	UIHitbox(IntRect rect, bool visible, UISprite* sprite) {
		this->sprite = sprite;
		relativePos = Vector2f(rect.getPosition());
		cameraPos= Vector2f (sprite->getCameraPosition().x + relativePos.x, sprite->getCameraPosition().y + relativePos.y);
		scale = sprite->getScale();
		this->visible = visible;
		relativeRect = IntRect(Vector2i(relativePos), Vector2i(rect.getSize().x * scale.x, rect.getSize().y * scale.y));
	}
	UIHitbox() {
		//cout << "heh";
	}

public: void updatePos() {
	this->cameraPos = relativePos + sprite->getCameraPosition();
}
public: bool isVisible() {
	return visible;
}
public: Vector2f getCameraPos() {
	return cameraPos;
}
public: Vector2i getSize() {
	return relativeRect.getSize();
}
	void setSize(Vector2i size) {
		  this->relativeRect = IntRect(relativeRect.getPosition(), size);
	}
public: UISprite* getSprite() {
	return sprite;
}

};