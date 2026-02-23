#include "debug option.cpp"
#pragma once

class FullscreenOption : public DebugOption {

	

public:

	FullscreenOption(shared_ptr<renderer> instance) {

		update(instance);
	}

	void update(shared_ptr<renderer> instance) {
		name = "Fullscreen: ";
		if (instance->getFullscreen()) {
			name = name + "on";
		}
		else {
			name = name + "off";
		}
	}

	void action(shared_ptr<player> p, shared_ptr<renderer> instance, double* targetFPS) {
		
		Vector2u size = instance->getWindow()->getSize();


		instance->setFullscreen(!instance->getFullscreen());


		update(instance);
	}

};