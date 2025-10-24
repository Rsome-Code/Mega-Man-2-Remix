#include "solid tile.cpp"
#pragma once

class DisTile : public solidTile {

	using solidTile::solidTile;

	bool checkDist() {
		return false;
	}
};