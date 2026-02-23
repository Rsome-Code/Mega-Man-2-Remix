#include "UI Sprite.cpp"
#include "weapon.cpp"
#include "Ammo Bar.cpp"
#include "pause option.cpp"
#pragma once

class WeaponOption:public Option {

	shared_ptr<Weapon> weapon;
	shared_ptr<AmmoBar> bar;
	

public:
	WeaponOption(shared_ptr<Weapon> w, Vector2f pos) {
		position = pos;
		weapon = w;

		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\pause\\bars\\" + w->getName() + ".png");

		bar = shared_ptr<AmmoBar>(new AmmoBar(t, Vector2f(position.x + (13 *4), position.y)));
		icon = shared_ptr<UISprite>(new UISprite("Letter", t, IntRect(0, 8, 8, 8), position, Vector2f(4,4)));
		bar->update(weapon->getAmmo());
		bar->stopSound();
	}

	void active(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			displayIcon = !displayIcon;
			flashTime_left = flashTime;
		}
	}

	shared_ptr<Weapon> getWeapon() {
		return weapon;
	}

	void reset() {
		flashTime_left = flashTime;
		displayIcon = true;
	}

	shared_ptr<AmmoBar> getBar() {
		return bar;
	}

	list<shared_ptr<UISprite>> getSprites(){
		list<shared_ptr<UISprite>> temp = bar->getSprites();
		if (displayIcon) {
			temp.push_back(icon);
		}

		return temp;
		
	}
};