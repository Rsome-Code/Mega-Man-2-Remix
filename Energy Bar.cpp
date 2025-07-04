#include "object.cpp"
#pragma once

class energyBar {
protected:
	UISprite* sprite;
	int amount;
	int max = 28;

public:
	energyBar(Texture* t, Vector2f loc, int ini) {
		amount = ini;
		sprite = new UISprite("Bar", t, Vector2i(1, 1), Vector2i(9, 56), loc, Vector2f(4, 4));
		setAmount();
	}
	energyBar(Texture* t, Vector2f loc) {
		amount = max;

		sprite = new UISprite("Bar", t, Vector2i(1, 1), Vector2i(9, 56), loc, Vector2f(4, 4));
		setAmount();
	}

	void setAmount() {
		int temp = 1 + (9 * amount);
		sprite->setRect(Vector2i(temp, 1), Vector2i(8, 56));
	}

	int getAmount() {
		return amount;
	}
	void increaseAmount(int a) {
		if (amount + a > max) {
			amount = max;
		}
		else {
			amount = amount + a;
		}
		setAmount();
	}
	UISprite* getSprite() {
		return sprite;
	}
};