#include "weapon.cpp"
#include "mega buster bullet.cpp"

#pragma once

class MegaBuster : public Weapon {

public:
	MegaBuster(shared_ptr<objectSprite> player, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {
		bullets = { shared_ptr<megaBuster>(new megaBuster(player, t, soundCol)),  shared_ptr<megaBuster>(new megaBuster(player, t, soundCol)),  shared_ptr<megaBuster>(new megaBuster(player, t, soundCol)) };
		maxWeaponCount = 3;
		colourP = shared_ptr<Texture> (new Texture());
		colourP->loadFromFile("Assets\\player\\NES - Mega Man 2 - Mega Man.png");
		name = "Mega Buster";
		maxAmmo = 28;

		shootB = shared_ptr<SoundBuffer> (new SoundBuffer());
		shootB->loadFromFile("Assets\\sound\\buster.wav");
		shootSound = shared_ptr<Sound>(new Sound());
		shootSound->setBuffer(*shootB);
	}

	bool checkAmmo() {
		return true;
	}

	String getSymbol() {
		return "";
	}
	
};