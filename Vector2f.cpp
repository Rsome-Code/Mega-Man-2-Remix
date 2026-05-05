#include "vector2i.cpp"
#pragma once

class Vector2f: public Vector2i {
public:
	float x;
	float y;

	Vector2f() {}
	Vector2f(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vector2f(Vector2i i) {
		this->x = i.x;
		this->y = i.y;
	}


};