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
	shared_ptr<player> p;
	shared_ptr<UISprite> background;
	shared_ptr<animation> startAnim;
	shared_ptr<animTimer> startTime;
	shared_ptr<animation> endAnim;
	shared_ptr<animTimer> endTime;
	shared_ptr<timer> time;
	shared_ptr<pController> controller;

	shared_ptr<WeaponOption> megaBuster;
	shared_ptr<WeaponOption> atomicFire;
	shared_ptr<WeaponOption> bubbleLead;
	shared_ptr<WeaponOption> metalBlade;
	shared_ptr<WeaponOption> item1;
	shared_ptr<WeaponOption> item2;
	shared_ptr<WeaponOption> item3;
	shared_ptr<WeaponOption> quickBoomerang;
	shared_ptr<WeaponOption> timeStopper;
	shared_ptr<WeaponOption> airShooter;
	shared_ptr<WeaponOption> leafShield;
	shared_ptr<WeaponOption> crashBomb;


	shared_ptr<PageOption> pageOpt;

	shared_ptr<ETankOption> eTanks;

	shared_ptr<Option> active = pageOpt;
	int currentSelect = 0;
	int maxSelect1;
	int maxSelect2;

	int width = 96;
	int height = 160;

	//These should have been arrays
	list<shared_ptr<Option>> page1Options;
	list<shared_ptr<Option>> page2Options;

	float iconX = position.x + 12 * 4;

	bool upPressed = false;
	bool downPressed = false;
	bool startPressed = true;

	bool page1 = true;

	bool healing = false;
	int toHeal = 0;
	float healTime = 0.1;
	float healTime_left = healTime;


	shared_ptr<UISprite> lifeSprite;
	shared_ptr<text> lifeText;
	shared_ptr<Font> font;

	shared_ptr<SoundBuffer> openB;
	shared_ptr<Sound> openSound;

	shared_ptr<SoundBuffer> optionB;
	shared_ptr<Sound> optionSound;

public:

	virtual ~Pause() {


	}

	Pause(string level, shared_ptr<player> p) {
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture);
		this->p = p;
		t->loadFromFile("Assets\\pause\\" + level + ".png");
		shared_ptr<Texture> miscT = shared_ptr<Texture> (new Texture);
		miscT->loadFromFile("Assets\\misc\\" + p->getActiveWeapon()->getName() + ".png");
		background = shared_ptr<UISprite>(new UISprite("background", t, IntRect(1, 11, width, height), position, Vector2f(4,4)));
		startAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(1, 11, width, height), IntRect(98, 11, width, height), IntRect(195, 11, width, height), IntRect(292, 11, width, height), IntRect(389, 11, width, height), IntRect(486, 11, width, height), IntRect(583, 11, width, height), IntRect(680, 11, width, height), IntRect(777, 11, width, height), IntRect(874, 11, width, height), IntRect(971, 11, width, height), IntRect(1068, 11, width, height), IntRect(1165, 11, width, height), IntRect(1262, 11, width, height), IntRect(1359, 11, width, height)}, background));
		endAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(1, 186, width, height), IntRect(98, 186, width, height), IntRect(195, 186, width, height), IntRect(292, 186, width, height), IntRect(389, 186, width, height), IntRect(486, 186, width, height), IntRect(583, 186, width, height), IntRect(680, 186, width, height), IntRect(777, 186, width, height), IntRect(874, 186, width, height), IntRect(971, 186, width, height), IntRect(1068, 186, width, height), IntRect(1165, 186, width, height), IntRect(1262, 186, width, height), IntRect(1359, 186, width, height)}, background));
		startTime = shared_ptr<animTimer> (new animTimer(startAnim, 30, false));
		endTime = shared_ptr<animTimer> (new animTimer(endAnim, 30, false));
		time = shared_ptr<timer>(new timer());

		pageOpt = shared_ptr<PageOption>(new PageOption(t, Vector2f(iconX, position.y + ((9 * 4)) * 2)));
		pageOpt->setNum(maxSelect1);
		maxSelect1++;
		maxSelect2++;

		int pos = 2;

		megaBuster = shared_ptr<WeaponOption>(new WeaponOption(p->getMegaBuster(), Vector2f(iconX, position.y + ((8*pos) * 4)*2)));
		megaBuster->getBar()->update(p->getHP());
		megaBuster->setNum(maxSelect1);
		maxSelect1++;
		page1Options.push_back(megaBuster);

		if (p->hasAtomicFire()) {
			pos = 3;
			addP1Option(&atomicFire, pos, p->getAtomicFire());
			
			
		}
		if (p->checkAir()) {
			addP1Option(&airShooter, 4, p->getAirShooter());
			
		}
		if (p->checkShield()) {
			addP1Option(&leafShield, 5, p->getLeafShield());
		}
		if (p->checkLead()) {
			pos = 6;
			addP1Option(&bubbleLead, pos, p->getBubbleLead());
		}
		if (p->checkBoomerang()) {
			addP1Option(&quickBoomerang, 7, p->getBoomerang());
		}

		if (p->checkStopper()) {
			addP2Option(&timeStopper, 2, p->getTimeStopper());
		}

		
		
		

		if (p->checkBlade()) {
			addP2Option(&metalBlade, 3, p->getMetalBlade());
		}
		if (p->checkBomb()) {
			addP2Option(&crashBomb, 4, p->getCrashBomb());
		}

		if (p->checkAtomicFire()) {
			addP2Option(&item1, 5, p->getItem1());
		}

		if (p->checkAir()) {
			addP2Option(&item2, 6, p->getItem2());
		}

		if (p->checkStopper()) {
			addP2Option(&item3, 7, p->getItem3());
		}



		controller = p->getControls()->getController();

		pos = 8;
		eTanks = shared_ptr<ETankOption>(new ETankOption(miscT, Vector2f(iconX, position.y + ((pos*8) * 4) * 2), p->getETanks()));
		eTanks->setNum(maxSelect1);
		maxSelect1++;
		
		page1Options.push_back(eTanks);

		page1Options.push_back(pageOpt);
		page2Options.push_back(pageOpt);
		
		font = shared_ptr<Font>(new Font());

		font->loadFromFile("Assets//font.otf");
		string numString = to_string(p->getLives());

	
		lifeText = shared_ptr<text>(new text(string(": 0" + numString), Vector2f(iconX + 32*4, position.y + (64 * 4) * 2), float(38), font, Colour::White()));

		lifeSprite = shared_ptr<UISprite>(new UISprite("ui", miscT, IntRect(139, 2, 16, 15), Vector2f(iconX + (16 * 4), position.y + (62 * 4) * 2), Vector2f(4,4)));

		openB = shared_ptr<SoundBuffer> (new SoundBuffer());
		openB->loadFromFile("assets\\sound\\pause.wav");
		openSound = shared_ptr<Sound>(new Sound());
		openSound->setBuffer(*openB);

		optionB = shared_ptr<SoundBuffer> (new SoundBuffer());
		optionB->loadFromFile("Assets\\sound\\cursor_move.wav");

		optionSound = shared_ptr<Sound>(new Sound());
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
			item3->getBar()->update(p->getAirShooter()->getAmmo());
		}
		if (p->checkBomb()) {
			crashBomb->getBar()->update(p->getCrashBomb()->getAmmo());
		}
		if (p->checkAir()) {
			airShooter->getBar()->update(p->getAirShooter()->getAmmo());
			item2->getBar()->update(p->getAirShooter()->getAmmo());
		}
		if (p->checkShield()) {
			leafShield->getBar()->update(p->getLeafShield()->getAmmo());
		}
		if (p->checkBoomerang()) {
			quickBoomerang->getBar()->update(p->getBoomerang()->getAmmo());
		}

		startAnim->reset();
		startTime->reset();
		startAnim->thisFrame();
	}

	void addP1Option(shared_ptr<WeaponOption>* opt, int pos, shared_ptr<Weapon> wep) {
		addOption(opt, pos, wep);
		shared_ptr<WeaponOption> temp = *opt;
		temp->setNum(maxSelect1);
		maxSelect1++;

		page1Options.push_back(*opt);
	}

	void addP2Option(shared_ptr<WeaponOption>* opt, int pos, shared_ptr<Weapon> wep) {
		addOption(opt, pos, wep);
		
		shared_ptr<WeaponOption> temp = *opt;
		temp->setNum(maxSelect2);
		maxSelect2++;

		page2Options.push_back(*opt);
	}
		
	void addOption(shared_ptr<WeaponOption>* opt, int pos, shared_ptr<Weapon> wep) {
		*opt = shared_ptr<WeaponOption> (new WeaponOption(wep, Vector2f(iconX, position.y + ((8 * pos) * 4) * 2)));
	}


	void defaultOption() {
		for (shared_ptr<Option> opt : page1Options) {
			if (opt->getWeapon() != NULL) {
				if (opt->getWeapon()->getName() == p->getActiveWeapon()->getName()) {
					currentSelect = opt->getNum();
					page1 = true;
					break;
				}
			}
		}
		if (currentSelect == 0) {
			for (shared_ptr<Option> opt : page2Options) {
				if (opt->getWeapon()->getName() == p->getActiveWeapon()->getName()) {
					currentSelect = opt->getNum();
					page1 = false;
					break;
				}
			}
		}
	}

	void loop(shared_ptr<renderer> instance, float targetRate, list<shared_ptr<tile>> tileList, list<shared_ptr<tile>> z2List, list<shared_ptr<tile>> z3List, list<shared_ptr<tile>> z4List, list<shared_ptr<GameObject>> backgroundObjects, shared_ptr<camera> cam) {
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

			for (shared_ptr<object> ob : backgroundObjects) {
				instance->objectAccess(ob, cam);
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



			tileDistanceCheck(instance, tileList, cam);
			for (shared_ptr<tile> t : tileList) {

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

		shared_ptr<Weapon> temp = active->getWeapon();
		if (temp != NULL) {
			p->setActiveWeapon(temp);
		}



	}

	bool runHeal(float deltaT, shared_ptr<renderer> instance) {

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

	void displayPage1(shared_ptr<renderer> instance) {
		
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
		if (p->checkAir()) {
			instance->UIDisplay(airShooter->getSprites());
		}
		if (p->checkShield()) {
			instance->UIDisplay(leafShield->getSprites());
		}
		instance->UIDisplay(eTanks->getSprites());
		
	}

	void displayPage2(shared_ptr<renderer> instance) {
		if (p->hasAtomicFire()) {
			instance->UIDisplay(item1->getSprites());
		}
		if (p->checkBlade()) {
			instance->UIDisplay(metalBlade->getSprites());
		}
		if (p->checkStopper()) {
			instance->UIDisplay(timeStopper->getSprites());
			instance->UIDisplay(item3->getSprites());
		}
		if (p->checkBomb()) {
			instance->UIDisplay(crashBomb->getSprites());
		}
		if (p->checkAir()) {
			instance->UIDisplay(item2->getSprites());
		}
		instance->UIDisplay(lifeSprite);
		instance->textDisplay(lifeText);
	}

	bool runMenu(float* deltaT, shared_ptr<renderer> instance) {
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
			for (shared_ptr<Option> o : page1Options) {
				if (o->getNum() == currentSelect) {
					active = o;
				}
				else {
					o->reset();
				}
			}
		}
		else {
			for (shared_ptr<Option> o : page2Options) {
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

	void tileDistanceCheck(shared_ptr<renderer> instance, list<shared_ptr<tile>> tileList, shared_ptr<camera> cam) {

		Vector2f camPos = Vector2f(cam->getPosition().x, cam->getPosition().y);
		Vector2u dist = Vector2u((1920 + camPos.x), 1080 + camPos.y);
		//list<tuple <shared_ptr<tile>, bool>>::iterator tileI = tileList.begin();

		for (shared_ptr<tile> t : tileList) {
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