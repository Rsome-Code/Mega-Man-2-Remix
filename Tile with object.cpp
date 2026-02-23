#include "tile.cpp"
#include "gobject.cpp"
#pragma once

class TileWithObject : public tile {
protected:
	list<shared_ptr<objectSprite>> objects;

	list<shared_ptr<objectSprite>> getInternalSprites() {
		return objects;
	}

	virtual void deleteInt() {

	}
};