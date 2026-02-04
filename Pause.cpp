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
#include "text.cpp"
#include "E Tank Option.cpp"
#include "GObject.cpp"
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
	WeaponOption* bubbleLead;
	WeaponOption* metalBlade;
	WeaponOption* item1;
	WeaponOption* quickBoomerang;
	WeaponOption* timeStopper;


	PageOption* pageOpt;

	ETankOption* eTanks;

	Option* active = pageOpt;
	int currentSelect = 0;
	int maxSelect1;
	int maxSelect2;

	int width = 96;
	int height = 160;

	//These should have been arrays
	list<Option*> page1Options;
	list<Option*> page2Options;

	float iconX = position.x + 12 * 4;

	bool upPressed = false;
	bool downPressed = false;
	bool startPressed = true;

	bool page1 = true;

	bool healing = false;
	int toHeal = 0;
	float healTime = 0.1;
	float healTime_left = healTime;


	UISprite* lifeSprite;
	text* lifeText;
	Font font;

	SoundBuffer* openB;
	Sound* openSound;

	SoundBuffer* optionB;
	Sound* optionSound;

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
		maxSelect2++;

		int pos = 2;

		megaBuster = new WeaponOption(p->getMegaBuster(), Vector2f(iconX, position.y + ((8*pos) * 4)*2));
		megaBuster->getBar()->update(p->getHP());
		megaBuster->setNum(maxSelect1);
		maxSelect1++;
		page1Options.push_back(megaBuster);

		if (p->hasAtomicFire()) {
			pos = 3;
			addP1Option(&atomicFire, pos, p->getAtomicFire());
			
			
		}
		if (p->checkLead()) {
			pos = 6;
			addP1Option(&bubbleLead, pos, p->getBubbleLead());
		}

		if (p->checkStopper()) {
			addP2Option(&timeStopper, 2, p->getTimeStopper());
		}

		if (p->checkBlade()) {
			addP2Option(&metalBlade, 2, p->getMetalBlade());
		}

		if (p->checkAtomicFire()) {
			addP2Option(&item1, 5, p->getItem1());
		}

		if (p->checkBoomerang()) {
			addP1Option(&quickBoomerang, 7, p->getBoomerang());
		}



		controller = p->getControls()->getController();

		pos = 8;
		eTanks = new ETankOption(miscT, Vector2f(iconX, position.y + ((pos*8) * 4) * 2), p->getETanks());
		eTanks->setNum(maxSelect1);
		maxSelect1++;
		
		page1Options.push_back(eTanks);

		page1Options.push_back(pageOpt);
		page2Options.push_back(pageOpt);
		

		font.loadFromFile("Assets//font.otf");
		string numString = to_string(p->getLives());
		lifeText = new text(string(": 0" + numString), Vector2f(iconX + 32*4, position.y + (64 * 4) * 2), float(38), &font, &Color::White);

		lifeSprite = new UISprite("ui", miscT, IntRect(139, 2, 16, 15), Vector2f(iconX + (16 * 4), position.y + (62 * 4) * 2), Vector2f(4,4));

		openB = new SoundBuffer();
		openB->loadFromFile("assets\\sound\\pause.wav");
		openSound = new Sound();
		openSound->setBuffer(*openB);

		optionB = new SoundBuffer();
		optionB->loadFromFile("Assets\\sound\\cursor_move.wav");

		optionSound = new Sound();
		optionSound->setBuffer(*optionB);

		

	}

	void initial() {
		megaBuster->getBar()->update(p->getHP());
		if (p->hasAtomicFire()) {
			atomicFire->getBar()->update(p->getAtomicFire()->getAmmo());
			item1->getBar()->update(p->getItem1()->getAmmo());
		}
		if (p->checkLead()) {
			bubbleLead->getBar()->update(p->getBubbleLead()->getAmmo());
		}
		if (p->checkBlade()) {
			metalBlade->getBar()->update(p->getMetalBlade()->getAmmo());
		}
		if (p->checkStopper()) {
			timeStopper->getBar()->update(p->getTimeStopper()->getAmmo());
		}
		startAnim->reset();
		startTime->reset();
		startAnim->thisFrame();
	}

	void addP1Option(WeaponOption** opt, int pos, Weapon* wep) {
		addOption(opt, pos, wep);
		WeaponOption* temp = *opt;
		temp->setNum(maxSelect1);
		maxSelect1++;

		page1Options.push_back(*opt);
	}

	void addP2Option(WeaponOption** opt, int pos, Weapon* wep) {
		addOption(opt, pos, wep);
		
		WeaponOption* temp = *opt;
		temp->setNum(maxSelect2);
		maxSelect2++;

		page2Options.push_back(*opt);
	}
		
	void addOption(WeaponOption** opt, int pos, Weapon* wep) {
		*opt = new WeaponOption(wep, Vector2f(iconX, position.y + ((8 * pos) * 4) * 2));
	}


	void defaultOption() {
		for (Option* opt : page1Options) {
			if (opt->getWeapon() != NULL) {
				if (opt->getWeapon()->getName() == p->getActiveWeapon()->getName()) {
					currentSelect = opt->getNum();
					page1 = true;
					break;
				}
			}
		}
		if (currentSelect == 0) {
			for (Option* opt : page2Options) {
				if (opt->getWeapon()->getName() == p->getActiveWeapon()->getName()) {
					currentSelect = opt->getNum();
					page1 = false;
					break;
				}
			}
		}
	}

	void loop(renderer* instance, float targetRate, list<tile*> tileList, list<tile*> z2List, list<tile*> z3List, list<tile*> z4List, list<GameObject*> backgroundObjects, camera* cam) {
		initial();
		bool startA = true;
		bool end = false;
		bool run = true;

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		defaultOption();

		openSound->play();

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
							optionSound->play();
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
		if (p->checkLead()) {
			instance->UIDisplay(bubbleLead->getSprites());
		}
		if (p->checkBoomerang()) {
			instance->UIDisplay(quickBoomerang->getSprites());
		}
		instance->UIDisplay(eTanks->getSprites());
		
	}

	void displayPage2(renderer* instance) {
		if (p->hasAtomicFire()) {
			instance->UIDisplay(item1->getSprites());
		}
		if (p->checkBlade()) {
			instance->UIDisplay(metalBlade->getSprites());
		}
		if (p->checkStopper()) {
			instance->UIDisplay(timeStopper->getSprites());
		}
		instance->UIDisplay(lifeSprite);
		instance->textDisplay(lifeText);
	}

	bool runMenu(float* deltaT, renderer* instance) {
		instance->UIDisplay(pageOpt->getSprites());
		if (page1) {
			displayPage1(instance);
			if (checkInput(maxSelect1)) {
				return true;
			}
		}
		else {
			displayPage2(instance);
			if (checkInput(maxSelect2)) {
				return true;
			}
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
		else {
			for (Option* o : page2Options) {
				if (o->getNum() == currentSelect) {
					active = o;
				}
				else {
					o->reset();
				}
			}
		}
	}

	//Returns true when start is pressed
	bool checkInput(int maxSelect) {
		
		if (controller->checkDOWN() && !downPressed) {
			optionSound->play();
			downPressed = true;
			currentSelect++;
			if (currentSelect == maxSelect) {
				currentSelect = 0;
			}

		}
		else if (!controller->checkDOWN()) {
			downPressed = false;
		}
		if (controller->checkUP() && !upPressed) {
			optionSound->play();
			upPressed = true;
			currentSelect--;
			if (currentSelect == -1) {
				currentSelect = maxSelect-1;
			}

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
		Vector2u dist = Vector2u((1920 + camPos.x), 1080 + camPos.y);
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