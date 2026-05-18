#include "tile.cpp"
#include "time.cpp"
#include "render Logic.cpp"
#include "enemy.cpp"
#include "enemy bullet.cpp"
#pragma once

class Freeze {
	
public:
	static void stop(bool obBefore, shared_ptr<renderer> instance, float targetRate, shared_ptr<player> player, list<shared_ptr<tile>> tileList, list<shared_ptr<tile>> z2List, list<shared_ptr<tile>> z3List, list<shared_ptr<tile>> z4List, list<shared_ptr<GameObject>> obList, list<shared_ptr<enemy>> eList, list<shared_ptr<EnemyBullet>> eBList, list<shared_ptr<GameObject>> backgroundObjects, list<shared_ptr<GameObject>> foregroundObjects, shared_ptr<camera> cam, float timeLeft) {

		shared_ptr<timer> time = shared_ptr<timer>(new timer());


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

			for (shared_ptr<object> ob : backgroundObjects) {
				instance->objectAccess(ob, cam);
			}

			
			for (shared_ptr<GameObject> ob : obList) {
				if (ob->getDisplay() && ob->getSprite() != NULL) {
					instance->objectDisplay(ob->getSprite(), cam);
					instance->bObjectDisplay(ob->getExtraSprites(), cam);
				}
			}

			if (obBefore) {
				for (shared_ptr<enemy> e : eList) {
					if (e->getDisplay()) {
						if (e->getDamSprite() != NULL) {
							instance->objectDisplay(e->getDamSprite(), cam);
						}
						instance->objectAccess(e, cam);
						if (e->getBar() != NULL) {
							shared_ptr<AmmoBar> bar = *e->getBar();
							instance->UIDisplay(bar->getSprites());
						}
					}
				}
			}
			

			for (shared_ptr<tile> t : z4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}



			//tileDistanceCheck(instance, tileList);

			for (shared_ptr<tile> t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			if (!obBefore) {
				for (shared_ptr<enemy> e : eList) {
					if (e->getDisplay()) {
						if (e->getDamSprite() != NULL) {
							instance->objectDisplay(e->getDamSprite(), cam);
						}
						instance->objectAccess(e, cam);
						if (e->getBar() != NULL) {
							shared_ptr<AmmoBar> bar = *e->getBar();
							instance->UIDisplay(bar->getSprites());
						}
					}
				}
			}
			
			for (shared_ptr<GameObject> ob : obList) {
				if (ob->getDisplay() && ob->getSprite() != NULL) {
					instance->objectDisplay(ob->getSprite(), cam);
					instance->bObjectDisplay(ob->getExtraSprites(), cam);
				}
			}
			


			for (shared_ptr<EnemyBullet> eB : eBList) {
				if (eB->getDisplay()) {
					instance->objectAccess(eB, cam);
				}
			}

			if (player->getActiveWeapon()->getName() != "Time Stopper") {
				
				for (shared_ptr<bullet> b : player->getBullets()) {
					instance->objectDisplay(b->getSprite(), cam);
				}
			}
			else {
				instance->UIDisplay(player->getUIBullets());
			}

			instance->objectDisplay(player->getSprites(), cam);
			instance->objectDisplay(player->getSprite(), cam);

			for (shared_ptr<GameObject> g : foregroundObjects) {
				instance->bObjectDisplay(g->getSprite(), cam);
			}

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