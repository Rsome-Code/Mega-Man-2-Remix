#include "enemy.cpp"

#pragma once

class Beam :public enemy {

	using enemy::enemy;


	
protected:
	int speed = 420;

	bool start = false;

public:

	void initial() {
		hp = 1000;
		damage = 100;

		//mov->setTexture

		mov->applyRect(IntRect(0, 0, 16, 16));
		mov->setPosition(initialPos);

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(-9999, 0, 0, 0), sprite));
		setCode();


	}

	void setSoundPointer(shared_ptr<Sound> sou) {
		sound = sou;
	}

	void setTexture(shared_ptr<Texture> tex) {
		sprite->setTexture(tex);
	}

	void moveStart() {
		start = true;
		sound->play();
	}

	bool deleteOverX() {
		return false;
	}
};