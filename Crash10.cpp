#include "crash tile.cpp"
#pragma once

class Crash10 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(9);

		above = false;
		right = true;
		below = false;
		left = true;
	}
};