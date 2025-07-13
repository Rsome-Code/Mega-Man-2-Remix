#include "tile.cpp"
#include "time.cpp"
#include "render Logic.cpp"
#pragma once

class Freeze {
	
public:
	static void stop(renderer* instance, float targetRate, objectSprite* player, list<tile*> tileList, list<tile*> z2List, list<tile*> z3List, list<tile*> z4List, list<object*> obList, camera* cam, float timeLeft) {

		timer* time = new timer();


		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		bool run = true;


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


			for (tile* t : z4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}



			//tileDistanceCheck(instance, tileList);

			for (tile* t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			for (object* ob : obList) {
				if (ob->getDisplay()) {
					instance->objectAccess(ob, cam);
				}
			}

			instance->objectDisplay(player, cam);

			timeLeft -= deltaT;
			if (timeLeft <= 0) {
				run = false;
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
	}
};