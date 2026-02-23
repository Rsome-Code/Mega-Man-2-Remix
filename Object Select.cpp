#include"object Sprite.cpp"
#include "object Hitbox.cpp"
#pragma once

class objectSelect {
	shared_ptr<objectSprite> sprite;
	shared_ptr<objectHitbox> hit;

public:

	objectSelect(shared_ptr<Texture> T, Vector2i i, Vector2f pos) {
		sprite = shared_ptr<objectSprite>(new objectSprite("menu", T, i, Vector2i(16, 16), pos, Vector2f(4, 4)));
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 16)), true, sprite));
	}

	shared_ptr<objectSprite> getSprite() {
		return sprite;
	}
	shared_ptr<objectHitbox> getHitbox() {
		return hit;
	}
};