#include "UI Sprite.cpp"
#include "weapon.cpp"
#include "Ammo Bar.cpp"
#pragma once

class Option {
protected:
	UISprite* icon;
	float flashTime = 0.15;
	float flashTime_left = flashTime;

	bool displayIcon = true;

	Vector2f position;

	int optionNum;


public:
	Option() {}
	Option(Texture* t, Vector2f pos) {
		position = pos;



		icon = new UISprite("Letter", t, IntRect(0, 8, 8, 8), position, Vector2f(4, 4));
	
	}

	void setNum(int n) {
		optionNum = n;
	}
	int getNum() {
		return optionNum;
	}

	void active(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			displayIcon = !displayIcon;
			flashTime_left = flashTime;
		}
	}
	void reset() {
		flashTime_left = flashTime;
		displayIcon = true;
	}

	virtual AmmoBar* getBar() {
		return NULL;
	}

	virtual list<UISprite*> getSprites() {
		list<UISprite*> temp;
		if (displayIcon) {
			temp.push_back(icon);
		}

		return temp;

	}

	virtual Weapon* getWeapon() {
		return NULL;
	}
};