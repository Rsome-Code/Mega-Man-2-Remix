#include "object sprite.cpp"
#include "object.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#include "physics Object.cpp"
#include "tile.cpp"
#include "hitbox detector.cpp"
#pragma once

class Item :public object {
protected:
	objectHitbox* hit;
	animation* anim;
	animTimer* timer = NULL;
	physicsObject* phys;
	bool grounded = false;

	objectHitbox* gHit = NULL;

	bool hC = false;
	

public:


	objectHitbox* getHitbox() {
		return hit;
	}

	void used() {
		hit->setPosition(Vector2f(0, 0));
		act = false;
		display = false;
	}


	virtual void eachFrame(float* deltaT, objectSprite* p, list<tile*>* tiles) {
		hit->updatePos();
		if (timer != NULL) {
			timer->run(deltaT);
		}

		if (!grounded) {
			gravLoop(tiles, deltaT);
		}
	}

	void gravLoop(list<tile*>* tiles, float* deltaT) {
		phys->enableGravity(true);
		phys->eachFrame(deltaT);
		for (tile* t : *tiles) {
			if (t->getGround() != NULL) {
				groundCheck(t);
			}
		}
	}

	void groundCheck(tile* t) {
		if (!hC) {
			//gHit = new objectHitbox(IntRect(hit->getRelativeRect().getPosition(), Vector2i(hit->getRelativeRect().getSize().x, hit->getRelativeRect().getPosition().y + 16)), phys);
			hC = true;
		}
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			grounded = true;
			phys->enableGravity(false);
			phys->setVVelocity(0);
			phys->setPosition(Vector2f(sprite->getPosition().x, t->getGround()->getPosition().y - sprite->getSize().y));
		}
	}
};