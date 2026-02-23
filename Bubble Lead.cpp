#include "weapon.cpp"
#include "bubble bullet.cpp"
#pragma once

class BubbleLead : public Weapon {

public:
	BubbleLead(shared_ptr<objectSprite> player, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {
		maxWeaponCount = 2;

		for (int i = 0; i < maxWeaponCount; i++) {
			shared_ptr<BubbleBullet> bul = shared_ptr<BubbleBullet>(new BubbleBullet(player, t, soundCol));
			bullets.push_back(bul);
		}

		colourP = shared_ptr<Texture> (new Texture());
		colourP->loadFromFile("Assets\\player\\bubble lead.png");

		ammoDecrease = 0.5;
		name = "Bubble Lead";

		shootB = shared_ptr<SoundBuffer> (new SoundBuffer());
		shootB->loadFromFile("assets\\sound\\buster.wav");
		shootSound = shared_ptr<Sound>(new Sound());
		shootSound->setBuffer(*shootB);
	}

	


	String getSymbol() {
		return "B";
	}

};