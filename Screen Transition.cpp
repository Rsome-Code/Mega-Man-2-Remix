#include "object.cpp"
#include "Object Hitbox.cpp"
#pragma once

class transition :public object {
	objectHitbox* hit;
	bool active;
	int type;

public:
	transition(Vector2f pos, int t) {
		type = t;
		sprite = new objectSprite(Vector2f((pos.x * 16) * 4, (pos.y * 16) * 4));
		hit = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(5, 1080)), true, sprite);
	}

	objectHitbox* getHitbox() {
		return hit;
	}

	int getType() {
		return type;
	}
};