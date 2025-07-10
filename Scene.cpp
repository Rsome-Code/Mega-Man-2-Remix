#include "player.cpp"
#include "time.cpp"
#include "Render Logic.cpp"
#include "Camera.cpp"
#include "grid.cpp"
#include "solid Tile.cpp"
#include "Abstract Stage.cpp"
#include "Hitbox Detector.cpp"
#include "Screen Transition.cpp"
#include "transition angle.cpp"
#include "screen lighting.cpp"
#include "pause.cpp"
#include "item.cpp"
#include <list>
#pragma once

class scene {
	player* p;
	timer* time;
	camera* cam;
	bool run = true;
	grid* background;

	bool onLadder = false;
	bool ground = false;

	bool interpolating = false;
	Vector2f newCamPos;

	string stageName;

	abstractStage* stage;
	list<tile*> tileList;
	list<tile*> z2List;
	list<tile*> z3List;
	list<tile*> z4List;

	list<tile*> newTileList;
	list<tile*> newZ2List;
	list<tile*> newZ3List;
	list<tile*> newZ4List;

	list<object*> objects;
	list<transition*> tList;
	list<transition*>::iterator tIterator;
	int section = 0;
	int transitionType;

	bool lastFlagRight = true;
	Vector2f lastFlagPos;

	Texture* enemyT;

	//ScreenLighting* screenLighting;
	Pause* pause;

	bool startPressed = true;
	bool paused = false;

	text* readyText;
	Font font;

public:
	scene(player* pl, abstractStage* stg, Texture* en) {
		enemyT = en;
		p = pl;
		p->getSprite()->setMovable(true);
		time = new timer();
		cam = new camera(p->getSprite(), Vector2f(-960, -540));
		p->setPosition(stg->getInitialPlayer());
		cam->setPosition(stg->getInitialCamera());
		background = new grid();
		stage = stg;
		tileList = stage->getTiles();
		z2List = stage->getZ2List();
		z3List = stage->getZ3List();
		z4List = stage->getZ4List();

		stageName = stage->getName();

		objects = stage->getObjects();
		tList = stage->getTList();
		tIterator = tList.begin();
		p->start(stg->getInitialPlayer());
		lastFlagPos = Vector2f(0, 0);

		//screenLighting = new ScreenLighting();
		
		
		font.loadFromFile("Assets//font.otf");
		readyText = new text(string("READY"), Vector2f(900, 500), float(16), &font, &Color::White);



	}

public:

	bool afterT = false;
	bool justAfterT = false;



	void loop(renderer* instance, double targetRate) {

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;
		
		bool unPaused = false;

		startAnim(instance, targetRate);

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

			if (checkPause(instance, targetRate)) {
				paused = true;
				p->getSprite()->setVVelocity(0);
				if (p->getActiveWeapon()->getHoldTime() == NULL || !p->getController()->checkB()) {

					p->getControls()->resetHold();

				}
				p->start(p->getSprite()->getPosition());
				unPaused = true;
			}
			enemyCheck(objects, instance, targetRate);
			if (paused) {
				
				

				start = time->timerStart();
				startP = &start;
				paused = false;
			}

			if (afterT) {
				p->getSprite()->setMove(true);
				p->getSprite()->setVVelocity(0);
				afterT = false;
			}

			else if (flagCheck(instance, targetRate)) {
				deltaT = 0;
				p->getSprite()->setMove(false);

				
				justAfterT = true;
				
			}
			else if (justAfterT) {
				//p->getSprite()->setMove(true);
				afterT = true;
				justAfterT = false;
			}
			
			p->eachFrame(&deltaT, tileList);



			if (!p->isTeleporting()) {
				ground = false;
				onLadder = false;

				if (!unPaused) {
					if (!p->getControls()->getOnLadder()) {


						tileCheck(tileList);

						p->setGrounded(ground);
						p->getControls()->setInfrontOfLadder(onLadder);

					}

					else {
						onLadder = ladderTileCheck(tileList);
						p->getControls()->setLadder(onLadder);
						p->getControls()->setInfrontOfLadder(onLadder);

						ladderAbove(tileList);

						if (!headLadderTileCheck(tileList)) {
							p->getAnimation()->ladderGetUp();
						}

					}

					p->getControls()->setLadderBelow(ladderBelowTileCheck(tileList));
					p->getControls()->setLadderAbove(ladderAboveTileCheck(tileList));
				}
				else {
					unPaused = false;
					p->getControls()->setLadder(false);
					p->getControls()->setInfrontOfLadder(false);
				}

			}
			

			cam->followX();


			Vector2f flagPos = stage->getFlag();

			enum transitionAngle ang = stage->getAngle();

			cameraFlagCheck(flagPos, ang);


			enemyDistanceCheck(instance, objects);
			

			bulletCollisionCheck(deltaT);



			for (tile* t : z4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}



			tileDistanceCheck(instance, tileList);
			for (tile* t : tileList) {

				if (t->getDisplay() && t->getSprite() !=NULL) {
					instance->objectAccess(t, cam);
				}
			}

			for (object* t : objects) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			instance->objectDisplay(p->getBullets(), cam);
			instance->objectAccess(p->getDamEffect(), cam);
			//p->updateLighting();
			//lightingCheck();
			p->checkHold();
			instance->objectDisplay(p->getSprite(), cam);
			
			p->getSprite()->setRect(IntRect(Vector2i(p->getSprite()->getRect().getPosition().x, p->getBeforeHold()), p->getSprite()->getRect().getSize()));
			//instance->screenLightingDisplay(screenLighting->getRectangles());
			instance->UIDisplay(p->getUI());
			//transition* cur = *next(tIterator);
			//instance->objectHitboxSetup(list<objectHitbox*> { p->getFoot()}, cam);
			//instance->hitboxDisplay(list<UIHitbox*> {p->getFoot()});
			
			instance->getWindow()->display();
			instance->getWindow()->clear();

			if (p->getHP() <= 0) {
				startAnim(instance, targetRate);
				respawn();
				p->heal(2);
			}

		}
	}

	void death() {

	}

	void startAnim(renderer* instance, float targetRate) {

		cam->setPosition(Vector2f(stage->getLastFlagPos().x, cam->getPosition().y));


		for (object* e : objects) {
			e->setOffScreen(true);
			e->setAct(false);
			e->setDisplay(false);
			e->initial();
			e->reset();

			e->setDisplay(true);
			e->setAct(true);
			e->setOffScreen(false);
			e->setInitOffScreen(false);
		}
		

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		float flashTime = 0.4;
		float flashTime_left = flashTime;
		bool display = false;

		float timeLeft = 4;
		bool run = true;

		//cam->followX();

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



			tileDistanceCheck(instance, tileList);
			for (tile* t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			flashTime_left -= deltaT;
			timeLeft -= deltaT;
			if (flashTime_left <= 0) {
				display = !display;
				flashTime_left = flashTime;
			}

			if (display) {
				instance->textDisplay(readyText);
			}

			if (timeLeft <= 0) {
				run = false;
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}

	}

	void respawn() {
		
		p->getSprite()->setPosition(Vector2f(cam->getPosition().x + 900, cam->getPosition().y + 500));
		p->start(Vector2f(cam->getPosition().x + 900, (13*4)*16));
		
	
	}

	void bulletCollisionCheck(float deltaT) {
		for (object* enemy : objects) {
		
			if (enemy->getAct() && enemy->getHurtbox() != NULL) {
				
				//t->checkHit(p->getHitbox());
				enemy->eachFrame(&deltaT, p->getSprite());

				for (bullet* bull : p->getControls()->getBulletObjects()) {
					if (enemy->checkHurt(bull->getHitbox())) {

						enemy->lowerHP(bull->checkDamage(enemy));
						

						if (bull->checkDamage(enemy) <= 0){
							bull->deflect();
						}
						else {
							bull->onHit(enemy);
						}
					}
					else if (enemy->checkHit(bull->getHitbox())) {
						bull->deflect();
					}
					
				}
			}

			else if (enemy->getIncrease() != NULL) {
				enemy->eachFrame(&deltaT, p->getSprite());
			}
		}
	}

	bool  checkPause(renderer* instance, float targetRate) {
		if (p->getController()->checkSTART() && !startPressed) {
			pause = new Pause(stageName, p);
			pause->loop(instance, targetRate, tileList, z2List, z3List, z4List, cam);
			for (object* o : objects) {
				Texture* temp = new Texture();
				temp->loadFromFile("assets\\misc\\" + p->getActiveWeapon()->getName() + ".png");
				if (o->getSprite()->getType() == "ammo" || o->getSprite()->getType() == "health" || o->getSprite()->getType() == "E Tank") {

					o->getSprite()->setTexture(temp);
				}
			}
			startPressed = true;
			return true;
		}
		else if (!p->getController()->checkSTART()) {
			startPressed = false;
		}
		return false;
	}

	void ladderAbove(list<tile*> tileList) {
		for (tile* t : tileList) {
			if (t->getCeiling() != NULL) {
				if (hitboxCheck(p->getHead(), t->getCeiling())) {
					p->setPosition(Vector2f(p->getSprite()->getPosition().x, t->getCeiling()->getPosition().y + 2));
				}
			}
		}
	}



	bool flagCheck(renderer* instance, float targetRate) {
		
		Vector2f flagPos = stage->getFlag();
		
		enum transitionAngle ang = stage->getAngle();


		if (ang == RIGHT) {
			if (p->getSprite()->getPosition().x + 48 >= flagPos.x) {

				lastFlagRight = true;

				loadNextSection();

				sectionTransition(instance, targetRate, ang, flagPos);

				deletePrevSection();
				return true;
			}
		}
		else if (ang == DOWN) {
			if (p->getSprite()->getPosition().y + 48 >= flagPos.y) {
				lastFlagRight = false;
				loadNextSection();

				sectionTransition(instance, targetRate, ang, flagPos);

				deletePrevSection();
				return true;
			}
		}
		return false;
	}

	void cameraFlagCheck(Vector2f flagPos, enum transitionAngle angle) {
		

		//if (angle == RIGHT) {
			if ((cam->getPosition().x + 1920) >= flagPos.x + (16*4)) {
				cam->setPosition(Vector2f(flagPos.x - (1920 - (16*4)), cam->getPosition().y));
			}
			if (lastFlagPos != Vector2f(0, 0)) {
				if (cam->getPosition().x <= lastFlagPos.x) {
					cam->setPosition(Vector2f(lastFlagPos.x, cam->getPosition().y));
				}
			}
		//}
	}

	void sectionTransition(renderer* instance, float targetRate, transitionAngle ang, Vector2f flagPos) {
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

			

			for (tile* t : newZ4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			
			
			for (tile* t : newZ3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			
			for (tile* t : newZ2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : newTileList) {
				instance->objectAccess(t, cam);
			}
			for (tile* t : tileList) {
				instance->objectAccess(t, cam);
			}
			

			if (ang == RIGHT) {
				cam->move(0, &deltaT, float(400));
				if (p->getGrounded()) {
					p->getAnimation()->toeAnim(&deltaT, true);
				}
				else {
					p->getAnimation()->runJump();
				}
				p->getSprite()->move(0, &deltaT, 50);
				if (cam->getPosition().x >= flagPos.x) {
					run = false;
				}
			}
			else if (ang == UP) {
				cam->move(90, &deltaT, float(400));
				p->getAnimation()->ladderAnim(&deltaT);
				p->getSprite()->move(270, &deltaT, 50);
				if (cam->getPosition().y <= flagPos.y) {
					run = false;
				}
			}
			
			else if (ang == DOWN) {
				cam->move(90, &deltaT, float(400));
				if (p->getControls()->getOnLadder()) {
					p->getAnimation()->ladderAnim(&deltaT);
				}
				else {
					p->getAnimation()->runJump();
				}
				
				p->getSprite()->move(90, &deltaT, 70);
				if (cam->getPosition().y >= flagPos.y) {
					run = false;
				}

			}
			p->updateHitbox();
			//p->updateLighting();
			//lightingCheck();
			instance->objectDisplay(p->getSprite(), cam);

			instance->getWindow()->display();
			instance->getWindow()->clear();

		}

		if (lastFlagRight) {
			lastFlagPos = stage->getLastFlagPos();
		}
		else {
			lastFlagPos = cam->getPosition();
		}


	}

	void loadNextSection() {
		section++;

		stage->reload(stageName + to_string(section));
		newTileList = stage->getTiles();
		newZ2List = stage->getZ2List();
		newZ3List = stage->getZ3List();
		newZ4List = stage->getZ4List();
		objects = stage->getObjects();
	}

	void deletePrevSection() {
		tileList = newTileList;
		z2List = newZ2List;
		z3List = newZ3List;
		z4List = newZ4List;

		newTileList.clear();
		newZ2List.clear();
		newZ3List.clear();
		newZ4List.clear();
	}

	void lightingCheck() {
		for (object* ob : objects) {
			if (ob->getLightSource() != NULL) {
				LightSource* light = ob->getLightSource();
				light->updatePos(ob->getSprite()->getCameraPosition());
				p->lightingCheck(light);

				//screenLightingUpdate(light);
			}
		}
	}

	void screenLightingUpdate(LightSource* l) {
		//screenLighting->lightingCheck(l, cam);
	}


	void transitionCheck() {
		if (next(next(tIterator)) != tList.end()) {
			transition* cur = *next(tIterator);
			if (hitboxCheck(p->getHitbox(), cur->getHitbox())) {
				interpolating = true;
				
				transition* ne = *next(next(tIterator));

				transitionType = ne->getType();

				if (transitionType == 0) {
					newCamPos = Vector2f(cur->getHitbox()->getPosition().x, cam->getPosition().y);
				}
				else if (transitionType == 1 || transitionType == 2) {
					newCamPos = Vector2f(cam->getPosition().x, cur->getHitbox()->getPosition().y);
				}
				cam->setXLimit(Vector2f(cur->getSprite()->getPosition().x, ne->getHitbox()->getPosition().x + ne->getHitbox()->getSize().x - 1920));
				tIterator = next(tIterator);
			}
		}


	}

	void enemyCheck(list<object*> eList, renderer* instance, float targetRate) {
		for (object* e : eList) {
			if (e->getAct() && e->getHitbox() != NULL) {
				if (hitboxCheck(e->getHitbox(), p->getHitbox())) {
					if (e->getIncrease() == NULL) {
						if (!p->getDamage()) {
							p->takeDamage(e->getDamage());
						}
					}
					else {
						
						itemGet(instance, targetRate, e);
					}
				}
			}
		}
	}

	void itemGet(renderer* instance, float targetRate, object* item) {
		bool loop = true;
		if (item->getSprite()->getType() == "health") {
			if (p->getHP() == 28) {
				loop = false;
			}
		}

		if (item->getSprite()->getType() == "ammo") {
			if (p->getActiveWeapon()->getAmmo() == 28) {
				loop = false;
			}
		}

		if (item->getSprite()->getType() == "E Tank") {
			if (p->getETanks() <= 3) {
				p->setETanks(p->getETanks() + 1);
			}
		}

		if (loop) {
			itemLoop(instance, targetRate, item);
			paused = true;
		}
		item->used();
		
	}

	void itemLoop(renderer* instance, float targetRate, object* item) {
		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		bool run = true;

		float healRate = 0.15;
		float healRate_left = healRate;

		int healLeft = item->getIncrease();

		if (item->getSprite()->getType() == "health") {
			if (28 - p->getHP() < healLeft) {
				healLeft = 28 - p->getHP();
			}
		}
		if (item->getSprite()->getType() == "ammo") {
			if (28 - p->getActiveWeapon()->getAmmo() < healLeft) {
				healLeft = 28 - p->getActiveWeapon()->getAmmo();
			}
		}

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
			tileDistanceCheck(instance, tileList);
			for (tile* t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}
			for (object* t : objects) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}
			instance->objectDisplay(p->getSprite(), cam);
			
			healRate_left = healRate_left - deltaT;
			if (healRate_left <= 0) {
				healRate_left = healRate;
				if (item->getSprite()->getType() == "health") {
					p->heal(1);
				}
				else if (item->getSprite()->getType() == "ammo") {
					p->getActiveWeapon()->addAmmo(1);
				}
				healLeft--;
				
			}
			if (healLeft <= 0) {
				run = false;
			}
			p->updateAmmo();
			instance->UIDisplay(p->getUI());
			instance->getWindow()->display();
			instance->getWindow()->clear();
			
		}
	}

	void tileDistanceCheck(renderer* instance, list<tile*> tileList) {

		Vector2f camPos = Vector2f(cam->getPosition().x, cam->getPosition().y);
		Vector2u dist = Vector2u((instance->getWindow()->getSize().x + camPos.x), instance->getWindow()->getSize().y + camPos.y);
		//list<tuple <tile*, bool>>::iterator tileI = tileList.begin();

		for (tile* t : tileList) {
			bool display = false;

			Vector2f tilePos = t->getSprite()->getPosition();
			
			if (tilePos.x > camPos.x - (16 * 4) && tilePos.x < dist.x && tilePos.y > camPos.y-(16 * 4) && tilePos.y < dist.y) {
				display = true;
			}

			t->setDisplay(display);
			t->setAct(display);


		}
	
	}



	void enemyDistanceCheck(renderer* instance, list<object*> objects) {
		float camPos = cam->getPosition().x;
		float camEdge = cam->getPosition().x + instance->getWindow()->getSize().x;
		for (object* e : objects) {
			if (e->getIncrease() == NULL) {
				float initial = e->getInitialPosition().x;
				if (!e->getOffScreen()) {
					float ePos = e->getSprite()->getPosition().x;


					if (ePos > camPos && ePos < camEdge) {
						if (e->getOffScreen()) {
							e->setOffScreen(false);
							e->setDisplay(true);
							e->setAct(true);
						}

					}
					else if (e->getOffScreen() == false) {
						e->setOffScreen(true);
						e->setAct(false);
						e->setDisplay(false);
						//e->deleteSprite();

					}

				}
				else {
					if (e->getInitOffScreen()) {
						if (initial > camPos && initial < camEdge) {
							e->initial();
							e->reset();

							e->setDisplay(true);
							e->setAct(true);
							e->setOffScreen(false);
							e->setInitOffScreen(false);
						}
					}

					if (initial > camPos && initial < camEdge) {
						e->setInitOffScreen(false);
					}
					else {
						e->setInitOffScreen(true);
					}

				}
			}
		}
	}

	bool ladderTileCheck(list<tile*> tileList) {
		for (tile* t : tileList) {
			if (t->getAct()) {
				if (t->getLadder() != NULL) {
					if (hitboxCheck(p->getLadderHitbox(), t->getLadder())) {
						p->setPosition(Vector2f(t->getLadder()->getPosition().x - 8, p->getSprite()->getPosition().y));
						return true;
					}
				}
			}
		}
		return false;
	}

	bool headLadderTileCheck(list<tile*> tileList) {
		for (tile* t : tileList) {
			if (t->getAct()) {
				if (t->getLadder() != NULL) {
					if (hitboxCheck(p->getHead(), t->getLadder())) {
						return true;
					}
				}
			}
		}
		return false;
	}

	bool ladderBelowTileCheck(list<tile*> tileList) {
		for (tile* t : tileList) {
			if (t->getAct()) {
				if (t->getLadder() != NULL) {
					if (hitboxCheck(p->getBelow(), t->getLadder())) {
						return true;
					}
				}
			}
		}
		return false;
	}

	bool ladderAboveTileCheck(list<tile*> tileList) {
		for (tile* t : tileList) {
			if (t->getAct()) {
				if (t->getLadder() != NULL) {
					if (hitboxCheck(p->getAbove(), t->getLadder())) {
						return true;
					}
				}
			}
		}
		return false;
	}


	void tileCheck(list<tile*> tileList) {

		for (tile* t : tileList) {

			bool thisGround = false;
			bool thisC = false;

			if (t->getAct()) {

				float currentX = p->getSprite()->getPosition().x;
				if (t->getGround() != NULL) {
					if (hitboxCheck(p->getFoot(), t->getGround())) {
						if (p->getSprite()->getAcceleration().y < 0 || p->getGrounded()) {
							p->getSprite()->setPosition(Vector2f(currentX, t->getGround()->getPosition().y - (p->getHitbox()->getSize().y + 12)));
							//cam->follow();

							ground = true;
							thisGround = true;
						}
					}
				}
				if(!thisGround) {
					if (t->getCeiling() != NULL) {
						if (hitboxCheck(p->getHead(), t->getCeiling())) {
							p->getControls()->jumpCancel();
							thisC = true;
						}
					}
					if (!thisC) {
						if (t->getRight() != NULL) {
							if (hitboxCheck(p->getHitbox(), t->getRight())) {
								p->getSprite()->setPosition(Vector2f(t->getRight()->getPosition().x + t->getRight()->getSize().x - 20, p->getSprite()->getPosition().y));
								p->updateHitbox();
								//p->getSprite()->setPosition(Vector2f(p->getSprite()->getPosition().x - p->getSprite()->getHDistanceTraveled(), p->getSprite()->getPosition().y));
							}
						}
						if (t->getLeft() != NULL) {
							if (hitboxCheck(p->getHitbox(), t->getLeft())) {
								//p->getSprite()->setPosition(Vector2f(p->getSprite()->getPosition().x - p->getSprite()->getHDistanceTraveled(), p->getSprite()->getPosition().y));
								//p->getSprite()->setVVelocity(0);
								p->getSprite()->setPosition(Vector2f(t->getLeft()->getPosition().x - t->getLeft()->getSize().x - p->getHitbox()->getSize().x - 20, p->getSprite()->getPosition().y));
								p->updateHitbox();
							}
						}
					}
				}

				if (t->getLadder() != NULL) {
					if (hitboxCheck(p->getLadderHitbox(), t->getLadder())) {
						onLadder = true;
					}
				}

			}
		}
	}


	bool hitboxCheck(objectHitbox* pHit, objectHitbox* hit) {

		return hitboxDetect::hitboxDetection(pHit, hit);


	}
		
	
	
};