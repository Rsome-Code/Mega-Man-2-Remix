#include "crash tile.cpp"
#pragma once

class Crash8 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(7);

		above = true;
		right = true;
		below = true;
		left = false;
	}
};