#include "beam.cpp"
#pragma once

class BeamRight : public Beam {
	using Beam::Beam;

	void setCode() {
		code = "beam right";
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		
		if (start) {
			float thisFrame = speed * *deltaT;
			sprite->setRect(IntRect(0, 0, sprite->getRect().width + thisFrame, 16));
			hit->setRelativeRect(IntRect(0, 0, sprite->getRect().width, 16));
		}
	}
};