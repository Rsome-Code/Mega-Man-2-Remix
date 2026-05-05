#include "crash tile.cpp"
#pragma once

class Crash4 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(3);

		above = true;
		right = true;
		below = false;
		left = true;
	}
};