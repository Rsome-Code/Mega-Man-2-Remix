#include "beam.cpp"
#pragma once

class BeamLeft : public Beam {
	using Beam::Beam;

	void setCode() {
		code = "beam left";
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {

		if (start) {
			float thisFrame = speed * *deltaT;

			float pos = sprite->getEndPosition().x;

			sprite->setRect(IntRect(sprite->getRect().left - thisFrame, 0, sprite->getRect().width + (thisFrame), 16));
			sprite->setRightPosition(Vector2f(pos, sprite->getPosition().y));
			hit->setRelativeRect(IntRect(0, 0, sprite->getRect().width, 16));
		}
	}
};