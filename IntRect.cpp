#include "Vector2i.cpp"
#pragma once


class IntRect {

	Vector2i position;
	Vector2i size;
public:
	int left;
	int right;
	int top;
	int bottom;

	int width;
	int height;

	IntRect() {
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;

		width = 0;
		height = 0;
	
	}
	IntRect(Vector2i position, Vector2i size) {
		this->position = position;
		left = this->position.x;
		right = this->position.x + size.x;
		top = this->position.y;
		bottom = this->position.y + size.y;
		width = size.x;
		height = size.y;
		this->size = size;
	}
	IntRect(int x, int y, int width, int height) {
		this->position = Vector2i(x, y);
		left = position.x;
		right = this->position.x + size.x;
		this->size = Vector2i(width, height);
		top = this->position.y;
		bottom = this->position.y + size.y;
		this->width = size.x;
		this->height = size.y;
	}

	Vector2i getPosition() {
		return position;
	}
	Vector2i getSize() {
		return size;
	}
	

};