#include "item.cpp"
#pragma once

class ExtraLife : public Item {

public:
	ExtraLife(Texture* t, Vector2f position) {
		sprite = new objectSprite("Extra Life", t, IntRect(139, 2, 16, 15), position, Vector2f(4, 4));
		hit = new objectHitbox(IntRect(0, 0, 16, 15), sprite);

	}

	void setCode() {
		code = "Extra Life";
	}

	void eachFrame(float* deltaT, objectSprite* p) {
		//timer->run(deltaT);
	}


	int getIncrease() { return 1; }
};