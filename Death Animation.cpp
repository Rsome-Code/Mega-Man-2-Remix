#include "Movable object.cpp"
#include "Animate.cpp"
#include "Animation timer.cpp"

#pragma once

class DeathAnim {
	shared_ptr<objectSprite> sprite;
	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;
	float growRate = 0.5;
	float growAccel = 64;

	Vector2f originPos;
	Vector2f originSize;

	
public:

	virtual ~DeathAnim() {

	}

	DeathAnim(shared_ptr<objectSprite> origin, string palette) {
		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture);
		t->loadFromFile("Assets//death//" + palette + ".png");
		sprite = shared_ptr<objectSprite>(new objectSprite("death anim", t, IntRect(0, 0, 96, 96), origin->getPosition(), Vector2f(0.00000, 0.00000)));
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(0, 0, 96, 96), IntRect(98, 0, 96, 96), IntRect(196, 0, 96, 96), IntRect(294, 0, 96, 96)}, sprite));
		timer = shared_ptr<animTimer> (new animTimer(anim, 15, true));
		originPos = origin->getPosition();
		originSize = origin->getSize();
	}

	void run(float* deltaT) {
		timer->run(deltaT);

		float growThisFrame = growRate * *deltaT;

		growRate = growRate * (pow(growAccel, *deltaT));

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
	
	shared_ptr<objectSprite> getSprite() {
		return sprite;
	}

	shared_ptr<animation> getAnim() {
		return anim;
	}

	void setOrigin(shared_ptr<objectSprite> sprite) {
		originPos = sprite->getPosition();
		originSize = sprite->getSize();
	}
};