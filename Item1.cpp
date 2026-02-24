#include "Item1 Bullet.cpp"
#include "Item Weapon.cpp"
#pragma once

class Item1 : public ItemWeapon {
	using ItemWeapon::ItemWeapon;
	shared_ptr<Texture> t;


public:

	Item1(shared_ptr<objectSprite> o, shared_ptr<Texture> te) {
		origin = o;
		t = te;

		name = "Item-1";
		colourP = shared_ptr<Texture> (new Texture());
		colourP->loadFromFile("Assets\\player\\item.png");

		ammoDecrease = 4;
		maxWeaponCount = 3;
		bulCode = "item 1";
	}

	bool fire(bool right, list<shared_ptr<ItemBullet>>* allItems) {

		if (ammo > 0 && weaponCount < maxWeaponCount) {
			float offset;
			if (right) {
				offset = 80 + (10*4);
			}
			else {
				offset = -24 - (22*4);
			}
			shared_ptr<Item1Bullet> temp = shared_ptr<Item1Bullet>(new Item1Bullet(t, Vector2f(origin->getPosition().x + offset, origin->getPosition().y + 38)));
			temp->getSprite()->setPosition(Vector2f(origin->getPosition().x + offset, origin->getPosition().y + 38));
			allItems->push_back(temp);
			ammo = ammo - ammoDecrease;
			return true;
		}
		return false;
	}

	String getSymbol() {
		return "1";
	}

	bool checkThrow() {
		return true;
	}
};