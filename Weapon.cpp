#include "movable object.cpp"
#include "object hitbox.cpp"
#include "bullet.cpp"
#include "item bullets.cpp"
#include <SFML/audio.hpp>
#pragma once

class Weapon {
protected:
	list<bullet*> bullets;
	
	bool fireReady = true;
	int weaponCount = 0;
	int maxWeaponCount;

	float ammo = 28;
	int maxAmmo = 28;
	float ammoDecrease = 1;

	Texture* colourP;

	string name;

	Sound* shootSound;
	SoundBuffer* shootB;

public:


	void tileColl(list<tile*> tileList) {
		for (bullet* bull : bullets) {
			bull->tileCollision(tileList);
		}
	}

	int getMaxAmmo() {
		return maxAmmo;
	}
	string getName() {
		return name;
	}
	virtual void eachFrame(float* deltaT) {
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

		if (checkAmmo()) {

			for (bullet* b : bullets) {
				if (!b->getShooting()) {
					b->start(right);
					shootSound->play();
					fired = true;
					ammo = ammo - ammoDecrease;
					if (ammo < 0) {
						ammo = 0;
					}
					break;
				}
			}
			
			//I'm not sure why this is here
			//fired = uniqueFire(fired);

			
		}
		else {
			secondFire();
		}
		return fired;
	
	}

	virtual list<bullet*> getBullets() {
		return bullets;
	}

	virtual bool uniqueFire(bool fired) { return fired; };
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

	void setAmmo(int a) {
		ammo = a;
	}

	float getAmmo() {
		return ammo;
	}

	Texture* getTexture() {
		return colourP;
	}

	int getMAxAmmo() {
		return maxAmmo;
	}

	virtual void stopSound() {}

	void shootReset() {
		for (bullet* b : bullets) {
			b->shootReset();
		}
	}

	virtual String getSymbol() = 0;

	virtual bool fire(bool right, list<ItemBullet*>* allItems) {
		return false;
	}

	virtual void checkCount(list<ItemBullet*> allItems) {}

	virtual bool checkThrow() {
		return false;
	}

	virtual void checkDirection(bool up, bool right, bool down, bool left) {

	}
	
};