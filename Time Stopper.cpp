#include "weapon.cpp"
#include "time stopper bullet.cpp"
#pragma once

class TimeStopper: public Weapon {

	float decreaseTime = 0.5;
	float timeLeft = decreaseTime;
	shared_ptr<TimeStopBullet> tBullet;

public:
	TimeStopper(shared_ptr<Texture> t) {
		
		tBullet = shared_ptr<TimeStopBullet>(new TimeStopBullet(t));
		maxWeaponCount = 1;
		ammoDecrease = 1;

		name = "Time Stopper";

		colourP = shared_ptr<Texture> (new Texture());
		colourP->loadFromFile("Assets\\player\\" + name + ".png");

		shootB = shared_ptr<SoundBuffer> (shared_ptr<SoundBuffer> (new SoundBuffer()));
		shootB->loadFromFile("Assets\\sound\\time_stopper.wav");
		shootSound = shared_ptr<Sound> (new Sound());
		shootSound->setBuffer(*shootB);
	}

	bool checkThrow() {
		return true;
	}

	void eachFrame(float* deltaT) {
		

		if (tBullet->getShooting()) {
			timeLeft -= *deltaT;
			if (timeLeft <= 0) {
				timeLeft = decreaseTime;
				ammo -= ammoDecrease;
			}
			tBullet->eachFrame(deltaT);

			if (ammo == 0){
				weaponCount -= 1;
				tBullet->stop();
			}
		}
		
		
		
	}

	list<shared_ptr<bullet>> getBullets() {
		return list<shared_ptr<bullet>> {tBullet};
	}

	bool fire(bool right) {
		if (!tBullet->getShooting() && ammo > 0) {
			shootSound->play();
			tBullet->start(true);
			return true;
		}
		return false;
	}

	String getSymbol() {
		return "F";
	}
};