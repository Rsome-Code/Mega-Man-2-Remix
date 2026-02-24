#include "door.cpp"
#pragma once

class FlashDoor : public Door {

	using Door::Door;

	float flashTime = 0.38216560509554140127388535031847;
	float flashTime_left = flashTime;

	int beat = 1;


	void resetAnim() {
		beat = 1;
	}

	void animate(float* deltaT) {

		flashTime_left -= *deltaT;

		if (flashTime_left <= 0) {
			flashTime_left = flashTime;

			if (beat == 0) {
				sprite->setRect(IntRect(sprite->getRect().getPosition().x - 16, sprite->getRect().getPosition().y, sprite->getRect().getSize().x, sprite->getRect().getSize().y));
			}

			beat = (beat + 1) %3;

			if (beat == 0) {
				sprite->setRect(IntRect(sprite->getRect().getPosition().x + 16, sprite->getRect().getPosition().y, sprite->getRect().getSize().x, sprite->getRect().getSize().y));
			}
		}

	}

};