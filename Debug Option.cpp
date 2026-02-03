#include "player.cpp"
#include "render logic.cpp"
#pragma once

class DebugOption {

protected:
	string name;

public:
	virtual void action(player* p, renderer* instance, double* targetFPS) = 0;

	string getName() {
		return name;
	}

	virtual void update(renderer* instance, double* fps) {}
};