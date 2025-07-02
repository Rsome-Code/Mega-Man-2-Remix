#include "weapon.cpp"
#include "Atomic Fire Bullet.cpp"
#pragma once

class AtomicFire : public Weapon {

	AtomicFireB* bul;

public:
	AtomicFire(objectSprite* player, Texture* t) {
		bul = new AtomicFireB(player, t);
		bullets = {bul};
		maxWeaponCount = 1;
		colourP = new Texture();
		colourP->loadFromFile("Assets\\player\\Atomic fire.png");

		maxAmmo = 5;
		ammo = 5;
		name = "Atomic Fire";
	}

	bool hold(float* deltaT) {
		if (checkAmmo()) {
			bullet* temp = *bullets.begin();
			temp->hold(deltaT);
			return true;
		}
		return false;
	}

	bool release(bool right) {
		if (checkAmmo()) {
			
			bullet* temp = *bullets.begin();
			if (temp->release(right)) {
				ammo--;
				return true;
			}
		}
		return false;

	}

	bool fire(bool right) {
		return false;
	}

	bool uniqueFire() {
		return false;
	}

	float getHoldTime() {
		bullet* temp = *bullets.begin();
		return temp->getHoldTime();
	}
	float getMaxHoldTime() {
		bullet* temp = *bullets.begin();
		return temp->getMaxHoldTime();
	}

	void resetHold() {
	
		bul->resetHoldTime();
	}
};