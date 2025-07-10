#include "object sprite.cpp"
#include "object.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#pragma once

class Item :public object {
protected:
	objectHitbox* hit;
	animation* anim;
	animTimer* timer = NULL;

public:


	objectHitbox* getHitbox() {
		return hit;
	}

	void used() {
		hit->setPosition(Vector2f(0, 0));
		act = false;
		display = false;
	}


	virtual void eachFrame(float* deltaT, objectSprite* p) {
		if (timer != NULL) {
			timer->run(deltaT);
		}
	}
};