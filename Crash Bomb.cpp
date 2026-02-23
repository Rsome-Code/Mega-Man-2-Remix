#include "weapon.cpp"
#include "crash bullet.cpp"
#pragma once

class CrashBomb : public Weapon {

	shared_ptr<CrashBullet> bul;

public:
	CrashBomb(shared_ptr<objectSprite> player, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {

		maxWeaponCount = 1;
		name = "Crash Bomb";

		bul = shared_ptr<CrashBullet>(new CrashBullet(player, t, soundCol));

		bullets.push_back(bul);

		colourP = shared_ptr<Texture>(new Texture());
		colourP->loadFromFile("Assets\\player\\crash bomb.png");

		ammoDecrease = 5;

		shootSound = soundCol->getPlayerShoot();

	}




	String getSymbol() {
		return "C";
	}
};