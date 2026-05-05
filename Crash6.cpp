#include "crash tile.cpp"
#pragma once

class Crash6 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(5);

		above = true;
		right = false;
		below = false;
		left = true;
	}
};