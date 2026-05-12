#include "movable object.cpp"
#include "object hitbox.cpp"
#include "bullet.cpp"
#include "item bullets.cpp"
#include <SFML/audio.hpp>
#pragma once

class Weapon {
protected:
	list<shared_ptr<bullet>> bullets;
	
	bool fireReady = true;
	int weaponCount = 0;
	int maxWeaponCount;

	float ammo = 28;
	int maxAmmo = 28;
	float ammoDecrease = 1;

	shared_ptr<Texture> colourP;

	string name;

	shared_ptr<Sound> shootSound;
	shared_ptr<SoundBuffer> shootB;

public:


	void tileColl(list<shared_ptr<tile>> tileList) {
		for (shared_ptr<bullet> bull : bullets) {
			bull->tileCollision(tileList);
		}
	}


	string getName() {
		return name;
	}

	virtual string getTextName() {
		return getName();
	}

	virtual void eachFrame(float* deltaT) {
		for (shared_ptr<bullet> b : bullets) {
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

		for (shared_ptr<bullet> b : bullets) {
			if (b->getShooting()) {
				b->secondFire();
			}
		}

		if (checkAmmo()) {

			for (shared_ptr<bullet> b : bullets) {
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
		

		return fired;
	
	}

	virtual list<shared_ptr<bullet>> getBullets() {
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

	shared_ptr<Texture> getTexture() {
		return colourP;
	}

	int getMAxAmmo() {
		return maxAmmo;
	}
	int getMaxAmmo() {
		return maxAmmo;
	}

	virtual void stopSound() {}

	void shootReset() {
		for (shared_ptr<bullet> b : bullets) {
			b->shootReset();
		}
	}

	virtual String getSymbol() = 0;

	virtual bool fire(bool right, list<shared_ptr<ItemBullet>>* allItems) {
		return false;
	};

	virtual void checkCount(list<shared_ptr<ItemBullet>> allItems) {}

	virtual bool checkThrow() {
		return false;
	}

	virtual void checkDirection(bool up, bool right, bool down, bool left) {

	}
	
};