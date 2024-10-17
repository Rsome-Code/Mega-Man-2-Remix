#include"object Sprite.cpp"
#include "object Hitbox.cpp"
#pragma once

class objectSelect {
	objectSprite* sprite;
	objectHitbox* hit;

public:

	objectSelect(Texture* T, Vector2i i, Vector2f pos) {
		sprite = new objectSprite("menu", T, i, Vector2i(16, 16), pos, Vector2f(4, 4));
		hit = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 16)), true, sprite);
	}

	objectSprite* getSprite() {
		return sprite;
	}
	objectHitbox* getHitbox() {
		return hit;
	}
};