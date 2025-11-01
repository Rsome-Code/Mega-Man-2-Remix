#include "weapon.cpp"
#include "item bullets.cpp"
#pragma once

class ItemWeapon : public Weapon {
	using Weapon::Weapon;
protected:
	string bulCode;
	objectSprite* origin;
public:


	void checkCount(list<ItemBullet*> allItems) {
		int ammount = 0;
		for (ItemBullet* bul : allItems) {
			if (bul->getCode() == bulCode) {
				ammount++;
			}
		}
		weaponCount = ammount;
	}


};