#include "weapon.cpp"
#include "Atomic Fire Bullet.cpp"
#pragma once

class AtomicFire : public Weapon {

public:
	AtomicFire(objectSprite* player, Texture* t) {
		bullets = { new AtomicFireB(player, t)};
		maxWeaponCount = 1;
		colourP = new Texture();
		colourP->loadFromFile("Assets\\Atomic fire.png");

		maxAmmo = 5;
		ammo = 5;
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
};