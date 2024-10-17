#include "object Sprite.cpp"
#include "object Hitbox.cpp"
#pragma once

class grid {
	objectSprite* sprite;
	objectHitbox* hit;

public:
	grid() {
		Texture* t = new Texture();
		t->loadFromFile("Assets\\grids.png");
		sprite = new objectSprite("background", t, Vector2i(0, 0), Vector2i(4905, 4707), Vector2f(0, 0), Vector2f(1, 1), 1);
		hit = new objectHitbox(IntRect(Vector2i(90, 1277), Vector2i(4683, 1000)), false, sprite);

	}
	objectHitbox* getHitbox() {
		return hit;
	}
	objectSprite* getSprite() {
		return sprite;
	}
};