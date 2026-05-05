#include "crash tile.cpp"
#pragma once

class Crash14 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(13);

		above = false;
		right = false;
		below = true;
		left = false;
	}
};