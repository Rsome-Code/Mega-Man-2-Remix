#include"UI Sprite.cpp"
#include "UI Hitbox.cpp"
#pragma once

class menuSelect {
	shared_ptr<UISprite> sprite;
	shared_ptr<UIHitbox> hit;

public:

	virtual ~menuSelect() {

	}

	menuSelect(shared_ptr<Texture> T, Vector2i i, Vector2f pos) {
		sprite = shared_ptr<UISprite>(new UISprite("menu", T, i, Vector2i(16, 16), pos, Vector2f(4,4)));
		hit = shared_ptr<UIHitbox>(new UIHitbox(IntRect(Vector2i(0,0), Vector2i(16,16)), true, sprite));
	}

	shared_ptr<UISprite> getSprite() {
		return sprite;
	}
	shared_ptr<UIHitbox> getHitbox() {
		return hit;
	}
};