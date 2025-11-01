#include "solid tile.cpp"
#pragma once

class ObTile : public solidTile {

	using solidTile::solidTile;

	bool checkDist() {
		return false;
	}
};