#include "player.cpp"
#include "render logic.cpp"
#pragma once

class DebugOption {

protected:
	string name;

public:
	virtual void action(shared_ptr<player> p, shared_ptr<renderer> instance, double* targetFPS) = 0;

	string getName() {
		return name;
	}

	virtual void update(shared_ptr<renderer> instance, double* fps) {}
};