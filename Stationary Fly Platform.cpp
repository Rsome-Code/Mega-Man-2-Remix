#include "fly platform.cpp"
#pragma once

class StationaryFlyPlatform :public FlyPlatform {

	using FlyPlatform::FlyPlatform;


	void checkColl(shared_ptr<player> player) {

		if (hitboxDetect::hitboxDetection(player->getFoot(), hit)) {
			state = on;
			if (!player->getGrounded()) {
				player->setGrounded(true);
				player->setGroundedOverride(true);
			}
			player->setShootemControls(true);

			player->setAutoOn(false);

		}
	}

	string getCode() {
		return "stationary fly platform";
	}

};