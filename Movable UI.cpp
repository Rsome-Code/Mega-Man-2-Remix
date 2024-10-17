#include "UI Sprite.cpp"
#pragma once

class movableUI:public UISprite {

	int speed = 500;

	using UISprite::UISprite;

	Vector2f calcXY(float angle, float dist) {
		angle = angle / 57.295779513082320876798154814105;
		float adjacent = cos(angle) * dist;
		float opposite = sin(angle) * dist;

		Vector2f newPos = Vector2f(cameraPosition.x + adjacent, cameraPosition.y + opposite);

		return newPos;
	}

public:void move(float angle, float* since, float SP) {

	float last = *since;

	float s = this->speed * (SP / 100);
	float thisFrameD = s * last;

	Vector2f amount = Vector2f(calcXY(angle, thisFrameD));
	setCameraPosition(amount);
}
public: void setSpeed(float newS) {
	this->speed = newS;
}
};