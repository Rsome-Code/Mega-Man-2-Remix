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
	}

	bool hold(float* deltaT) {
		bullet* temp = *bullets.begin();
		temp->hold(deltaT);
		return true;
	}

	bool release(bool right) {
		bullet* temp = *bullets.begin();
		return temp->release(right);

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