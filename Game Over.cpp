#include "UI Sprite.cpp"
#include "render logic.cpp"
#include "time.cpp"
#include "Render Logic.cpp"
#include "Camera.cpp"
#include "controller.cpp"
#include "Weapon.cpp"
#include "xcontrol.cpp"
#include <SFML/audio.hpp>
#pragma once

class GameOver {
	shared_ptr<UISprite> background;

	float timeLeft = 5;
	shared_ptr<Music> music;

public:

	virtual ~GameOver() {

	}

	GameOver() {
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\NES - Mega Man 2 - Miscellaneous - Menus.png");

		background = shared_ptr<UISprite>(new UISprite(t, IntRect(520, 282, 520, 288), Vector2f(0, 0), Vector2f(4, 4)));

		music = shared_ptr<Music>(new Music());
		music->openFromFile("assets\\sound\\music\\game over.mp3");
		music->setVolume(50);

	}

	void loop(shared_ptr<renderer> instance, float targetRate) {
		shared_ptr<timer> time = shared_ptr<timer>(new timer());


		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		bool run = true;

		music->play();

		timeLeft = 5;

		while (instance->getWindow()->isOpen() && run) {
			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();
			}
			time->frameLimiter(targetRate, startP);
			deltaT = time->checkTimer(startP);
			start = time->timerStart();
			startP = &start;

			run = (!music->getStatus() == Music::Stopped);

			instance->UIDisplay(background);
			instance->getWindow()->display();
			instance->getWindow()->clear();
		}



	}
};