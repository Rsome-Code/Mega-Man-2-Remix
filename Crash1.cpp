#include "crash tile.cpp"
#pragma once

class Crash1: public CrashTile {

	using CrashTile::CrashTile;
	
public:
	void crashSetup() {
		
		setTileNum(0);
		above = false;
		right = true;
		below = true;
		left = false;
	}
};