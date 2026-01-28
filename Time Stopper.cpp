#include "weapon.cpp"
#include "time stopper bullet.cpp"
#pragma once

class TimeStopper: public Weapon {

	float decreaseTime = 0.5;
	float timeLeft = decreaseTime;
	TimeStopBullet* tBullet;

public:
	TimeStopper(Texture* t) {
		
		tBullet = new TimeStopBullet(t);
		maxWeaponCount = 1;
		ammoDecrease = 1;

		name = "Time Stopper";

		colourP = new Texture();
		colourP->loadFromFile("Assets\\player\\" + name + ".png");

		shootB = new SoundBuffer();
		shootB->loadFromFile("Assets\\sound\\time_stopper.wav");
		shootSound = new Sound();
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

	list<bullet*> getBullets() {
		return list<bullet*> {tBullet};
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