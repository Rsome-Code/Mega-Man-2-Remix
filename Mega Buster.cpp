#include "weapon.cpp"
#include "mega buster bullet.cpp"

#pragma once

class MegaBuster : public Weapon {

public:
	MegaBuster(objectSprite* player, Texture* t) {
		bullets = { new megaBuster(player, t),  new megaBuster(player, t),  new megaBuster(player, t) };
		maxWeaponCount = 3;
		colourP = new Texture();
		colourP->loadFromFile("Assets\\NES - Mega Man 2 - Mega Man.png");

	}

	bool checkAmmo() {
		return true;
	}
	
};