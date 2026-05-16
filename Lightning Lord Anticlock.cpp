#include "lightning lord.cpp"
#pragma once

class LightningLordAnticlock: public LightningLord {

	float angle = 90;

	void move(float* deltaT) {
		angle = angle - angleChange;

		mov->move(angle, deltaT, speed);
	}

};