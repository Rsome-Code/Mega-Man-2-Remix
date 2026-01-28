#include "Weapon.cpp"
#pragma once

class LeafShield :public Weapon {

public:
	LeafShield(objectSprite* player, Texture* t, SoundCollection* soundCol) {
		//bullets = { new megaBuster(player, t),  new megaBuster(player, t),  new megaBuster(player, t) };
		//maxWeaponCount = 3;
		colourP = new Texture();
		colourP->loadFromFile("Assets\\player\\NES - Mega Man 2 - Mega Man.png");
		name = "Leaf Shield";
		

		shootB = new SoundBuffer();
		shootB->loadFromFile("Assets\\sound\\buster.wav");
		shootSound = new Sound();
		shootSound->setBuffer(*shootB);
	}
	String getSymbol() {
		return "W";
	}
};