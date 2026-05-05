#include "crash tile.cpp"
#pragma once

class Crash2 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {
		
		setTileNum(1);

		above = false;
		right = false;
		below = true;
		left = true;
	}
};