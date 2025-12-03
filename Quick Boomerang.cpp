#include "weapon.cpp"
#include "boomerang bullet.cpp"
#pragma once

class QuickBoomerang : public Weapon {

public:
	QuickBoomerang(objectSprite* player, Texture* t) {
		maxWeaponCount = 6;

		for (int i = 0; i < maxWeaponCount; i++) {
			BoomerangBullet* bul = new BoomerangBullet(t, player);
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

	String getSymbol() {
		return "Q";
	}
};