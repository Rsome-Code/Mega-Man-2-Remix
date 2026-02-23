#include "weapon.cpp"
#include "metal blade bullet.cpp"
#pragma once


class MetalBlade : public Weapon {

	list<shared_ptr<MetalBladeBullet>> bullets;

	enum Direction {
		right, downRight, down, downLeft, left, upLeft, up, upRight, neutral
	};

	Direction direct;
public:
	MetalBlade(shared_ptr<objectSprite> p, shared_ptr<Texture> tex, shared_ptr<SoundCollection> soundCol) {
		direct = neutral;

		for (int i = 0; i < 3; i++) {
			shared_ptr<MetalBladeBullet> temp = shared_ptr<MetalBladeBullet>(new MetalBladeBullet(p, tex, soundCol));
			bullets.push_back(temp);
		}

		colourP = shared_ptr<Texture> (new Texture());
		colourP->loadFromFile("Assets\\player\\metal blade.png");

		ammoDecrease = 0.25;
		name = "Metal Blade";

		shootB = shared_ptr<SoundBuffer> (new SoundBuffer());
		shootB->loadFromFile("assets\\sound\\metal_blade.wav");
		shootSound = shared_ptr<Sound>(new Sound());
		shootSound->setBuffer(*shootB);

		name = "metal blade";
	}



	shared_ptr<Sound> getSound() {
		return shootSound;
	}

	bool fire(bool right) {

		bool fired = false;

		if (checkAmmo()) {

			for (shared_ptr<MetalBladeBullet> b : bullets) {
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
		for (shared_ptr<MetalBladeBullet> b : bullets) {
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

	virtual list<shared_ptr<bullet>> getBullets() {
		list<shared_ptr<bullet>> b;

		for (shared_ptr<bullet> bul : bullets) {
			b.push_back(bul);
		}

		return b;
	}
};