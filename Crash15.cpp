#include "crash tile.cpp"
#pragma once

class Crash15 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(14);

		above = true;
		right = false;
		below = true;
		left = false;
	}
};