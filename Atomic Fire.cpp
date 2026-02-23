#include "weapon.cpp"
#include "Atomic Fire Bullet.cpp"
#pragma once

class AtomicFire : public Weapon {

	shared_ptr<AtomicFireB> bul;


public:
	AtomicFire(shared_ptr<objectSprite> player, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {
		bul = shared_ptr<AtomicFireB>(new AtomicFireB(player, t, soundCol));
		bullets = {bul};
		maxWeaponCount = 1;
		colourP = shared_ptr<Texture> (new Texture());
		colourP->loadFromFile("Assets\\player\\Atomic fire.png");

		ammoDecrease = 5;
		name = "Atomic Fire";

	}

	bool hold(float* deltaT) {
		if (checkAmmo()) {
			shared_ptr<bullet> temp = *bullets.begin();
			temp->hold(deltaT);
			return true;
		}
		return false;
	}

	bool release(bool right) {
		if (checkAmmo()) {
			
			shared_ptr<bullet> temp = *bullets.begin();
			if (temp->release(right)) {
				ammo = ammo - ammoDecrease;
				if (ammo < 0) {
					ammo = 0;
				}
				return true;
			}
		}
		else {
			bul->forceStop();
		}
		return false;

	}

	bool fire(bool right) {
		bul->start(false);
		return false;
	}

	bool uniqueFire() {
		return false;
	}

	float getHoldTime() {
		shared_ptr<bullet> temp = *bullets.begin();
		return temp->getHoldTime();
	}
	float getMaxHoldTime() {
		shared_ptr<bullet> temp = *bullets.begin();
		return temp->getMaxHoldTime();
	}

	void resetHold() {
	
		bul->resetHoldTime();
	}

	void stopSound() {
		bul->forceStop();
	}

	String getSymbol() {
		return "H";
	}
};