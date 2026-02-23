#include "weapon.cpp"
#include "boomerang bullet.cpp"
#pragma once

class QuickBoomerang : public Weapon {

public:
	QuickBoomerang(shared_ptr<objectSprite> player, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {
		maxWeaponCount = 6;

		for (int i = 0; i < maxWeaponCount; i++) {
			shared_ptr<BoomerangBullet> bul = shared_ptr<BoomerangBullet>(new BoomerangBullet(t, player, soundCol));
			bullets.push_back(bul);
		}

		colourP = shared_ptr<Texture> (new Texture());
		colourP->loadFromFile("Assets\\player\\Quick Boomerang.png");

		ammoDecrease = 0.1;
		name = "Quick Boomerang";

		shootB = shared_ptr<SoundBuffer> (new SoundBuffer());
		shootB->loadFromFile("assets\\sound\\buster.wav");
		shootSound = shared_ptr<Sound>(new Sound());
		shootSound->setBuffer(*shootB);
	}

	string getTextName() {
		return "Quick\n-Boomerang";
	}

	String getSymbol() {
		return "Q";
	}
};