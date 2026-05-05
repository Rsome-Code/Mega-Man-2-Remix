#include "crash tile.cpp"
#pragma once

class Crash16 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(15);

		above = true;
		right = false;
		below = false;
		left = false;
	}
};