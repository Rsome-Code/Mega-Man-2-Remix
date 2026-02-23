#include "object sprite.cpp"
#include <SFML/audio.hpp>
#include "animation timer.cpp"
#pragma once

class Splash {
	shared_ptr<objectSprite> sprite;

	shared_ptr<animation> splashAnim;
	shared_ptr<animTimer> splashTimer;
	shared_ptr<Texture> tex;

	shared_ptr<SoundBuffer> sB;
	shared_ptr<Sound> sound;

	bool on = false;

public:
	
	Splash(shared_ptr<Texture> t) {
		tex = t;

		sB = shared_ptr<SoundBuffer> (new SoundBuffer());
		sB->loadFromFile("assets\\sound\\water_splash.wav");
		sound = shared_ptr<Sound>(new Sound());
		sound->setBuffer(*sB);
		sprite = NULL;
	}

	void start(shared_ptr<objectSprite> player) {
		sprite = shared_ptr<objectSprite>(new objectSprite(tex, IntRect(168, 2, 13, 31), Vector2f(player->getPosition().x, player->getMiddlePos().y), Vector2f(4, 4)));

		splashAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(168, 2, 13, 31), IntRect(184, 2, 13, 31), IntRect(201, 2, 16, 31), IntRect(223, 2, 6, 31)}, sprite));
		splashTimer = shared_ptr<animTimer> (new animTimer(splashAnim, 8, false));

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

	shared_ptr<objectSprite> getSprite() {
		return sprite;
	}
};