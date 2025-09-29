#include "weapon.cpp"
#include "mega buster bullet.cpp"

#pragma once

class MegaBuster : public Weapon {

public:
	MegaBuster(objectSprite* player, Texture* t) {
		bullets = { new megaBuster(player, t),  new megaBuster(player, t),  new megaBuster(player, t) };
		maxWeaponCount = 3;
		colourP = new Texture();
		colourP->loadFromFile("Assets\\player\\NES - Mega Man 2 - Mega Man.png");
		name = "Mega Buster";
		maxAmmo = 28;

		shootB = new SoundBuffer();
		shootB->loadFromFile("Assets\\sound\\buster.wav");
		shootSound = new Sound();
		shootSound->setBuffer(*shootB);
	}

	bool checkAmmo() {
		return true;
	}
	
};