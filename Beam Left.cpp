#include "beam.cpp"
#pragma once

class BeamLeft : public Beam {
	using Beam::Beam;

	void setCode() {
		code = "beam left";
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		if (start) {
			float thisFrame = speed * *deltaT;

			float pos = sprite->getEndPosition().x;

			sprite->setRect(IntRect(sprite->getRect().left - thisFrame, 0, sprite->getRect().width + (thisFrame), 16));
			sprite->setRightPosition(Vector2f(pos, sprite->getPosition().y));
			hit->setRelativeRect(IntRect(0, 0, sprite->getRect().width, 16));

			collCheck(tileList);
		}
	}

	void collCheck(list<shared_ptr<tile>>* tileList) {
		for (shared_ptr<tile> t : *tileList) {
			if (t->getRight() != NULL) {
				if (hitboxDetect::hitboxDetection(Vector2f(sprite->getPosition().x, sprite->getMiddlePos().y), t->getRight())) {
					start = false;
				}
			}
		}
	}

};