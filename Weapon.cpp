#include "movable object.cpp"
#include "object hitbox.cpp"
#include "bullet.cpp"
#pragma once

class Weapon {
protected:
	list<bullet*> bullets;
	
	bool fireReady = true;
	int weaponCount = 0;
	int maxWeaponCount;

	Texture* colourP;

public:
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

	bool fire(bool right) {
		bool fired = false;
		if (fireReady) {
			weaponCount++;
			
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
	virtual void hold(float* deltaT) {};
	virtual bool release(bool right) { return false; };

	Texture* getTexture() {
		return colourP;
	}
	
};