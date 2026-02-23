#include "Weapon.cpp"
#pragma once

class LeafShield :public Weapon {

public:
	LeafShield(shared_ptr<objectSprite> player, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {
		//bullets = { new megaBuster(player, t),  new megaBuster(player, t),  new megaBuster(player, t) };
		//maxWeaponCount = 3;
		colourP = shared_ptr<Texture> (new Texture());
		colourP->loadFromFile("Assets\\player\\NES - Mega Man 2 - Mega Man.png");
		name = "Leaf Shield";
		

		shootB = shared_ptr<SoundBuffer> (new SoundBuffer());
		shootB->loadFromFile("Assets\\sound\\buster.wav");
		shootSound = shared_ptr<Sound>(new Sound());
		shootSound->setBuffer(*shootB);
	}
	String getSymbol() {
		return "W";
	}
};