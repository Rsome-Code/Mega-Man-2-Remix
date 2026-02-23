#include "object.cpp"
#include "Object Hitbox.cpp"
#pragma once

class transition :public object {
	shared_ptr<objectHitbox> hit;
	bool active;
	int type;

public:
	transition(Vector2f pos, int t) {
		type = t;
		sprite = shared_ptr<objectSprite>(new objectSprite(Vector2f((pos.x * 16) * 4, (pos.y * 16) * 4)));
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(5, 1080)), true, sprite));
	}

	shared_ptr<objectHitbox> getHitbox() {
		return hit;
	}

	int getType() {
		return type;
	}
};