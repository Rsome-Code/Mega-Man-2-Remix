#include "weapon.cpp"
#include "metal blade bullet.cpp"
#pragma once


class MetalBlade : public Weapon {

	list<MetalBladeBullet*> bullets;

	enum Direction {
		right, downRight, down, downLeft, left, upLeft, up, upRight, neutral
	};

	Direction direct;
public:
	MetalBlade(objectSprite* p, Texture* tex) {
		direct = neutral;

		for (int i = 0; i < 3; i++) {
			MetalBladeBullet* temp = new MetalBladeBullet(p, tex);
			bullets.push_back(temp);
		}

		colourP = new Texture();
		colourP->loadFromFile("Assets\\player\\metal blade.png");

		ammoDecrease = 0.25;
		name = "Metal Blade";

		shootB = new SoundBuffer();
		shootB->loadFromFile("assets\\sound\\metal_blade.wav");
		shootSound = new Sound();
		shootSound->setBuffer(*shootB);

		name = "metal blade";
	}


	bool fire(bool right) {

		bool fired = false;

		if (checkAmmo()) {

			for (MetalBladeBullet* b : bullets) {
				if (!b->getShooting()) {
					b->start(checkAngle(right), right);
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

	void eachFrame(float* deltaT) {
		for (MetalBladeBullet* b : bullets) {
			if (b->eachFrame(deltaT)) {

				weaponCount--;
			}
		}
	}

	int checkAngle(bool faceRight) {
		int angle;
		if (direct == neutral) {
			if (faceRight) {
				angle = 0;
			}
			else {
				angle = 180;
			}
		}
		else {
			angle = 45 * direct;
		}
		return angle;
	}


	void checkDirection(bool up, bool right, bool down, bool left) {
		if (up) {
			if (left) {
				direct = upLeft;
			}
			else if (right) {
				direct = upRight;
			}
			else {
				direct = Direction::up;
			}
		}
		else if (down) {
			if (left) {
				direct = downLeft;
			}
			else if (right) {
				direct = downRight;
			}
			else {
				direct = Direction::down;
			}
		}
		else if (right) {
			direct = Direction::right;
		}
		else if (left) {
			direct = Direction::left;
		}

		else {
			direct = neutral;
		}

	}

	String getSymbol() {
		return "M";
	}

	bool checkThrow() {
		return true;
	}

	virtual list<bullet*> getBullets() {
		list<bullet*> b;

		for (bullet* bul : bullets) {
			b.push_back(bul);
		}

		return b;
	}
};