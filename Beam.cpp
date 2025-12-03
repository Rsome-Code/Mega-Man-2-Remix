#include "enemy.cpp"

#pragma once

class Beam :public enemy {

	using enemy::enemy;


	
protected:
	int speed = 200;

	bool start = false;

public:

	void initial() {
		hp = 1000;
		damage = 100;

		//mov->setTexture

		mov->applyRect(IntRect(0, 0, 16, 16));
		mov->setPosition(initialPos);

		hit = new objectHitbox(IntRect(0, 0, 16, 16), sprite);
		hurt = new objectHitbox(IntRect(-9999, 0, 0, 0), sprite);
		setCode();


	}

	void setSoundPointer(Sound* sou) {
		sound = sou;
	}

	void setTexture(Texture* tex) {
		sprite->setTexture(tex);
	}

	void moveStart() {
		start = true;
		//sound->play();
	}
};