#include "tile.cpp"
#include "gobject.cpp"
#pragma once

class TileWithObject : public tile {
protected:
	list<objectSprite*> objects;

	list<objectSprite*> getInternalSprites() {
		return objects;
	}
};