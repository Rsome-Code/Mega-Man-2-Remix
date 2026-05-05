#include "crash tile.cpp"
#pragma once

class Crash7 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(6);

		above = true;
		right = false;
		below = true;
		left = true;
	}
};