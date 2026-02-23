#include "spawn area.cpp"
#pragma once

class SpawnOnTop : public SpawnArea {

	using SpawnArea::SpawnArea;

protected:
	Vector2f getSpawnPos(shared_ptr<camera> cam) {
		int xChance = rand();
		xChance = (xChance % 960) + 960;

		return Vector2f(xChance + cam->getPosition().x, cam->getPosition().y);
	}
};