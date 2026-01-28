#include "weapon.cpp"
#include "boomerang bullet.cpp"
#pragma once

class QuickBoomerang : public Weapon {

public:
	QuickBoomerang(objectSprite* player, Texture* t, SoundCollection* soundCol) {
		maxWeaponCount = 6;

		for (int i = 0; i < maxWeaponCount; i++) {
			BoomerangBullet* bul = new BoomerangBullet(t, player, soundCol);
			bullets.push_back(bul);
		}

		colourP = new Texture();
		colourP->loadFromFile("Assets\\player\\Quick Boomerang.png");

		ammoDecrease = 0.1;
		name = "Quick Boomerang";

		shootB = new SoundBuffer();
		shootB->loadFromFile("assets\\sound\\buster.wav");
		shootSound = new Sound();
		shootSound->setBuffer(*shootB);
	}

	string getTextName() {
		return "Quick\n-Boomerang";
	}

	String getSymbol() {
		return "Q";
	}
};