#include "UI Sprite.cpp"
#include "weapon.cpp"
#include "Ammo Bar.cpp"
#pragma once

class Option {
protected:
	shared_ptr<UISprite> icon;
	float flashTime = 0.15;
	float flashTime_left = flashTime;

	bool displayIcon = true;

	Vector2f position;

	int optionNum;


public:
	Option() {}
	Option(shared_ptr<Texture> t, Vector2f pos) {
		position = pos;



		icon = shared_ptr<UISprite>(new UISprite("Letter", t, IntRect(0, 8, 8, 8), position, Vector2f(4, 4)));
	
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

	virtual shared_ptr<AmmoBar> getBar() {
		return NULL;
	}

	virtual list<shared_ptr<UISprite>> getSprites() {
		list<shared_ptr<UISprite>> temp;
		if (displayIcon) {
			temp.push_back(icon);
		}

		return temp;

	}

	virtual shared_ptr<Weapon> getWeapon() {
		return NULL;
	}
};