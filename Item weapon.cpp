#include "weapon.cpp"
#include "item bullets.cpp"
#pragma once

class ItemWeapon : public Weapon {
	using Weapon::Weapon;
protected:
	string bulCode;
	shared_ptr<objectSprite> origin;
public:


	void checkCount(list<shared_ptr<ItemBullet>> allItems) {
		int ammount = 0;
		for (shared_ptr<ItemBullet> bul : allItems) {
			if (bul->getCode() == bulCode) {
				ammount++;
			}
		}
		weaponCount = ammount;
	}


};