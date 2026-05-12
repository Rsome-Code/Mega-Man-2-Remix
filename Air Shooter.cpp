#include "weapon.cpp"
#include "air bullet.cpp"
#pragma once

class AirShooter : public Weapon {

	shared_ptr<Sound> shootSound;

public:

	AirShooter(shared_ptr<objectSprite> player, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {

		shootSound = soundCol->getAir();

		maxWeaponCount = 3;

		int speed = 500;

		for (int i = 0; i<maxWeaponCount; i++) {
			shared_ptr<AirBullet> bul = shared_ptr<AirBullet>(new AirBullet(t, player, soundCol, speed));
			speed = speed + 100;
			bullets.push_back(bul);
		}
		name = "Air Shooter";

		colourP = shared_ptr<Texture>(new Texture());
		colourP->loadFromFile("Assets\\player\\Air Shooter.png");
	}
	string getTextName() {
		return "Air Shooter";
	}

	String getSymbol() {
		return "A";
	}

	bool fire(bool right) {

		bool fired = false;



		if (checkAmmo()) {

			bool allShoot = true;


			for (shared_ptr<bullet> b : bullets) {
				if (b->getShooting()) {
					allShoot = false;
				}
			}

			if (allShoot) {

				for (shared_ptr<bullet> b : bullets) {

					b->start(right);
					shootSound->play();
					fired = true;
					ammo = ammo - ammoDecrease;
					if (ammo < 0) {
						ammo = 0;
					}

					

				}
			}



		}


		return fired;

	}
};