#include "weapon.cpp"
#include "bubble bullet.cpp"
#pragma once

class BubbleLead : public Weapon {

public:
	BubbleLead(objectSprite* player, Texture* t, SoundCollection* soundCol) {
		maxWeaponCount = 2;

		for (int i = 0; i < maxWeaponCount; i++) {
			BubbleBullet* bul = new BubbleBullet(player, t, soundCol);
			bullets.push_back(bul);
		}

		colourP = new Texture();
		colourP->loadFromFile("Assets\\player\\bubble lead.png");

		ammoDecrease = 0.5;
		name = "Bubble Lead";

		shootB = new SoundBuffer();
		shootB->loadFromFile("assets\\sound\\buster.wav");
		shootSound = new Sound();
		shootSound->setBuffer(*shootB);
	}

	


	String getSymbol() {
		return "B";
	}

};