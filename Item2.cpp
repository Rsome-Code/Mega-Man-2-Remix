#include "Item2 Bullet.cpp"
#include "Item Weapon.cpp"
#pragma once

class Item2 : public ItemWeapon {
	using ItemWeapon::ItemWeapon;
	shared_ptr<Texture> t;


public:

	Item2(shared_ptr<objectSprite> o, shared_ptr<Texture> te) {
		origin = o;
		t = te;

		name = "Item-2";
		colourP = shared_ptr<Texture>(new Texture());
		colourP->loadFromFile("Assets\\player\\item.png");

		ammoDecrease = 4;
		maxWeaponCount = 3;
		bulCode = "item 2";
	}

	bool fire(bool right, list<shared_ptr<ItemBullet>>* allItems) {

		if (ammo > 0 && weaponCount < maxWeaponCount) {
			float offset;
			if (right) {
				offset = 80 + (10 * 4);
			}
			else {
				offset = -24 - (22 * 4);
			}
			shared_ptr<Item2Bullet> temp = shared_ptr<Item2Bullet>(new Item2Bullet(t, Vector2f(origin->getPosition().x + offset, origin->getPosition().y + 38), right));
			temp->getSprite()->setPosition(Vector2f(origin->getPosition().x + offset, origin->getPosition().y + 38));
			allItems->push_back(temp);
			ammo = ammo - ammoDecrease;
			return true;
		}
		return false;
	}

	String getSymbol() {
		return "2";
	}

	bool checkThrow() {
		return true;
	}
};