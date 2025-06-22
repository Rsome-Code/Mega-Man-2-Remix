#include "Object Sprite.cpp"
#pragma once

class movable:public objectSprite {
	using objectSprite::objectSprite;
protected: 
	float speed;
	bool canMove = true;

public:
	movable(string type, Texture* texture, Vector2i rect, Vector2i rectSize, Vector2f position, Vector2f scale, int cameraSpeed) {
		this->type = type;
		this->texture = texture;

		loadTexture();
		setRect(rect, rectSize);
		setPosition(position);
		setScale(scale);
		cameraPosition = Vector2f(0, 0);
		this->speed = 500;
		zAxis = cameraSpeed;
	}
	movable(string type, Texture* texture, Image im, IntRect rect, Vector2f position, Vector2f scale, int cameraSpeed, float defaultTransparency) {
		this->type = type;
		this->texture = texture;

		loadTexture();
		setRect(rect.getPosition(), rect.getSize());
		setPosition(position);
		setScale(scale);
		cameraPosition = Vector2f(0, 0);
		this->speed = 500;
		zAxis = cameraSpeed;
		this->defaultTransparency = defaultTransparency;
		image = im;
	}
public: movable() {
}
	Vector2f calcXY(float angle, float dist) {
		angle = angle / 57.295779513082320876798154814105;
		float adjacent = cos(angle) * dist;
		float opposite = sin(angle) * dist;

		Vector2f newPos = Vector2f(position.x + adjacent, position.y + opposite);

		return newPos;
	}

public:
	void setMove(bool b) {
		canMove = b;
	}
	void move(float angle, float* since) {
		if (canMove) {
			float last = *since;


			float thisFrameD = speed * last;

			Vector2f amount = Vector2f(calcXY(angle, thisFrameD));
			setPosition(amount);
		}
	}

	void move(float angle, float* since, float speed) {
		if (canMove) {
			float last = *since;

			float thisFrameD = speed * last;

			Vector2f amount = Vector2f(calcXY(angle, thisFrameD));
			setPosition(amount);
		}
	}
	void move(float angle, float dist) {
		if (canMove) {
			float thisFrameD = dist;

			Vector2f amount = Vector2f(calcXY(angle, thisFrameD));
			setPosition(amount);
		}
	}

	//accel is in pixel per second per second
	/*void accelerate(float* since, float accel) {
		float speedChange = accel * *since;
		setSpeed(this->speed + speedChange);
	}*/

	void setSpeed(float newS) {
		this->speed = newS;
	}
	float getSpeed() {
		return speed;
	}
};