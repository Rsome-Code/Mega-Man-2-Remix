#include "debug option.cpp"
#pragma once

class FPSChange: public DebugOption {

public:
	FPSChange(double* fps) {
		name = string("Change FPS: " + to_string(int(*fps)));
	}

	void action(shared_ptr<player> p, shared_ptr<renderer> instance, double* targetFPS) {

		*targetFPS = *targetFPS + 30;

		if (*targetFPS > 180) {
			*targetFPS = 60;
		}

		name = string("Change FPS: " + to_string(int(*targetFPS)));
	}
};