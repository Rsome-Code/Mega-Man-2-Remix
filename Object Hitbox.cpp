#include "UI Hitbox.cpp"
#include "Object Sprite.cpp"
#include "physics object.cpp"
#pragma once

class objectHitbox:public UIHitbox {
	//using UIHitbox::UIHitbox;

	shared_ptr<objectSprite> sprite;
	Vector2f worldPos;


	//Rectangle to be inputted will be relative to the top-left corner of the sprite it's attached too.
	//Size will be based on the sprite sheet the sprite is from, so it will also be relative.
public:objectHitbox(IntRect relative, bool vis, shared_ptr<objectSprite> s) {
		sprite = s;
		relativePos = Vector2i(relative.getPosition());
		worldPos = Vector2f(relativePos.x + sprite->getPosition().x, relativePos.y + sprite->getPosition().y);
		visible = vis;
		
		scale = sprite->getScale();
		relativeRect = IntRect(Vector2i(relativePos), Vector2i(relative.getSize().x * scale.x, relative.getSize().y * scale.y));
	}

	objectHitbox(IntRect relative, shared_ptr<objectSprite> s) {
		sprite = s;
		relativePos = relative.getPosition();
		worldPos = Vector2f(relativePos.x + sprite->getPosition().x, relativePos.y + sprite->getPosition().y);
		visible = false;

		scale = sprite->getScale();
		relativeRect = IntRect(Vector2i(relativePos), Vector2i(relative.getSize().x * scale.x, relative.getSize().y * scale.y));
	}




	objectHitbox() {
		relativePos = Vector2i(0, 0);
		relativeRect = IntRect(Vector2i(0,0), Vector2i(0,0));
	}


	public:void updatePos() {
		this->worldPos = sprite->getPosition() + Vector2f(relativePos);
	}

	 IntRect getRelativeRect() {
			  return relativeRect;
	}

	 void setRelativeRect(IntRect r) {
		 relativePos = Vector2i(r.getPosition().x + (sprite->getVisualOffset().x), r.getPosition().y + (sprite->getVisualOffset().y));
		 worldPos = Vector2f(relativePos.x + sprite->getPosition().x, relativePos.y + sprite->getPosition().y);
		 relativeRect = IntRect(relativePos, Vector2i(r.getSize().x * sprite->getScale().x, r.getSize().y * sprite->getScale().y));
	 }

public:void setCameraPos(Vector2f pos) {
	this->cameraPos = pos;
}
public: Vector2f getPosition() {
	return worldPos;
}
public: shared_ptr<objectSprite> getSprite() {
	return sprite;
}
	  void setPosition(Vector2f p) {
		  this->worldPos = p;
	  }
	  void setRelativePosition(Vector2i p) {
		  this->relativePos = p;
		  relativeRect = (IntRect(p, relativeRect.getSize()));
	  }

	  void setRelativeSize(Vector2i s) {
		  this->relativeRect = IntRect(relativePos, s);
	  }

		Vector2f getRelativePosition() {
		  return Vector2f(this->relativePos);
	  }

		void setSprite(shared_ptr<objectSprite> s) {
			sprite = s;
		}
};