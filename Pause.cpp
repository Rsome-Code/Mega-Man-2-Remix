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
#include "Page Option.cpp"
#include "tile.cpp"
#include "camera.cpp"
#include "E Tank Option.cpp"
#pragma once

class Pause {
	Vector2f position = Vector2f(1000, 200);
	player* p;
	UISprite* background;
	animation* startAnim;
	animTimer* startTime;
	animation* endAnim;
	animTimer* endTime;
	timer* time;
	pController* controller;

	WeaponOption* megaBuster;
	WeaponOption* atomicFire;

	PageOption* pageOpt;

	ETankOption* eTanks;

	Option* active = pageOpt;
	int currentSelect = 0;
	int maxSelect1;
	int maxSelect2;

	int width = 96;
	int height = 160;

	list<Option*> page1Options;
	list<Option*> page2Options;

	float iconX = position.x + 12 * 4;

	bool upPressed = false;
	bool downPressed = false;
	bool startPressed = true;

	bool page1 = true;

	bool healing = false;
	int toHeal = 0;
	float healTime = 0.15;
	float healTime_left = healTime;

public:

	Pause(string level, player* p) {
		Texture* t = new Texture;
		this->p = p;
		t->loadFromFile("Assets\\pause\\" + level + ".png");
		Texture* miscT = new Texture;
		miscT->loadFromFile("Assets\\misc\\" + p->getActiveWeapon()->getName() + ".png");
		background = new UISprite("background", t, IntRect(1, 11, width, height), position, Vector2f(4,4));
		startAnim = new animation(list<IntRect>{IntRect(1, 11, width, height), IntRect(98, 11, width, height), IntRect(195, 11, width, height), IntRect(292, 11, width, height), IntRect(389, 11, width, height), IntRect(486, 11, width, height), IntRect(583, 11, width, height), IntRect(680, 11, width, height), IntRect(777, 11, width, height), IntRect(874, 11, width, height), IntRect(971, 11, width, height), IntRect(1068, 11, width, height), IntRect(1165, 11, width, height), IntRect(1262, 11, width, height), IntRect(1359, 11, width, height)}, background);
		endAnim = new animation(list<IntRect>{IntRect(1, 186, width, height), IntRect(98, 186, width, height), IntRect(195, 186, width, height), IntRect(292, 186, width, height), IntRect(389, 186, width, height), IntRect(486, 186, width, height), IntRect(583, 186, width, height), IntRect(680, 186, width, height), IntRect(777, 186, width, height), IntRect(874, 186, width, height), IntRect(971, 186, width, height), IntRect(1068, 186, width, height), IntRect(1165, 186, width, height), IntRect(1262, 186, width, height), IntRect(1359, 186, width, height)}, background);
		startTime = new animTimer(startAnim, 30, false);
		endTime = new animTimer(endAnim, 30, false);
		time = new timer();

		pageOpt = new PageOption(t, Vector2f(iconX, position.y + ((9 * 4)) * 2));
		pageOpt->setNum(maxSelect1);
		maxSelect1++;

		megaBuster = new WeaponOption(p->getMegaBuster(), Vector2f(iconX, position.y + (18 * 4)*2));
		megaBuster->getBar()->update(p->getHP());
		megaBuster->setNum(maxSelect1);
		maxSelect1++;
		page1Options.push_back(megaBuster);

		if (p->hasAtomicFire()) {
			atomicFire = new WeaponOption(p->getAtomicFire(), Vector2f(iconX, position.y + (27 * 4) * 2));
			atomicFire->setNum(maxSelect1);
			maxSelect1++;
			page1Options.push_back(atomicFire);
		}

		controller = p->getControls()->getController();


		eTanks = new ETankOption(miscT, Vector2f(iconX, position.y + (63 * 4) * 2), p->getETanks());
		eTanks->setNum(maxSelect1);
		maxSelect1++;
		
		page1Options.push_back(eTanks);

		page1Options.push_back(pageOpt);
		


	}

	void loop(renderer* instance, float targetRate, list<tile*> tileList, list<tile*> z2List, list<tile*> z3List, list<tile*> z4List, camera* cam) {
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



			for (tile* t : z4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}



			tileDistanceCheck(instance, tileList, cam);
			for (tile* t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

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
				if (!healing) {
					if (runMenu(&deltaT, instance)) {
						if (active != pageOpt && active != eTanks) {
							end = true;
							run = false;
						}
						else if (active == pageOpt) {
							page1 = !page1;
						}
						else if (active == eTanks) {
							if (p->getETanks() > 0) {
								p->setETanks(p->getETanks() - 1);
								eTanks->update(p->getETanks());
								healing = true;
								toHeal = p->getMaxHP() - p->getHP();
							}
						}
					}
				}
				else {
					if (runHeal(deltaT, instance)) {
						healing = false;
					}
				}
				
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}

		Weapon* temp = active->getWeapon();
		if (temp != NULL) {
			p->setActiveWeapon(temp);
		}
	}

	bool runHeal(float deltaT, renderer* instance) {

		healTime_left -= deltaT;

		instance->UIDisplay(pageOpt->getSprites());
		displayPage1(instance);


		active->active(&deltaT);

		if (healTime_left <= 0) {
			p->heal(1);
			megaBuster->getBar()->update(p->getHP());
			healTime_left = healTime;
			toHeal -= 1;
		}

		if (toHeal <= 0) {
			return true;
		}
		return false;
	}

	bool runStart(float* deltaT) {
		startTime->run(deltaT);
		return startTime->isFinished();
	}
	bool runEnd(float* deltaT) {
		endTime->run(deltaT);
		return endTime->isFinished();
	}

	void displayPage1(renderer* instance) {
		
		instance->UIDisplay(megaBuster->getSprites());
		if (p->hasAtomicFire()) {
			instance->UIDisplay(atomicFire->getSprites());
		}
		instance->UIDisplay(eTanks->getSprites());
		
	}

	void displayPage2(renderer* instance) {

	}

	bool runMenu(float* deltaT, renderer* instance) {
		instance->UIDisplay(pageOpt->getSprites());
		if (page1) {
			displayPage1(instance);
		}
		else {
			displayPage2(instance);
		}
		
		if (checkInput()) {
			return true;
		}

		updateSelect();

		active->active(deltaT);


		return false;
	}


	void updateSelect() {
		if (page1) {
			for (Option* o : page1Options) {
				if (o->getNum() == currentSelect) {
					active = o;
				}
				else {
					o->reset();
				}
			}
		}
	}

	bool checkInput() {
		
		if (controller->checkDOWN() && !downPressed) {
			downPressed = true;
			currentSelect++;
			if (currentSelect == maxSelect1) {
				currentSelect = 0;
			}
			cout << currentSelect;
			cout << ", ";
		}
		else if (!controller->checkDOWN()) {
			downPressed = false;
		}
		if (controller->checkUP() && !upPressed) {
			upPressed = true;
			currentSelect--;
			if (currentSelect == -1) {
				currentSelect = maxSelect1-1;
			}
			cout << currentSelect;
			cout << ", ";
		}
		else if (!controller->checkUP()) {
			upPressed = false;
		}
		if (controller->checkSTART() && !startPressed) {
			startPressed = true;
			return true;
		}
		else if (!controller->checkSTART()) {
			startPressed = false;
		}
		return false;


	}

	void tileDistanceCheck(renderer* instance, list<tile*> tileList, camera* cam) {

		Vector2f camPos = Vector2f(cam->getPosition().x, cam->getPosition().y);
		Vector2u dist = Vector2u((instance->getWindow()->getSize().x + camPos.x), instance->getWindow()->getSize().y + camPos.y);
		//list<tuple <tile*, bool>>::iterator tileI = tileList.begin();

		for (tile* t : tileList) {
			bool display = false;

			Vector2f tilePos = t->getSprite()->getPosition();

			if (tilePos.x > camPos.x - (16 * 4) && tilePos.x < dist.x && tilePos.y > camPos.y - (16 * 4) && tilePos.y < dist.y) {
				display = true;
			}

			t->setDisplay(display);
			t->setAct(display);


		}

	}
};