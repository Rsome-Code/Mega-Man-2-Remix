#include "crash tile.cpp"
#pragma once

class Crash11 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(10);

		above = false;
		right = false;
		below = false;
		left = true;
	}
};