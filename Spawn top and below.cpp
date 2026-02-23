#include "spawn on top.cpp"
#pragma once

class SpawnAboveBelow : public SpawnArea {
	using SpawnArea::SpawnArea;

protected:
	bool up = false;

protected:
	Vector2f getSpawnPos(shared_ptr<camera> cam) {

		bool above = rand() % 2;

		if (above) {
			up = true;
			return getAbove(cam);
		}
		else {
			up = false;
			return getBelow(cam);
		}
	}

protected:
	Vector2f getAbove(shared_ptr<camera> cam) {
		int xChance = rand();
		xChance = (xChance % 960) + 960;

		return Vector2f(xChance + cam->getPosition().x, cam->getPosition().y);
	}

protected:
	Vector2f getBelow(shared_ptr<camera> cam) {
		int xChance = rand();
		xChance = (xChance % 960) + 960;

		return Vector2f(xChance + cam->getPosition().x, cam->getPosition().y + 1080);
	}
};