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

			collCheck(tileList);
		}
	}

	void collCheck(list<tile*>* tileList) {
		for (tile* t : *tileList) {
			if (t->getLeft() != NULL) {
				if (hitboxDetect::hitboxDetection(Vector2f(sprite->getEndPosition().x, sprite->getMiddlePos().y), t->getLeft())) {
					start = false;
				}
			}
		}
	}
};