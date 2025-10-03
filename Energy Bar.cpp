#include "object.cpp"
#include <SFML/audio.hpp>
#pragma once

class energyBar {
protected:
	UISprite* sprite;
	int amount;
	int max = 28;

	SoundBuffer* soundB;
	Sound* sound;


public:
	energyBar(Texture* t, Vector2f loc, int ini) {
		amount = ini;
		sprite = new UISprite("Bar", t, Vector2i(1, 1), Vector2i(9, 56), loc, Vector2f(4, 4));
		setAmount();
		soundB = new SoundBuffer();
		soundB->loadFromFile("assets\\sound\\refill.wav");
		sound = new Sound();
		sound->setBuffer(*soundB);
	}
	energyBar(Texture* t, Vector2f loc) {
		amount = max;

		sprite = new UISprite("Bar", t, Vector2i(1, 1), Vector2i(9, 56), loc, Vector2f(4, 4));
		setAmount();
		soundB = new SoundBuffer();
		soundB->loadFromFile("assets\\sound\\refill.wav");
		sound = new Sound();
		sound->setBuffer(*soundB);
	}

	void setAmount() {
		
		int temp = 1 + (9 * amount);
		if (temp == NULL) {
			sprite->setRect(Vector2i(0, 1), Vector2i(8, 56));
		}
		else {
			sprite->setRect(Vector2i(temp, 1), Vector2i(8, 56));
		}
	}

	int getAmount() {
		return amount;
	}
	void increaseAmount(int a) {
		if (amount + a > max) {
			amount = max;
		}
		else if (amount + a < 0) {
			amount = 0;
		}
		else{
			if (a > 0) {
				sound->play();
			}

			amount = amount + a;
		}
		setAmount();
	}

	void reset() {
		amount = max;
		setAmount();
	}
	UISprite* getSprite() {
		return sprite;
	}
};