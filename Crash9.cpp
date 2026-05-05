#include "crash tile.cpp"
#pragma once

class Crash9 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(8);

		above = false;
		right = true;
		below = false;
		left = false;
	}
};