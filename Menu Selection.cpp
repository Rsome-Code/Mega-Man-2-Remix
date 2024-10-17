#include"UI Sprite.cpp"
#include "UI Hitbox.cpp"
#pragma once

class menuSelect {
	UISprite* sprite;
	UIHitbox* hit;

public:

	menuSelect(Texture* T, Vector2i i, Vector2f pos) {
		sprite = new UISprite("menu", T, i, Vector2i(16, 16), pos, Vector2f(4,4));
		hit = new UIHitbox(IntRect(Vector2i(0,0), Vector2i(16,16)), true, sprite);
	}

	UISprite* getSprite() {
		return sprite;
	}
	UIHitbox* getHitbox() {
		return hit;
	}
};