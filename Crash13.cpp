#include "crash tile.cpp"
#pragma once

class Crash13 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(12);

		above = true;
		right = true;
		below = true;
		left = true;
	}
};