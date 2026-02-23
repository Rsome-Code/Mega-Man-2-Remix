#include "object Sprite.cpp"
#include "object Hitbox.cpp"
#pragma once

class grid {
	shared_ptr<objectSprite> sprite;
	shared_ptr<objectHitbox> hit;

public:
	grid() {
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("Assets\\grids.png");
		sprite = shared_ptr<objectSprite>(new objectSprite("background", t, Vector2i(0, 0), Vector2i(4905, 4707), Vector2f(0, 0), Vector2f(1, 1), 1));
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(90, 1277), Vector2i(4683, 1000)), false, sprite));

	}
	shared_ptr<objectHitbox> getHitbox() {
		return hit;
	}
	shared_ptr<objectSprite> getSprite() {
		return sprite;
	}
};