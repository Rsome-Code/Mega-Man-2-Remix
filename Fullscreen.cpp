#include "debug option.cpp"
#pragma once

class FullscreenOption : public DebugOption {

	

public:

	FullscreenOption(renderer* instance) {

		update(instance);
	}

	void update(renderer* instance) {
		name = "Fullscreen: ";
		if (instance->getFullscreen()) {
			name = name + "on";
		}
		else {
			name = name + "off";
		}
	}

	void action(player* p, renderer* instance, double* targetFPS) {
		
		Vector2u size = instance->getWindow()->getSize();


		instance->setFullscreen(!instance->getFullscreen());


		update(instance);
	}

};