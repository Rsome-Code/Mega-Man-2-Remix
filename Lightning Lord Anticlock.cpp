#include "lightning lord.cpp"
#pragma once

class LightningLordAnticlock: public LightningLord {

	using LightningLord::LightningLord;

	float angle = 90;

	void move(float* deltaT) {
		angle = angle - (angleChange * *deltaT);

		mov->move(angle, deltaT, speed);
	}

	void setCode() {
		code = "lightning lord anticlock";
	}
};