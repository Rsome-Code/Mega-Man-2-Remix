#include "UI Sprite.cpp"
#include "weapon.cpp"
#include "Ammo Bar.cpp"
#include "pause option.cpp"
#pragma once

class WeaponOption:public Option {

	Weapon* weapon;
	AmmoBar* bar;
	

public:
	WeaponOption(Weapon* w, Vector2f pos) {
		position = pos;
		weapon = w;

		Texture* t = new Texture();
		t->loadFromFile("assets\\pause\\bars\\" + w->getName() + ".png");

		bar = new AmmoBar(w->getMaxAmmo(), Vector2f(position.x + (13 *4), position.y), t);
		icon = new UISprite("Letter", t, IntRect(0, 8, 8, 8), position, Vector2f(4,4));
		bar->update(weapon->getAmmo());
	}

	void active(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			displayIcon = !displayIcon;
			flashTime_left = flashTime;
		}
	}

	Weapon* getWeapon() {
		return weapon;
	}

	void reset() {
		flashTime_left = flashTime;
		displayIcon = true;
	}

	AmmoBar* getBar() {
		return bar;
	}

	list<UISprite*> getSprites(){
		list<UISprite*> temp = bar->getSprites();
		if (displayIcon) {
			temp.push_back(icon);
		}

		return temp;
		
	}
};