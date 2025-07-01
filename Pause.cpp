#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>
#include "UI Sprite.cpp"
#include "Weapon Option.cpp"
#include "animation timer.cpp"
#include "render logic.cpp"
#include "time.cpp"
#include "player.cpp"
#pragma once

class Pause {
	Vector2f position = Vector2f(700, 200);
	player* p;
	UISprite* background;
	animation* startAnim;
	animTimer* startTime;
	animation* endAnim;
	animTimer* endTime;
	timer* time;

	WeaponOption* megaBuster;
	WeaponOption* atomicFire;

	int width = 96;
	int height = 160;

public:

	Pause(string level, player* p) {
		Texture* t = new Texture;
		this->p = p;
		t->loadFromFile("Assets\\pause\\" + level + ".png");
		background = new UISprite("background", t, IntRect(1, 11, width, height), position, Vector2f(4,4));
		startAnim = new animation(list<IntRect>{IntRect(1, 11, width, height), IntRect(98, 11, width, height), IntRect(195, 11, width, height), IntRect(292, 11, width, height), IntRect(389, 11, width, height), IntRect(486, 11, width, height), IntRect(583, 11, width, height), IntRect(680, 11, width, height), IntRect(777, 11, width, height), IntRect(874, 11, width, height), IntRect(971, 11, width, height), IntRect(1068, 11, width, height), IntRect(1165, 11, width, height), IntRect(1262, 11, width, height), IntRect(1359, 11, width, height)}, background);
		endAnim = new animation(list<IntRect>{IntRect(1, 186, width, height), IntRect(98, 186, width, height), IntRect(195, 186, width, height), IntRect(292, 186, width, height), IntRect(389, 186, width, height), IntRect(486, 186, width, height), IntRect(583, 186, width, height), IntRect(680, 186, width, height), IntRect(777, 186, width, height), IntRect(874, 186, width, height), IntRect(971, 186, width, height), IntRect(1068, 186, width, height), IntRect(1165, 186, width, height), IntRect(1262, 186, width, height), IntRect(1359, 186, width, height)}, background);
		startTime = new animTimer(startAnim, 20, false);
		endTime = new animTimer(endAnim, 20, false);
		time = new timer();

		megaBuster = new WeaponOption(p->getMegaBuster(), Vector2f(position.x + 9*4, position.y + 17 * 4));
		megaBuster->getBar()->update(p->getHP());
		atomicFire = new WeaponOption(p->getAtomicFire(), Vector2f(position.x + 9*4, position.y + 26 * 4));
	}

	void loop(renderer* instance, float targetRate) {
		bool startA = true;
		bool end = false;
		bool run = true;

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

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

			instance->UIDisplay(background);
			if (startA) {
				if (runStart(&deltaT)) {
					startA = false;
				}
			}
			else if (end) {
				if (runEnd(&deltaT)) {
					run = false;
				}
			}
			else {
				if (runMenu(&deltaT, instance)) {
					end = true;
				}
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
	}

	bool runStart(float* deltaT) {
		startTime->run(deltaT);
		return startTime->isFinished();
	}
	bool runEnd(float* deltaT) {
		return endTime->run(deltaT);
	}

	bool runMenu(float* deltaT, renderer* instance) {
		instance->UIDisplay(megaBuster->getSprites());
		instance->UIDisplay(atomicFire->getSprites());
		return false;
	}
};