#include "crash tile.cpp"
#pragma once

class Crash5 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(4);

		above = true;
		right = true;
		below = false;
		left = false;
	}
};