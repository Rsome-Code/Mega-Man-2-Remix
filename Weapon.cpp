#include "movable object.cpp"
#include "object hitbox.cpp"
#include "bullet.cpp"
#pragma once

class Weapon {
protected:
	list<bullet*> bullets;
	
	bool fireReady = true;
	float weaponCount = 0;
	int maxWeaponCount;

	int ammo = 28;
	int maxAmmo = 28;
	int ammoDecrease = 1;

	Texture* colourP;

	string name;

public:
	int getMaxAmmo() {
		return maxAmmo;
	}
	string getName() {
		return name;
	}
	void eachFrame(float* deltaT) {
		for (bullet* b : bullets) {
			if (b->eachFrame(deltaT)) {
				weaponCount--;
			}
		}
	}

	bool getFireReady() {
		return fireReady;
	}
	void setFireReady(bool b) {
		fireReady = b;
	}

	virtual bool fire(bool right) {
		bool fired = false;
		if (fireReady && checkAmmo()) {
			weaponCount++;
			ammo = ammo - ammoDecrease;
			if (ammo < 0) {
				ammo = 0;
			}

			for (bullet* b : bullets) {
				if (!b->getShooting()) {
					b->start(right);
					break;
				}
			}
			fired = true;
			fired = uniqueFire();
			if (weaponCount >= maxWeaponCount) {
				fireReady = false;
			}
			
		}
		else {
			secondFire();
		}
		return fired;
	}

	list<bullet*> getBullets() {
		return bullets;
	}

	virtual bool uniqueFire() { return true; };
	virtual void secondFire() {};
	virtual bool hold(float* deltaT) { return NULL; };
	virtual bool release(bool right) { return false; };
	virtual void resetHold() {};

	virtual float getHoldTime() { return NULL; };
	virtual float getMaxHoldTime() { return NULL; };

	virtual bool checkAmmo() {
		if (ammo > 0) {
			return true;
		}
		return false;
	}

	void addAmmo(int a) {
		ammo = ammo + a;
	}

	int getAmmo() {
		return ammo;
	}

	Texture* getTexture() {
		return colourP;
	}
	
};