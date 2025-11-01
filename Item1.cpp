#include "Item1 Bullet.cpp"
#include "Item Weapon.cpp"
#pragma once

class Item1 : public ItemWeapon {
	using ItemWeapon::ItemWeapon;
	Texture* t;


public:

	Item1(objectSprite* o, Texture* te) {
		origin = o;
		t = te;

		name = "Item-1";
		colourP = new Texture();
		colourP->loadFromFile("Assets\\player\\item.png");

		ammoDecrease = 4;
		maxWeaponCount = 3;
		bulCode = "item 1";
	}

	bool fire(bool right, list<ItemBullet*>* allItems) {

		if (ammo > 0 && weaponCount < maxWeaponCount) {
			float offset;
			if (right) {
				offset = 80;
			}
			else {
				offset = -24;
			}
			Item1Bullet* temp = new Item1Bullet(t, Vector2f(origin->getPosition().x + offset, origin->getPosition().y + 38));
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