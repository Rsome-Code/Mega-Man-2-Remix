#include "item.cpp"
#pragma once

class ETank : public Item {
	
public:
	ETank(shared_ptr<Texture> t, Vector2f position) {
		
		phys = shared_ptr<physicsObject> (new physicsObject("E Tank", t, IntRect(122, 1, 16, 16), position, Vector2f(4,4)));
		sprite = phys;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), sprite));
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(122, 1, 16, 16), IntRect(105, 1, 16, 16)}, sprite));
		timer = shared_ptr<animTimer> (new animTimer(anim, 10, true));
		setCode();
	}

	void setCode() {
		code = "E Tank";
	}

	void eachFrame(float* deltaT, shared_ptr<objectSprite> p) {
		timer->run(deltaT);
	}


	int getIncrease() { return 1; }
};