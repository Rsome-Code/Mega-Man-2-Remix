#include "object sprite.cpp"
#include <SFML/audio.hpp>
#include "animation timer.cpp"
#pragma once

class Splash {
	objectSprite* sprite;

	animation* splashAnim;
	animTimer* splashTimer;
	Texture* tex;

	SoundBuffer* sB;
	Sound* sound;

	bool on = false;

public:
	
	Splash(Texture* t) {
		tex = t;

		sB = new SoundBuffer();
		sB->loadFromFile("assets\\sound\\water_splash.wav");
		sound = new Sound();
		sound->setBuffer(*sB);
		sprite = NULL;
	}

	void start(objectSprite* player) {
		sprite = new objectSprite(tex, IntRect(168, 2, 13, 31), Vector2f(player->getPosition().x, player->getMiddlePos().y), Vector2f(4, 4));

		splashAnim = new animation(list<IntRect>{IntRect(168, 2, 13, 31), IntRect(184, 2, 13, 31), IntRect(201, 2, 16, 31), IntRect(223, 2, 6, 31)}, sprite);
		splashTimer = new animTimer(splashAnim, 8, false);

		sound->play();
		on = true;
	}

	void eachFrame(float* deltaT) {
		if (on) {
			splashTimer->run(deltaT);
			if (splashTimer->isFinished()) {
				on = false;
				sprite = NULL;
			}
			
		}
	}

	objectSprite* getSprite() {
		return sprite;
	}
};