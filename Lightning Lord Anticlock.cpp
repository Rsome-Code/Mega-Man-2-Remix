#include "lightning lord.cpp"
#pragma once

class LightningLordAnticlock: public LightningLord {

	using LightningLord::LightningLord;

	float angle = 270;

	void move(float* deltaT) {
		angle = angle - (angleChange * *deltaT);

		mov->move(angle, deltaT, speed);
	}

	void setCode() {
		code = "lightning lord anticlock";
	}

	void spawnObject(list<shared_ptr<GameObject>>* objects, shared_ptr<SoundCollection> soundCol) {

		bool spawn = true;

		for (shared_ptr<GameObject> ob : *objects) {
			if (ob->getCode() == "fly platform") {
				spawn = false;
				break;
			}
		}

		if (spawn) {

			shared_ptr<FlyPlatform> temp = shared_ptr<FlyPlatform>(new FlyPlatform(sprite->getTexture(), Vector2f(mov->getPosition().x + 14 * 4, mov->getPosition().y + 24 * 4)));
			temp->setMoveInCircle(angle, -angleChange, speed);

			objects->push_back(temp);
		}
	}
};