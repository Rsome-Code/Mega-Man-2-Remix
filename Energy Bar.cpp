#include "object.cpp"
#include <SFML/audio.hpp>
#pragma once

class energyBar {
protected:
	shared_ptr<UISprite> sprite;
	int amount;
	int max = 28;

	shared_ptr<SoundBuffer> soundB;
	shared_ptr<Sound> sound;


public:

	virtual ~energyBar() {

	}

	energyBar(shared_ptr<Texture> t, Vector2f loc, int ini) {
		amount = ini;
		sprite = shared_ptr<UISprite>(new UISprite("Bar", t, Vector2i(1, 1), Vector2i(9, 56), loc, Vector2f(4, 4)));
		setAmount();
		soundB = shared_ptr<SoundBuffer> (new SoundBuffer());
		soundB->loadFromFile("assets\\sound\\refill.wav");
		sound = shared_ptr<Sound>(new Sound());
		sound->setBuffer(*soundB);
	}
	energyBar(shared_ptr<Texture> t, Vector2f loc) {
		amount = max;

		sprite = shared_ptr<UISprite>(new UISprite("Bar", t, Vector2i(1, 1), Vector2i(9, 56), loc, Vector2f(4, 4)));
		setAmount();
		soundB = shared_ptr<SoundBuffer> (new SoundBuffer());
		soundB->loadFromFile("assets\\sound\\refill.wav");
		sound = shared_ptr<Sound>(new Sound());
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

	void setAmount(int num) {
		amount = num;
		setAmount();
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
	shared_ptr<UISprite> getSprite() {
		return sprite;
	}
};