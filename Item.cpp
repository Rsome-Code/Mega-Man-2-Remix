#include "object sprite.cpp"
#include "GObject.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#include "physics Object.cpp"
#include "tile.cpp"
#include "hitbox detector.cpp"
#pragma once

class Item :public GameObject {
protected:
	shared_ptr<objectHitbox> hit;
	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer = NULL;
	shared_ptr<physicsObject> phys;
	bool grounded = false;

	shared_ptr<objectHitbox> gHit = NULL;

	bool hC = false;
	
	shared_ptr<Sound> colSound = NULL;

public:


	shared_ptr<objectHitbox> getHitbox() {
		return hit;
	}

	void used() {
		if (colSound != NULL) {
			colSound->play();
		}
		hit->setPosition(Vector2f(0, 0));
		act = false;
		display = false;
	}


	virtual void eachFrame(float* deltaT, shared_ptr<objectSprite> p, list<shared_ptr<tile>>* tiles) {
		hit->updatePos();
		if (timer != NULL) {
			timer->run(deltaT);
		}

		if (!grounded) {
			gravLoop(tiles, deltaT);
		}
	}

	void gravLoop(list<shared_ptr<tile>>* tiles, float* deltaT) {
		phys->enableGravity(true);
		phys->eachFrame(deltaT);
		for (shared_ptr<tile> t : *tiles) {
			if (t->getGround() != NULL) {
				groundCheck(t);
			}
		}
	}

	void groundCheck(shared_ptr<tile> t) {
		if (!hC) {
			//gHit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(hit->getRelativeRect().getPosition(), Vector2i(hit->getRelativeRect().getSize().x, hit->getRelativeRect().getPosition().y + 16)), phys);
			hC = true;
		}
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			grounded = true;
			phys->enableGravity(false);
			phys->setVVelocity(0);
			phys->setPosition(Vector2f(sprite->getPosition().x, t->getGround()->getPosition().y - sprite->getSize().y));
		}
	}

	void setSoundPointer(shared_ptr<Sound> sou) {
		colSound = sou;
	}
};