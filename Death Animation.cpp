#include "Movable object.cpp"
#include "Animate.cpp"
#include "Animation timer.cpp"

#pragma once

class DeathAnim {
	objectSprite* sprite;
	animation* anim;
	animTimer* timer;
	float growRate = 1;
	float growAccel = 1.03;

	Vector2f originPos;
	Vector2f originSize;

	
public:
	DeathAnim(objectSprite* origin, string palette) {
		Texture* t = new Texture;
		t->loadFromFile("Assets//death//" + palette + ".png");
		sprite = new objectSprite("death anim", t, IntRect(0, 0, 96, 96), origin->getPosition(), Vector2f(0.000005, 0.000005));
		anim = new animation(list<IntRect>{IntRect(0, 0, 96, 96), IntRect(98, 0, 96, 96), IntRect(196, 0, 96, 96), IntRect(294, 0, 96, 96)}, sprite);
		timer = new animTimer(anim, 15, true);
		originPos = origin->getPosition();
		originSize = origin->getSize();
	}

	void run(float* deltaT) {
		timer->run(deltaT);

		float growThisFrame = growRate * *deltaT;
		growRate = growRate * growAccel;

		sprite->setScale(Vector2f(sprite->getScale().x + growThisFrame, sprite->getScale().y + growThisFrame));

		setPosition();
	}

	void setPosition() {
		float middle = sprite->getSize().x / 2;
		float newPosX = originPos.x - middle;
		float newPosY = originPos.y - middle;

		newPosX += originSize.x / 2;
		newPosY += originSize.y / 2;

		sprite->setPosition(Vector2f(newPosX, newPosY));
	}
	
	objectSprite* getSprite() {
		return sprite;
	}
};