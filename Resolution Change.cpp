#include "debug option.cpp"
#pragma once

class ResolutionChange : public DebugOption {


public:
	ResolutionChange(renderer* instance) {

		update(instance);
	}

	void action(player* p, renderer* instance, double* targetFPS) {

		int verticalRes = instance->getWindow()->getSize().y;

		verticalRes += 360;

		if (verticalRes > 1440) {
			verticalRes = 720;
		}

		else if (verticalRes < 720) {
			verticalRes = 1440;
		}

		int horiRes = verticalRes * (float(1920) / float(1080));

		//instance->getWindow()->setSize(Vector2u(horiRes, verticalRes));


		instance->setSize(Vector2u(horiRes, verticalRes));
		
		update(instance);
		
	}

	void update(renderer* instance) {
		name = string("Change Resolution: " + to_string(instance->getWindow()->getSize().x) + "/" + to_string(instance->getWindow()->getSize().y));
	}
};