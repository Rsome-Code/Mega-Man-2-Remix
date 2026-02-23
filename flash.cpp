#include "object.cpp"

#pragma once

class Flash {
	shared_ptr<object> ob;

	float flashTime = 0.01666666;
	float flashTime_left = flashTime;

public:

	Flash(shared_ptr<object> o) {
		ob = o;
	}

	void eachFrame(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			flashTime_left = flashTime;
			ob->setDisplay(!ob->getDisplay());
		}
	}

	void reset() {
		flashTime_left = flashTime;
		ob->setDisplay(true);
	}
};