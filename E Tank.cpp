#include "item.cpp"
#pragma once

class ETank : public Item {
	
public:
	ETank(Texture* t, Vector2f position) {
		sprite = new objectSprite("E Tank", t, IntRect(122, 1, 16, 16), position, Vector2f(4,4));
		hit = new objectHitbox(IntRect(0, 0, 16, 16), sprite);
		anim = new animation(list<IntRect>{IntRect(122, 1, 16, 16), IntRect(105, 1, 16, 16)}, sprite);
		timer = new animTimer(anim, 10, true);
	}

	void setCode() {
		code = "E Tank";
	}

	void eachFrame(float* deltaT, objectSprite* p) {
		timer->run(deltaT);
	}


	int getIncrease() { return 1; }
};