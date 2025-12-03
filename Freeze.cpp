#include "tile.cpp"
#include "time.cpp"
#include "render Logic.cpp"
#include "enemy.cpp"
#include "enemy bullet.cpp"
#pragma once

class Freeze {
	
public:
	static void stop(renderer* instance, float targetRate, player* player, list<tile*> tileList, list<tile*> z2List, list<tile*> z3List, list<tile*> z4List, list<GameObject*> obList, list<enemy*> eList, list<EnemyBullet*> eBList, list<GameObject*> backgroundObjects, camera* cam, float timeLeft) {

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

			for (object* ob : backgroundObjects) {
				instance->objectAccess(ob, cam);
			}

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
				if (ob->getDisplay() && ob->getSprite() != NULL) {
					instance->objectAccess(ob, cam);
				}
			}

			for (enemy* e : eList) {
				if (e->getDisplay()) {
					instance->objectAccess(e, cam);
					if (e->getBar() != NULL) {
						AmmoBar* bar = *e->getBar();
						instance->UIDisplay(bar->getSprites());
					}
				}
			}

			for (EnemyBullet* eB : eBList) {
				if (eB->getDisplay()) {
					instance->objectAccess(eB, cam);
				}
			}

			for (objectSprite* b : player->getBullets()) {
				instance->objectDisplay(b, cam);
			}

			instance->objectDisplay(player->getSprites(), cam);
			instance->objectDisplay(player->getSprite(), cam);

			instance->UIDisplay(player->getUI());

			timeLeft -= deltaT;
			if (timeLeft <= 0) {
				run = false;
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
	}
};