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
#include "freeze.cpp"
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

	Vector2f lastCheckPoint;

	Texture* enemyT;

	//ScreenLighting* screenLighting;
	Pause* pause;

	bool startPressed = true;
	bool paused = false;

	text* readyText;
	Font font;

	Texture* miscT;

	EndFlag* currentFlag;
	EndFlag* lastFlag;
	enum transitionAngle revLastAngle;

public:
	scene(player* pl, abstractStage* stg, Texture* en) {
		
		enemyT = en;
		p = pl;
		p->getSprite()->setMovable(true);
		time = new timer();
		cam = new camera(p->getSprite(), Vector2f(-960, -540));
		//p->setPosition(stg->getInitialPlayer());
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
		//p->start(stg->getInitialPlayer().x);
		lastFlagPos = Vector2f(0, 0);

		//screenLighting = new ScreenLighting();
		
		
		font.loadFromFile("Assets//font.otf");
		readyText = new text(string("READY"), Vector2f(900, 500), float(22), &font, &Color::White);

		loadFlag();

		miscT = new Texture();
		refreshMisc();
	}

public:

	bool afterT = false;
	bool justAfterT = false;

	void updateFlags() {
		currentFlag = stage->getCurrentFlag(section);
		lastFlag = stage->getCurrentFlag(section - 1);
		if (lastFlag != NULL) {
			if (lastFlag->getAngle() == LEFT) {
				revLastAngle = RIGHT;
			}
			if (lastFlag->getAngle() == RIGHT) {
				revLastAngle = LEFT;
			}
			if (lastFlag->getAngle() == UP) {
				revLastAngle = DOWN;
			}
			if (lastFlag->getAngle() == DOWN) {
				revLastAngle = UP;
			}
		}

	}

	void loop(renderer* instance, double targetRate) {

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;
		
		bool unPaused = false;
		

		//forceLoadSection(1);
		checkLastFlagRight();
		loadFlag();

		startAnim(instance, targetRate);
		respawn();
		//p->heal(-28);

		updateFlags();

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

			pDeathCheck(instance, targetRate);

			if (checkPause(instance, targetRate)) {
				paused = true;
				p->getSprite()->setVVelocity(0);
				if (p->getActiveWeapon()->getHoldTime() == NULL || !p->getController()->checkB()) {

					p->getControls()->resetHold();

				}
				Vector2f prevPosition = p->getSprite()->getPosition();
				p->start(p->getSprite()->getPosition().x);
				p->getTeleport()->forceEnd(prevPosition);
				unPaused = true;
			}
			if (!p->isTeleporting()) {
				enemyCollisionCheck(objects, instance, targetRate);
			}
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

			else if (flagCheck(instance, targetRate, currentFlag->getAngle(), currentFlag->getSprite()->getPosition(), true) || flagCheck(instance, targetRate, revLastAngle, lastFlag->getSprite()->getPosition(), false)) {
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


			Vector2f flagPos = stage->getFlagPos(section);

			enum transitionAngle ang = stage->getAngle();

			cameraFlagCheck(flagPos);


			enemyDistanceCheck(instance, objects);
			

			if (!p->isTeleporting()) {
				bulletCollisionCheck(deltaT);
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
			if (p->getDamEffect() != NULL) {
				instance->objectAccess(p->getDamEffect(), cam);
			}
			
			//p->updateLighting();
			//lightingCheck();
			p->checkHold();
			instance->objectDisplay(p->getSprites(), cam);
			
			p->getSprite()->setRect(IntRect(Vector2i(p->getSprite()->getRect().getPosition().x, p->getBeforeHold()), p->getSprite()->getRect().getSize()));
			//instance->screenLightingDisplay(screenLighting->getRectangles());
			instance->UIDisplay(p->getUI());
			//transition* cur = *next(tIterator);
			//instance->objectHitboxSetup(list<objectHitbox*> { p->getFoot()}, cam);
			//instance->hitboxDisplay(list<UIHitbox*> {p->getFoot()});
			





			instance->getWindow()->display();
			instance->getWindow()->clear();



		}
	}

	void pDeathCheck(renderer* instance, float targetRate) {
		if (p->getHP() <= 0) {

			if (p->getLives() > 0) {


				if (death(instance, targetRate, cam)) {
					startAnim(instance, targetRate);
					respawn();
					p->heal(p->getMaxHP());
					p->setNotDead();

					p->setLives(p->getLives() - 1);

				}
			}
			else {
				run = false;
			}
		}
	}

	bool death(renderer* instance, float tRate, camera* cam) {
		if (p->setDead()) {
			
			Freeze::stop(instance, tRate, p->getSprite(), tileList, z2List, z3List, z4List, objects, cam, 0.75);
			paused = true;
		}
		return p->checkDeathFinish();
			
		
	}

	void resetObjects() {
		for (object* e : objects) {
			if (e->getIncrease() == NULL) {
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
		}
	}

	void startAnim(renderer* instance, float targetRate) {

		if (lastFlagRight) {
			cam->setPosition(Vector2f(stage->getLastFlagPos().x, cam->getPosition().y));

		}
		else {
			cam->setPosition(Vector2f(stage->getLastFlagPos().x - 1920, stage->getLastFlagPos().y));
		}
		Vector2f flagPos = stage->getFlagPos(section);
		cameraFlagCheck(flagPos);

		resetObjects();

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		float flashTime = 0.4;
		float flashTime_left = flashTime;
		bool display = false;

		float timeLeft = 3;
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
		
		p->setPosition(Vector2f(cam->getPosition().x + (1920/2), cam->getPosition().y));
		p->start(cam->getPosition().y + (16*4));
		p->swapDirection();
		
	
	}

	void spawnItemFromObject(object* ob) {
		
		Vector2f middle = Vector2f((ob->getSprite()->getPosition().x + (ob->getSprite()->getSize().x/6)), (ob->getSprite()->getPosition().y + (ob->getSprite()->getSize().y/2)));

		ob->spawnItem(&objects, miscT, middle);

	}

	void refreshMisc() {
		miscT->loadFromFile("assets\\misc\\" + p->getActiveWeapon()->getName() + ".png");
	}

	void bulletCollisionCheck(float deltaT) {
		for (object* enemy : objects) {
		
			if (enemy->getAct() && enemy->getHurtbox() != NULL) {
				
				//t->checkHit(p->getHitbox());
				enemy->eachFrame(&deltaT, p->getSprite());

				if (enemy->getHP() > 0) {

					for (bullet* bull : p->getControls()->getBulletObjects()) {
						if (enemy->checkHurt(bull->getHitbox())) {

							enemy->lowerHP(bull->checkDamage(enemy));


							if (bull->checkDamage(enemy) <= 0) {
								bull->deflect();
							}
							else {
								bull->onHit(enemy);
								if (enemy->getHP() <= 0) {
									spawnItemFromObject(enemy);

								}
							}
						}
						else if (enemy->checkHit(bull->getHitbox())) {
							bull->deflect();
						}

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
				refreshMisc();
				if (o->getSprite()->getType() == "ammo" || o->getSprite()->getType() == "health" || o->getSprite()->getType() == "E Tank" || o->getSprite()->getType() == "Extra Life") {

					o->getSprite()->setTexture(miscT);
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



	bool flagCheck(renderer* instance, float targetRate, enum transitionAngle ang, Vector2f flagPos, bool nextSection) {

		if (ang == RIGHT) {
			if (p->getSprite()->getPosition().x + 48 >= flagPos.x) {

				startTransition(instance, targetRate, ang, flagPos, nextSection);
				return true;
			}
		}
		else if (ang == DOWN) {
			if (p->getSprite()->getPosition().y + 48 >= flagPos.y) {
				startTransition(instance, targetRate, ang, flagPos, nextSection);
				return true;
			}
		}
		else if (ang == UP) {
			if (p->getSprite()->getPosition().y - 48 <= flagPos.y) {
				startTransition(instance, targetRate, ang, flagPos, nextSection);
				return true;
			}
		}
		if (ang == LEFT) {
			if (p->getSprite()->getPosition().x - 48 <= flagPos.x) {

				startTransition(instance, targetRate, ang, flagPos, nextSection);
				return true;
			}
		}

		return false;
	}

	void startTransition(renderer* instance, float targetRate, transitionAngle ang, Vector2f flagPos, bool nextSection) {
		checkLastFlagRight();

		if (nextSection) {
			loadNextSection();
		}
		else {
			section--;
			loadSection();
		}

		sectionTransition(instance, targetRate, ang, flagPos);
	
		deletePrevSection();

		updateFlags();
	}

	void checkLastFlagRight(){
		EndFlag* lastFlag = stage->getLastFlag(section);
		if (lastFlag != NULL) {
			if (lastFlag->getAngle() == RIGHT) {
				lastFlagRight = true;
				lastFlagPos = lastFlag->getSprite()->getPosition();
			}
			else {
				lastFlagRight = false;
				lastFlagPos = lastFlag->getSprite()->getPosition();
			}
			
		}

		
	}

	void isCameraRightOfFlag(Vector2f flagPos) {
		if ((cam->getPosition().x + 1920) >= flagPos.x + (16 * 4)) {
			cam->setPosition(Vector2f(flagPos.x - (1920 - (16 * 4)), cam->getPosition().y));


		}
	}
	void isCameraLeftOfFlag(Vector2f flagPos) {
		if (flagPos != Vector2f(0, 0)) {
			if (cam->getPosition().x <= flagPos.x) {
				cam->setPosition(Vector2f(flagPos.x, cam->getPosition().y));
			}
		}
	}

	void cameraFlagCheck(Vector2f flagPos) {
		

		if (flagPos.x > stage->getLastFlagPos(section).x) {

			isCameraRightOfFlag(flagPos);

			isCameraLeftOfFlag(stage->getLastFlagPos(section));

			cout << flagPos.x;
			cout << ", ";
			cout << stage->getLastFlagPos(section).x;
			cout << "\n";
		}
		else {
			isCameraRightOfFlag(stage->getLastFlagPos(section));

			isCameraLeftOfFlag(flagPos);
		
		}


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
				cam->move(270, &deltaT, float(400));
				p->getAnimation()->ladderAnim(&deltaT);
				p->getSprite()->move(270, &deltaT, 50);
				if (cam->getPosition().y + 1080 <= flagPos.y) {
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

			if (ang == LEFT) {
				cam->move(180, &deltaT, float(400));
				if (p->getGrounded()) {
					p->getAnimation()->toeAnim(&deltaT, true);
				}
				else {
					p->getAnimation()->runJump();
				}
				p->getSprite()->move(180, &deltaT, 50);
				if (cam->getPosition().x + 1920 <= flagPos.x) {
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

		//if (lastFlagRight) {
			lastFlagPos = stage->getLastFlagPos();
		//}
		//else {
			//lastFlagPos = Vector2f(stage->getLastFlagPos().x - 1920, stage->getLastFlagPos().y);
		//}


	}

	void loadFlag() {
		EndFlag* thisOne = NULL;
		for (EndFlag* flag : stage->getFlags()) {
			if (flag->getSection() == section) {
				thisOne = flag;
			}
		}
		if (thisOne != NULL) {
			stage->addEndFlag(thisOne);
		}
	}

	void forceLoadSection(int i) {
		section = i;

		stage->updateSection(i);
		checkLastFlagRight();
		loadFlag();
		loadSection();
		deletePrevSection();
	}
	void loadSection() {
		if (section != 0) {
			stage->reload(stageName + to_string(section));
		}
		else {
			stage->reload(stageName);
		}
		loadFlag();

		newTileList = stage->getTiles();
		newZ2List = stage->getZ2List();
		newZ3List = stage->getZ3List();
		newZ4List = stage->getZ4List();
		objects = stage->getObjects();
	}
	void loadNextSection() {
		section++;

		loadSection();
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




	void enemyCollisionCheck(list<object*> eList, renderer* instance, float targetRate) {
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
			if (p->getHP() == p->getMaxHP()) {
				loop = false;
			}
		}

		if (item->getSprite()->getType() == "ammo") {
			if (p->getActiveWeapon()->getAmmo() == p->getActiveWeapon()->getMAxAmmo() || p->getActiveWeapon()->getName() == "Mega Buster") {
				loop = false;
			}
		}

		if (item->getSprite()->getType() == "E Tank") {
			if (p->getETanks() <= 3) {
				p->setETanks(p->getETanks() + 1);
			}
			loop = false;
		}

		if (item->getSprite()->getType() == "Extra Life") {
			if (p->getLives() <= 8) {
				p->setLives(p->getLives() + 1);
			}
			loop = false;
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
			if (p->getMaxHP() - p->getHP() < healLeft) {
				healLeft = p->getMaxHP() - p->getHP();
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

		Vector2f camPos = Vector2f(cam->getPosition().x - (32 * 4), cam->getPosition().y - (32 * 4));
		Vector2u dist = Vector2u((instance->getWindow()->getSize().x + camPos.x + (64 * 4)), instance->getWindow()->getSize().y + camPos.y + (64*4));
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