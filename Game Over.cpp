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
	UISprite* background;

	float timeLeft = 5;
	Music* music;

public:
	GameOver() {
		Texture* t = new Texture();
		t->loadFromFile("assets\\NES - Mega Man 2 - Miscellaneous - Menus.png");

		background = new UISprite(t, IntRect(520, 282, 520, 288), Vector2f(0, 0), Vector2f(4, 4));

		music = new Music();
		music->openFromFile("assets\\sound\\music\\game over.mp3");
		music->setVolume(50);

	}

	void loop(renderer* instance, float targetRate) {
		timer* time = new timer();


		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		bool run = true;

		music->play();

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