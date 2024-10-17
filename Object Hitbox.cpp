#include "UI Hitbox.cpp"
#include "Object Sprite.cpp"
#pragma once

class objectHitbox:public UIHitbox {
	using UIHitbox::UIHitbox;

	objectSprite* sprite;
	Vector2f worldPos;


	//Rectangle to be inputted will be relative to the top-left corner of the sprite it's attached too.
	//Size will be based on the sprite sheet the sprite is from, so it will also be relative.
	public:objectHitbox(IntRect relative, bool vis, objectSprite* s) {
		sprite = s;
		relativePos = Vector2f(relative.getPosition());
		worldPos = Vector2f(relativePos.x + sprite->getPosition().x, relativePos.y + sprite->getPosition().y);
		visible = vis;
		
		scale = sprite->getScale();
		relativeRect = IntRect(Vector2i(relativePos), Vector2i(relative.getSize().x * scale.x, relative.getSize().y * scale.y));
	}

	public:void updatePos() {
		this->worldPos = sprite->getPosition() + relativePos;
	}
public:void setCameraPos(Vector2f pos) {
	this->cameraPos = pos;
}
public: Vector2f getPosition() {
	return worldPos;
}
public: objectSprite* getSprite() {
	return sprite;
}
	  void setPosition(Vector2f p) {
		  this->worldPos = p;
	  }
	  void setRelativePosition(Vector2f p) {
		  this->relativePos = p;
	  }
};