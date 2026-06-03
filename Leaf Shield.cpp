#include "Weapon.cpp"
#include "Leaf Bullet.cpp"
#pragma once

class LeafShield :public Weapon {

	list<shared_ptr<LeafBullet>> leafList;

	enum Direction {
		right, downRight, down, downLeft, left, upLeft, up, upRight, neutral
	};

	Direction direct;

public:
	LeafShield(shared_ptr<objectSprite> player, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol, Vector2f relCPoint) {
		leafList = { shared_ptr<LeafBullet>(new LeafBullet(player, t, relCPoint)), shared_ptr<LeafBullet>(new LeafBullet(player, t, relCPoint)), shared_ptr<LeafBullet>(new LeafBullet(player, t, relCPoint)), shared_ptr<LeafBullet>(new LeafBullet(player, t, relCPoint)) };

		for (shared_ptr<LeafBullet> b : leafList) {
			b->dinkSetup(soundCol);
			bullets.push_back(b);
		}

		//maxWeaponCount = 3;

		name = "Leaf Shield";
		
		

		shootSound = soundCol->getLeaf();
		direct = neutral;

		colourP = shared_ptr<Texture>(new Texture());
		colourP->loadFromFile("Assets\\player\\Leaf Shield.png");

		ammoDecrease = 5;
	}

	String getSymbol() {
		return "W";
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

	virtual bool fire(bool right) {

		bool fired = false;

		

		
			bool shooting = false;

			for (shared_ptr<bullet> b : bullets) {
				if (b->getShooting()) {
					shooting = true;
					break;
				}
			}

			if (!shooting) {
				if (checkAmmo()) {
					int i = 0;
					for (shared_ptr<LeafBullet> b : leafList) {
						b->start(right);
						b->setCorner(i);
						shootSound->play();
						fired = true;

						i++;
					}
					ammo = ammo - ammoDecrease;
					if (ammo < 0) {
						ammo = 0;
					}
				}
			}
			else {
				for (shared_ptr<LeafBullet> b : leafList) {
					
					b->secondFire(checkAngle(right));

				}
			}
			

			//I'm not sure why this is here
			//fired = uniqueFire(fired);


		


		return fired;

	}
};