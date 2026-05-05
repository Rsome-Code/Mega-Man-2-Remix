#include "crash tile.cpp"
#pragma once

class Crash12 : public CrashTile {

	using CrashTile::CrashTile;

	void crashSetup() {

		setTileNum(11);

		above = true;
		right = true;
		below = true;
		left = true;
	}
};