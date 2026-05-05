#include "crash tile.cpp"
#pragma once

class Crash3 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(2);

		above = false;
		right = true;
		below = true;
		left = true;
	}
};