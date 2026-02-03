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
#include "door.cpp"
#include "end flag.cpp"
#include <list>
#include "temp enemy.cpp"
#include "spawn area.cpp"
#include "ammo bar.cpp"
#include "teleport Out.cpp"
#include "GObject.cpp"
#include <sfml/audio.hpp>
#include "sound collection.cpp"
#include "item bullets.cpp"

#include "debug menu.cpp"
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

	list<GameObject*> objects;
	list<GameObject*> backgroundObjects;
	list<GameObject*> newBackgroundObjects;
	list <GameObject*> foregroundObjects;
	list <GameObject*> newForegroundObjects;

	list<enemy*> enemies;
	list<EnemyBullet*> eBullets;
	list<Item*> items;
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

	bool selectPressed = true;

	text* readyText;
	Font font;

	Texture* miscT;

	EndFlag* currentFlag;
	EndFlag* lastFlag;
	enum transitionAngle revLastAngle;

	Door* door1;
	Door* door2;

	//list<enemy*> tempEnemies;
	list<Spawner*> spawners;

	SpawnArea* spawner;

	AmmoBar** masterHealth = NULL;

	DeathAnim** bossDeath = NULL;
	DeathAnim** bossDeath1 = NULL;
	DeathAnim** bossDeath2 = NULL;

	bool levelEnd = false;
	float levelEndTime = 10;
	float levelEndTime_left = levelEndTime;

	bool fallDeath = true;

	SoundBuffer* masterDeathB;
	Sound* masterDeathSound;

	bool victoryPlay = false;
	Music* victoryMusic;

	bool nextFlagActive = true;
	bool lastFlagActive = true;

	TeleportOut* teleExit;

	list<ItemBullet*> itemBullets;


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
		//tileList = stage->getTiles();
		//z2List = stage->getZ2List();
		//z3List = stage->getZ3List();
		//z4List = stage->getZ4List();

		stageName = stage->getName();

		//objects = stage->getObjects();
		//backgroundObjects = stage->getBackgroundObjects();
		//enemies = stage->getEnemies();

		//p->start(stg->getInitialPlayer().x);
		lastFlagPos = Vector2f(0, 0);

		//screenLighting = new ScreenLighting();


		font.loadFromFile("Assets//font.otf");
		readyText = new text(string("READY"), Vector2f(900, 500), float(22), &font, &Color::White);

		loadFlag();

		miscT = new Texture();
		refreshMisc();

		door1 = stage->getDoor1();
		door2 = stage->getDoor2();

		for (enemy* e : enemies) {
			e->initial();
		}

		masterDeathB = new SoundBuffer();
		masterDeathSound = new Sound();
		masterDeathB->loadFromFile("assets\\sound\\death.wav");
		masterDeathSound->setBuffer(*masterDeathB);

		victoryMusic = new Music();
		victoryMusic->openFromFile("assets\\sound\\music\\15 - Victory.mp3");

		teleExit = new TeleportOut(p->getSprite());

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

	void levelEndLoop(float deltaT) {
		levelEndTime_left -= deltaT;

		if (levelEndTime_left <= 6) {
			if (!victoryPlay) {
				victoryPlay = true;
				victoryMusic->play();
			}
		}

		if (levelEndTime_left <= 0) {
			levelEndTime_left = levelEndTime;
			run = false;
		}
	}


	bool obBeforeTile = false;
	vector<bool> beforeTileList;

	void checkObBefore() {
		obBeforeTile = beforeTileList[section];
	}

	void debugCheck(DebugMenu* dMenu, player* p, renderer* instance, double* targetRate) {
		if (p->getController()->checkSELECT() && !selectPressed) {
			dMenu->loop(instance, targetRate, tileList, z2List, z3List, z4List, backgroundObjects, cam, p);
			selectPressed = true;
		}
		else if (!p->getController()->checkSELECT()) {
			selectPressed = false;
		}
	}

	float deltaT = 0.00001;
	bool loop(renderer* instance, double targetRate, SoundCollection* soundCol, vector<bool> beforeTileList) {

		DebugMenu* dMenu = new DebugMenu(&font, instance, &targetRate);

		auto start = time->timerStart();
		auto* startP = &start;
		deltaT = 0.00001;

		this->beforeTileList = beforeTileList;


		bool unPaused = false;

		//Change this to the section to be debugged.
		section = 0;

		p->enableControls(true);

		checkLastFlagRight();
		loadFlag();
		updateFlags();

		Music* music = stage->getMusic();
		music->setVolume(30);


		startAnim(instance, targetRate, music, soundCol);
		respawn();
		deltaT = 0.00001;
		//p->heal(-27);

		pause = new Pause(stageName, p);

		

		while (instance->getWindow()->isOpen() && run) {

			debugCheck(dMenu, p, instance, &targetRate);

			//p->enableControls(false);
			//p->getSprite()->enableGravity(false);

			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();
			}
			if (time->frameLimiter(targetRate, startP)) {
				deltaT = 0.0033;
				//for (tile* t : tileList) {
				//	t->reset();
			//	}
				//deltaT = time->checkTimer(startP);
			}
			else {
				deltaT = time->checkTimer(startP);
			}



			start = time->timerStart();
			startP = &start;


			pDeathCheck(instance, targetRate, music, soundCol);


			if (levelEnd) {
				levelEndLoop(deltaT);
			}

			if (checkPause(instance, targetRate)) {

				paused = true;

				p->resetBullets();

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
				enemyCollisionCheck(enemies, instance, targetRate);
				itemCollisionCheck(instance, targetRate);
				itemLoop(deltaT);
			}
			if (paused) {
				start = time->timerStart();
				startP = &start;
				paused = false;
			}

			if (!p->isTeleporting() && p->getHP() > 0) {
				if (afterT) {
					p->getSprite()->setMove(true);
					//I don't remember why the player's vertical velocity is reset after a screen transition
					//p->getSprite()->setVVelocity(0);
					afterT = false;
				}

				else if (flagCheck(instance, targetRate, currentFlag->getAngle(), currentFlag->getSprite()->getPosition(), true, nextFlagActive, soundCol) || flagCheck(instance, targetRate, revLastAngle, lastFlag->getSprite()->getPosition(), false, lastFlagActive, soundCol)) {
					deltaT = 0;
					p->getSprite()->setMove(false);


					justAfterT = true;

				}
				else if (justAfterT) {
					//p->getSprite()->setMove(true);
					afterT = true;
					justAfterT = false;
				}
			}
			p->eachFrame(&deltaT, tileList, &itemBullets);

			checkFall();

			for (GameObject* o : objects) {
				o->setCamera(cam);
				o->eachFrame(&deltaT, p->getSprite(), cam);
				o->eachFrame(&deltaT, p, cam, &tileList);
				o->eachFrame(&deltaT, p, cam);
				o->eachFrame(&deltaT, p->getSprite());

				list<GameObject*> temp;
				for (GameObject* g : enemies) {
					temp.push_back(g);
				}
				
				o->eachFrame(&deltaT, p->getSprite(), temp);
				//instance->bObjectDisplay(g->getSprite(), cam);
			}

			if (!p->isTeleporting()) {
				ground = false;


				if (!unPaused) {
					tileCheck(instance, &deltaT);
					if (!p->getControls()->getOnLadder()) {


						

						//p->setGrounded(ground);


					}

					else {
						onLadderTileCheck(tileList, instance);
						//p->getControls()->setLadder(onLadder);
						//p->getControls()->setInfrontOfLadder(onLadder);


					}


				}
				else {
					unPaused = false;
					p->getControls()->setLadder(false);
					p->getControls()->setInfrontOfLadder(false);
				}
				//onLadder = false;

			}


			cam->followX();


			Vector2f flagPos = stage->getFlagPos(section);

			enum transitionAngle ang = stage->getAngle();

			//if (!p->isTeleporting()) {

			cameraFlagCheck(flagPos);

			if (fabs(currentFlag->getPosition().x - lastFlag->getPosition().x) == 1920 || ((((lastFlag->getAngle() == DOWN || lastFlag->getAngle() == UP) && currentFlag->getAngle() == RIGHT) && (currentFlag->getPosition().x - lastFlag->getPosition().x) <= 1920))) {

				flagCorrectedCam();
			}

			else {

				if (nextFlagActive && lastFlagActive) {
					if (fabs(currentFlag->getPosition().x - lastFlag->getPosition().x) != 1920) {
						if (currentFlag->getAngle() != UP && currentFlag->getAngle() != DOWN) {
							centerCamera();
						}
						else {
							nextFlagCam();
						}
					}
				}
			}
			//}

			enemyDistanceCheck(instance, enemies);

			if (spawner != NULL) {
				spawnLoop(deltaT, soundCol);
			}


			if (!p->isTeleporting()) {
				enemyCheck(deltaT, instance, targetRate, music, soundCol);
				if (paused) {
					start = time->timerStart();
					startP = &start;
					paused = false;
				}
			}

			enemyBullets(deltaT);

			for (Spawner* spawn : spawners) {

				spawn->getSprite()->setCameraPosition(Vector2f(spawn->getSprite()->getPosition().x - cam->getPosition().x, spawn->getSprite()->getPosition().y));
				if (spawn->getSprite()->getCameraPosition().x > 0 && spawn->getSprite()->getCameraPosition().x < 1920) {
					spawn->eachFrame(&deltaT, p->getSprite(), &enemies, cam, soundCol);
				}
			}

			for (object* o : backgroundObjects) {
				instance->objectAccess(o, cam);
			}


			//allTileOn(tileList);
			//allTileOn(z4List);
			//allTileOn(z3List);
			//allTileOn(z2List);

			backgroundTileDistanceCheck(z2List);
			backgroundTileDistanceCheck(z3List);
			backgroundTileDistanceCheck(z4List);


			itemBulletLoop(deltaT);
			p->setGrounded(ground);



			for (tile* t : z4List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), t->getDisplay(), cam);
			}
			for (tile* t : z3List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), t->getDisplay(), cam);

			}
			for (tile* t : z2List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), t->getDisplay(), cam);

			}

			if (obBeforeTile) {
				for (enemy* t : enemies) {
					if (t->getDamSprite() != NULL) {
						instance->objectDisplay(t->getDamSprite(), cam);
					}
					if (t->getSprite() != NULL) {

						instance->objectAccess(t, cam);
						//instance->objectHitboxSetup(t->getHitbox(), cam);
						//instance->hitboxDisplay(t->getHitbox());


					}
				}
			}

			for (tile* t : tileList) {
				t->animate(&deltaT);
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
					for (objectSprite* sp : t->getInternalSprites()) {
						instance->objectDisplay(sp, cam);
					}
					
				}
			}

			if (door1 != NULL) {
				instance->objectAccess(door1, cam);
			}
			if (door2 != NULL) {
				instance->objectAccess(door2, cam);
			}

			for (object* t : objects) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			if (!obBeforeTile) {
				for (enemy* t : enemies) {
					if (t->getDamSprite() != NULL) {
						instance->objectDisplay(t->getDamSprite(), cam);
					}
					if (t->getSprite() != NULL) {

						instance->objectAccess(t, cam);
						//instance->objectHitboxSetup(t->getHitbox(), cam);
						//instance->hitboxDisplay(t->getHitbox());


					}
				}
			}

			for (ItemBullet* i : itemBullets) {
				if (i->getSprite() != NULL) {
					instance->objectDisplay(i->getSprite(), cam);
				}
			}

			if (bossDeath != NULL) {
				DeathAnim* bD = *bossDeath;
				instance->objectDisplay(bD->getSprite(), cam);
				bD = *bossDeath1;
				instance->objectDisplay(bD->getSprite(), cam);
				bD = *bossDeath2;
				instance->objectDisplay(bD->getSprite(), cam);
			}



			for (object* i : items) {
				instance->objectAccess(i, cam);
			}

			for (object* o : eBullets) {
				instance->objectAccess(o, cam);
			}

			for (GameObject* g : foregroundObjects) {
				list<GameObject*> temp;
				for (GameObject* g : enemies) {
					temp.push_back(g);
				}
				g->setCamera(cam);
				g->eachFrame(&deltaT, p->getSprite(), temp);
				instance->bObjectDisplay(g->getSprite(), cam);
			}

			if (p->getActiveWeapon()->getName() != "Time Stopper") {
				instance->objectDisplay(p->getBullets(), cam);
			}
			else {
				instance->UIDisplay(p->getUIBullets());
			}

			if (p->getDamEffect() != NULL) {
				instance->objectAccess(p->getDamEffect(), cam);
			}

			

			for (GameObject* o : backgroundObjects) {
				o->setCamera(cam);
				o->eachFrame(&deltaT, p->getSprite());

			}



			//p->updateLighting();
			//lightingCheck();
			p->checkHold();

			if (p->getDisplay()) {
				instance->objectDisplay(p->getSprites(), cam);
			}

			p->getSprite()->setRect(IntRect(Vector2i(p->getSprite()->getRect().getPosition().x, p->getBeforeHold()), p->getSprite()->getRect().getSize()));
			//instance->screenLightingDisplay(screenLighting->getRectangles());
			instance->UIDisplay(p->getUI());
			if (masterHealth != NULL) {
				AmmoBar* a = *masterHealth;
				instance->UIDisplay(a->getSprites());
			}


			/*for (enemy* e : enemies) {
				instance->objectHitboxSetup(list<objectHitbox*> {e->getHitbox()}, cam);
				instance->hitboxDisplay(list<UIHitbox*> { e->getHitbox()});
			}
			*/



			instance->getWindow()->display();
			instance->getWindow()->clear();



		}



		if (levelEnd) {
			list<GameObject*> temps = objects;
			temps.push_back(door1);
			temps.push_back(door2);
			teleExit->loop(instance, targetRate, p, tileList, z2List, z3List, z4List, temps, backgroundObjects, cam);
		}

		return levelEnd;
	}

	void itemBulletLoop(float deltaT) {
		ItemBullet* toDelete = NULL;

		

		for (ItemBullet* iBul : itemBullets) {
			if (iBul->eachFrame(&deltaT)) {
				toDelete = iBul;
			}

			if (p->getSprite()->getVVelocity() <= 0) {
				if (hitboxCheck(iBul->getHit(), p->getFoot())) {
					p->getSprite()->setPosition(Vector2f(p->getSprite()->getPosition().x, iBul->getHit()->getPosition().y - (p->getHitbox()->getSize().y + 12)));
					//cam->follow();
					ground = true;
				}
			}
		}



		if (toDelete != NULL) {
			itemBullets.remove(toDelete);
		}
	}

	void allTileOn(list<tile*> tL) {
		for (tile* t : tL) {
			t->setAct(true);
			t->setDisplay(true);
		}
	}

	void nextFlagCam() {
		cam->setPosition(Vector2f(currentFlag->getPosition().x - 1920, cam->getPosition().y));
	}

	void flagCorrectedCam() {
		if (currentFlag->getPosition().x > lastFlag->getPosition().x) {
			cam->setPosition(Vector2f(lastFlag->getPosition().x, cam->getPosition().y));
		}
		else {
			cam->setPosition(Vector2f(lastFlag->getPosition().x - 1920, cam->getPosition().y));
		}
	}

	void centerCamera() {
		float xPos = ((fabs(currentFlag->getPosition().x - lastFlag->getPosition().x)) / 2) - (1920 / 2);
		if (currentFlag->getPosition().x < lastFlag->getPosition().x) {
			xPos = xPos + currentFlag->getPosition().x;
		}
		else {
			xPos = xPos + lastFlag->getPosition().x;
		}
		int temp = xPos;
		cam->setPosition(Vector2f(temp, cam->getPosition().y));
	}

	void checkFall() {
		if (p->getSprite()->getCameraPosition().y > 1080 && fallDeath) {

			p->takeDamage(28);

		}
	}

	void spawnLoop(float deltaT, SoundCollection* soundCol) {
		spawner->eachFrame(p, deltaT, &enemies, cam, soundCol);
	}

	void enemyBullets(float deltaT) {

		list<EnemyBullet*>::iterator it = eBullets.begin();
		list<EnemyBullet*>::iterator toDelete = eBullets.end();
		for (EnemyBullet* b : eBullets) {
			b->eachFrame(&deltaT, &tileList);
			b->eachFrame(&deltaT, &tileList, p);
			if (hitboxCheck(p->getHitbox(), b->getHitbox()) && !p->isInvincible() && p->checkInControl()) {
				p->takeDamage(b->getDamage());
			}

			if (bulletsCollide(b)) {
				toDelete = it;
			}

			if (checkEBullOffScreen(b)) {
				toDelete = it;
			}

			it = next(it);
		}
		//removeBullets();
		if (toDelete != eBullets.end()) {
			*toDelete = NULL;
			eBullets.erase(toDelete);

		}
	}

	bool bulletsCollide(EnemyBullet* b) {
		list<bullet*> bullets = p->getWeapon()->getBullets();
		if (b->getCollType() == EnemyBullet::CollisionType::DESTROY) {
			for (bullet* playerB : bullets) {
				objectHitbox* playerH = playerB->getHitbox();
				if (hitboxCheck(playerH, b->getHitbox())) {
					playerB->deflect();
				}
			}
		}
		else if (b->getCollType() == EnemyBullet::CollisionType::DESTROY) {
			for (bullet* playerB : bullets) {
				objectHitbox* playerH = playerB->getHitbox();
				if (hitboxCheck(playerH, b->getHitbox())) {
					return true;
				}
			}
		}
		return false;
	}



	bool checkEBullOffScreen(EnemyBullet* b) {
		if (checkObOffScreen(b, cam->getPosition(), Vector2f(cam->getPosition().x + 1920, cam->getPosition().y + 1080))) {
			//eBullets.remove(b);
			//delete b;
			return true;
		}
		return false;
	}

	bool checkObOffScreen(EnemyBullet* e, Vector2f camPos, Vector2f camEdge) {
		if (e->getPosition().x > camEdge.x || e->getPosition().x + e->getSprite()->getSize().x < camPos.x) {
			return true;
		}
		if (e->getPosition().y > camEdge.y || e->getPosition().y + e->getSprite()->getSize().y < camPos.y) {
			return true;
		}
		return false;
	}

	void pDeathCheck(renderer* instance, float targetRate, Music* music, SoundCollection* soundCol) {
		if (p->getHP() <= 0) {

			music->stop();
			p->getWeapon()->stopSound();
			for (enemy* e : enemies) {
				e->stopMusic();
			}



			if (death(instance, targetRate, cam)) {
				if (p->getLives() > 0) {
					startAnim(instance, targetRate, music, soundCol);
					respawn();
					//p->heal(p->getMaxHP());
					p->HPReset();
					p->setNotDead();

					p->setLives(p->getLives() - 1);
				}
				else {
					run = false;

				}

			}


		}
	}

	void itemLoop(float deltaT) {
		for (Item* o : items) {
			o->eachFrame(&deltaT, p->getSprite(), &tileList);
		}
	}

	bool death(renderer* instance, float tRate, camera* cam) {
		if (p->setDead()) {

			list<GameObject*> tempL = objects;
			tempL.push_back(door1);
			tempL.push_back(door2);
			Freeze::stop(instance, tRate, p, tileList, z2List, z3List, z4List, tempL, enemies, eBullets, backgroundObjects, foregroundObjects, cam, 0.75);

			masterDeathSound->play();

			paused = true;
		}
		return p->checkDeathFinish();


	}

	void resetObjects() {
		for (enemy* e : enemies) {
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

				e->setCamera(cam);
			}
		}
	}

	EndFlag* getLastCheckpoint() {
		return stage->getLastCheckpoint(section);
	}

	void startAnim(renderer* instance, float targetRate, Music* music, SoundCollection* soundCol) {

		
		eBullets.clear();
		p->setDeathNull();
		music->play();

		EndFlag* flag = getLastCheckpoint();

		section = flag->getSection() + 1;

		//for (int i = 0; i < 1000; i++) {
			forceLoadSection(section, soundCol);
		//}

		p->shootReset();

		if (flag->getAngle() == RIGHT) {
			cam->setPosition(flag->getPosition());

		}
		else {
			if (lastFlag->getAngle() == UP) {
				cam->setPosition(Vector2f(flag->getPosition().x - 1920, flag->getPosition().y - 1080));
			}
			else {
				cam->setPosition(Vector2f(flag->getPosition().x - 1920, flag->getPosition().y));
			}
		}
		Vector2f flagPos = flag->getPosition();

		cameraFlagCheck(flagPos);

		if (fabs(currentFlag->getPosition().x - lastFlag->getPosition().x) == 1920 || ((((lastFlag->getAngle() == DOWN || lastFlag->getAngle() == UP) && currentFlag->getAngle() == RIGHT) && (currentFlag->getPosition().x - lastFlag->getPosition().x) <= 1920))) {

			flagCorrectedCam();
		}

		else {

			if (nextFlagActive && lastFlagActive) {
				if (fabs(currentFlag->getPosition().x - lastFlag->getPosition().x) != 1920) {
					if (currentFlag->getAngle() != UP && currentFlag->getAngle() != DOWN) {
						centerCamera();
					}
					else {
						nextFlagCam();
					}
				}
			}
		}



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

			if (time->frameLimiter(targetRate, startP)) {
				deltaT = 0.0333333333;
			}
			else {
				deltaT = time->checkTimer(startP);
			}

			start = time->timerStart();
			startP = &start;

			for (object* o : backgroundObjects) {
				o->setCamera(cam);
				o->eachFrame(&deltaT, p->getSprite());
				instance->objectAccess(o, cam);
			}

			for (tile* t : z4List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z3List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z2List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), cam);
			}

			
			for (tile* t : tileList) {
				t->animate(&deltaT);
				tileDistanceCheck(instance, t);
			}
			for (object* o : objects) {
				float* f = new float(0);
				o->eachFrame(f, p->getSprite());
			}
			for (tile* t : tileList) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
					for (objectSprite* sp : t->getInternalSprites()) {
						instance->objectDisplay(sp, cam);
					}
				}
			}

			for (GameObject* g : foregroundObjects) {
				instance->bObjectDisplay(g->getSprite(), cam);
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

		bossDeath = NULL;

		p->setPosition(Vector2f(cam->getPosition().x + ((1920 / 2) - 8 * 4), cam->getPosition().y));
		p->start(cam->getPosition().y + (16 * 4));
		p->swapDirection();

	}

	void spawnItemFromEnemy(enemy* en, SoundCollection* soundCol) {

		Vector2f middle = Vector2f((en->getSprite()->getPosition().x + (en->getSprite()->getSize().x / 6)), (en->getSprite()->getPosition().y + (en->getSprite()->getSize().y / 2)));



		en->spawnItem(&items, miscT, middle, soundCol);

	}

	void refreshMisc() {
		miscT->loadFromFile("assets\\misc\\" + p->getActiveWeapon()->getName() + ".png");
		p->iniSplash(miscT);
	}

	void levelEndCheck(enemy* e, Music* music) {
		if (e->getCode() == stage->getName()) {

			p->enableControls(false);
			levelEnd = true;
			if (p->getHP() <= 0) {
				p->setHP(1);
			}
			masterDeathSound->play();


		}
	}

	void levelEndSequence(Music* music, renderer* instance, float tRate, enemy* enemy) {
		music->stop();
		list<GameObject*> tempL = objects;
		tempL.push_back(door1);
		tempL.push_back(door2);
		enemy->forceDamSprite();
		Freeze::stop(instance, tRate, p, tileList, z2List, z3List, z4List, tempL, enemies, eBullets, backgroundObjects, foregroundObjects, cam, 0.75);
		enemy->forceDamSpriteOff();
		paused = true;
		levelEndCheck(enemy, music);
		p->resetBullets();
	}

	void enemyCheck(float deltaT, renderer* instance, float tRate, Music* music, SoundCollection* soundCol) {
		enemy* toDelete = NULL;

		for (enemy* enemy : enemies) {

				

			

			if (enemy->getHP() > 0) {


				for (bullet* bull : p->getControls()->getBulletObjects()) {
					if (enemy->checkHurt(bull->getHitbox())) {

						if (!enemy->checkInvincible()) {
							enemy->lowerHP(bull->checkDamage(enemy));


							if (bull->checkDamage(enemy) <= 0) {
								bull->deflect();
							}
							else {

								if (enemy->getHP() <= 0) {

									if (enemy->getDeathAnims()[0] != NULL) {

										levelEndSequence(music, instance, tRate, enemy);
									}
									bull->onHit(enemy);
									spawnItemFromEnemy(enemy, soundCol);

									enemy->spawnEnemy(&enemies, soundCol);
								}

								else {
									bull->onHit(enemy);
								}
							}
						}
						else {
							bull->deflect();
						}
					}
					else if (enemy->checkHit(bull->getHitbox())) {
						bull->deflect();
					}

				}
				
				
				//Should only return true when enemy is Quick Man
				if (enemy->freezeDam(p)) {
					levelEndSequence(music, instance, tRate, enemy);
				}
				
			}

			if (enemy->eachFrame(&deltaT, p, &tileList, &enemies, &eBullets, soundCol) || enemyYCheck(enemy)) {
				toDelete = enemy;
			}

			if (!levelEnd) {
				p->enableControls(enemy->getIntroDone());
				if (!enemy->getIntroDone()) {
					music->stop();
				}
			}


		}
		if (toDelete != NULL) {
			enemies.remove(toDelete);
			
			delete toDelete;
			
		}

	}

	
	
	bool enemyYCheck(enemy* e) {
		if (e->getSprite()->getCameraPosition().y > 1080) {
			return e->yDeath(&enemies);
			return e->isDead(&enemies);
		}
		return false;
	}


	bool  checkPause(renderer* instance, float targetRate) {
		if (p->getController()->checkSTART() && !startPressed && p->checkInControl()) {
			p->getAtomicFire()->resetHold();

			pause->loop(instance, targetRate, tileList, z2List, z3List, z4List, backgroundObjects, cam);
			refreshMisc();
			for (object* o : items) {

				//if (o->getSprite()->getType() == "ammo" || o->getSprite()->getType() == "health" || o->getSprite()->getType() == "E Tank" || o->getSprite()->getType() == "Extra Life") {

				o->getSprite()->setTexture(miscT);
				//}
			}
			startPressed = true;
			return true;
		}
		else if (!p->getController()->checkSTART()) {
			startPressed = false;
		}
		return false;
	}

	void ladderAbove(tile* t) {

		if (t->getCeiling() != NULL) {
			if (hitboxCheck(p->getHead(), t->getCeiling())) {
				p->setPosition(Vector2f(p->getSprite()->getPosition().x, t->getCeiling()->getPosition().y + 2));
			}
		}

	}



	bool flagCheck(renderer* instance, float targetRate, enum transitionAngle ang, Vector2f flagPos, bool nextSection, bool active, SoundCollection* soundCol) {


		if (active) {

			if (ang == RIGHT) {
				if (p->getSprite()->getPosition().x + 48 >= flagPos.x) {

					bool door = doorCheck(instance, targetRate);

					startTransition(instance, targetRate, ang, flagPos, nextSection, soundCol);

					if (door) {
						doorClose(instance, targetRate);
					}
					return true;
				}
			}
			else if (ang == DOWN && !fallDeath) {
				if (p->getSprite()->getPosition().y + 48 >= flagPos.y) {
					startTransition(instance, targetRate, ang, flagPos, nextSection, soundCol);
					return true;
				}
			}
			else if (ang == UP && p->getControls()->getOnLadder()) {
				if (p->getSprite()->getPosition().y - 48 <= flagPos.y) {
					startTransition(instance, targetRate, ang, flagPos, nextSection, soundCol);
					return true;
				}
			}
			if (ang == LEFT) {
				if (p->getSprite()->getPosition().x <= flagPos.x) {

					startTransition(instance, targetRate, ang, flagPos, nextSection, soundCol);
					return true;
				}
			}
		}

		return false;
	}

	bool doorCheck(renderer* instance, float targetRate) {
		if (door1->getSection() == section) {
			door1->loop(instance, cam, targetRate, p, door2->getSprite(), tileList, z2List, z3List, z4List, backgroundObjects, true);
			p->resetBullets();
			return true;
		}
		if (door2->getSection() == section) {
			door2->loop(instance, cam, targetRate, p, door1->getSprite(), tileList, z2List, z3List, z4List, backgroundObjects, true);
			p->resetBullets();
			return true;
		}
		return false;
	}

	void doorClose(renderer* instance, float targetRate) {
		if (door1->getSection() == section - 1) {
			door1->loop(instance, cam, targetRate, p, door2->getSprite(), tileList, z2List, z3List, z4List, backgroundObjects, false);

		}
		if (door2->getSection() == section - 1) {
			door2->loop(instance, cam, targetRate, p, door1->getSprite(), tileList, z2List, z3List, z4List, backgroundObjects, false);

		}
	}


	void startTransition(renderer* instance, float targetRate, transitionAngle ang, Vector2f flagPos, bool nextSection, SoundCollection* soundCol) {
		checkLastFlagRight();

		if (nextSection) {
			loadNextSection(soundCol);
		}
		else {
			section--;
			loadSection(soundCol);
		}

		updateFlags();
		checkObBefore();

		
		sectionTransition(instance, targetRate, ang, flagPos, nextSection);

		deletePrevSection();

		lastFlagPos = lastFlag->getSprite()->getPosition();

	}

	void checkLastFlagRight() {
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

	bool isCameraRightOfFlag(Vector2f flagPos) {

		float nextPos = flagPos.x;

		if (currentFlag->getAngle() == RIGHT) {
			nextPos = nextPos + (16 * 4);
		}
		if ((cam->getPosition().x + 1920) >= nextPos) {
			cam->setPosition(Vector2f(nextPos - (1920), cam->getPosition().y));

			if (currentFlag->getAngle() == DOWN || lastFlag->getAngle() == UP) {
				fallDeath = false;
			}
			else {
				cam->setPosition(Vector2f(nextPos - (1920), cam->getPosition().y));
			}
			return true;

		}
		return false;
	}
	bool isCameraLeftOfFlag(Vector2f flagPos) {
		if (flagPos != Vector2f(0, 0)) {
			if (cam->getPosition().x <= flagPos.x) {
				cam->setPosition(Vector2f(flagPos.x, cam->getPosition().y));
				if (currentFlag->getAngle() == DOWN || lastFlag->getAngle() == UP) {
					fallDeath = false;
				}
				else {

				}
				return true;
			}
		}
		return false;
	}

	void cameraFlagCheck(Vector2f flagPos) {



		fallDeath = true;
		if (currentFlag->getSprite()->getPosition().x > lastFlagPos.x) {

			//If the distance between both flags are less than the camera size...
			if ((fabs(currentFlag->getSprite()->getPosition().x - lastFlagPos.x) <= 1920) && !((lastFlag->getAngle() == UP || lastFlag->getAngle() == DOWN) && currentFlag->getAngle() == RIGHT)) {
				cam->setPosition(Vector2f(lastFlagPos.x, cam->getPosition().y));
				fallDeath = false;
				nextFlagActive = true;
				lastFlagActive = true;
			}

			else if ((lastFlag->getAngle() == UP || lastFlag->getAngle() == DOWN) && lastFlag->getPosition().x > getFlag(section - 2)->getPosition().x) {

					nextFlagActive = (isCameraRightOfFlag(currentFlag->getSprite()->getPosition()));

					lastFlagActive = (isCameraLeftOfFlag(Vector2f(lastFlagPos.x - (1920), cam->getPosition().y)));
			}
			


			else {
				nextFlagActive = (isCameraRightOfFlag(currentFlag->getSprite()->getPosition()));

				lastFlagActive = (isCameraLeftOfFlag(lastFlagPos));
			}

		}
		else {
			if (fabs(currentFlag->getSprite()->getPosition().x - lastFlagPos.x) <= 1920) {
				cam->setPosition(Vector2f(lastFlagPos.x - (1920), cam->getPosition().y));
				fallDeath = false;
				nextFlagActive = true;
				lastFlagActive = true;
			}
			else {
				lastFlagActive = isCameraRightOfFlag(lastFlagPos);

				nextFlagActive = isCameraLeftOfFlag(currentFlag->getSprite()->getPosition());
			}
		}



	}




	void sectionTransition(renderer* instance, float targetRate, transitionAngle ang, Vector2f flagPos, bool forward) {
		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		bool run = true;

		float camSpeed = 1000;
		float playerSpeed = 150;
		Vector2f otherFlagPos;
		transitionAngle otherAngle;

		if (forward) {
			otherFlagPos = currentFlag->getPosition();
			otherAngle = currentFlag->getAngle();
		}
		else {
			otherFlagPos = lastFlag->getPosition();
			otherAngle = lastFlag->getAngle();
		}

		while (instance->getWindow()->isOpen() && run) {
			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();
			}
			time->frameLimiter(targetRate, startP);
			if (time->frameLimiter(targetRate, startP)) {
				deltaT = 0.0333;
			}
			else {
				deltaT = time->checkTimer(startP);
			}
			start = time->timerStart();
			startP = &start;

			for (object* o : backgroundObjects) {
				o->setCamera(cam);
				o->eachFrame(&deltaT, p->getSprite());
				instance->objectAccess(o, cam);

			}
			for (object* o : newBackgroundObjects) {
				o->setCamera(cam);
				o->eachFrame(&deltaT, p->getSprite());
				instance->objectAccess(o, cam);

			}

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

			for (GameObject* g : foregroundObjects) {
				instance->objectAccess(g, cam);
			}
			for (GameObject* g : newForegroundObjects) {
				instance->objectAccess(g, cam);
			}

			if (ang == RIGHT) {
				cam->move(0, &deltaT, camSpeed);
				if (p->getGrounded()) {
					p->getAnimation()->toeAnim(&deltaT, true);
				}
				else {
					p->getAnimation()->runJump();
				}
				p->getSprite()->move(0, &deltaT, playerSpeed);



				if (cam->getPosition().x >= flagPos.x) {
					run = false;
				}

				//Special rule needed when next transition is vertical and next position is within camera shot
				else if (otherAngle == UP || otherAngle == DOWN) {
					if (cam->getPosition().x > otherFlagPos.x - 1920) {
						run = false;
					}
				}
				else if (flagPos.x - cam->getPosition().x <= (cam->getPosition().x + 1920) - otherFlagPos.x) {
					run = false;
				}
			}
			else if (ang == UP) {
				cam->move(270, &deltaT, camSpeed);
				p->getAnimation()->ladderAnim(&deltaT);
				p->getSprite()->move(270, &deltaT, playerSpeed);
				if (forward) {
					if (cam->getPosition().y + 1080 <= flagPos.y) {
						run = false;
					}
				}
				else {
					if (otherAngle == UP) {
						if (cam->getPosition().y + 1080 <= otherFlagPos.y) {
							run = false;
						}
					}
					else {
						if (cam->getPosition().y <= otherFlagPos.y) {
							run = false;
						}
					}
				}
			}

			else if (ang == DOWN) {
				cam->move(90, &deltaT, camSpeed);
				if (p->getControls()->getOnLadder()) {
					p->getAnimation()->ladderAnim(&deltaT);
					p->getSprite()->move(90, &deltaT, playerSpeed);
				}
				else {
					p->getAnimation()->runJump();
				}

				if (forward) {
					if (cam->getPosition().y >= flagPos.y) {
						run = false;
					}
				}
				else {
					if (otherAngle == UP) {
						if (cam->getPosition().y + 1080 >= otherFlagPos.y) {
							run = false;
							cam->setPosition(Vector2f(cam->getPosition().x, otherFlagPos.y - 1080));
						}
					}
					else {
						if (cam->getPosition().y >= otherFlagPos.y) {
							run = false;
							cam->setPosition(Vector2f(cam->getPosition().x, otherFlagPos.y));
						}
					}
				}

			}

			else if (ang == LEFT) {
				cam->move(180, &deltaT, camSpeed);
				if (p->getGrounded()) {
					p->getAnimation()->toeAnim(&deltaT, false);
				}
				else {
					p->getAnimation()->runJump();
				}
				p->getSprite()->move(180, &deltaT, playerSpeed);
				if (cam->getPosition().x + 1920 <= flagPos.x) {
					run = false;
				}

				//Special rule needed when next transition is vertical and next position is within camera shot
				//Needs to be tested
				else if (otherAngle == UP || otherAngle == DOWN) {
					if (cam->getPosition().x < otherFlagPos.x) {
						run = false;
					}
				}

				else if (flagPos.x - cam->getPosition().x >= (cam->getPosition().x + 1920) - otherFlagPos.x) {
					run = false;
				}
			}

			instance->objectAccess(door1, cam);
			instance->objectAccess(door2, cam);

			p->updateHitbox();
			//p->updateLighting();
			//lightingCheck();
			instance->objectDisplay(p->getSprite(), cam);
			instance->UIDisplay(p->getUI());

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

	EndFlag* getFlag(int section) {
		EndFlag* thisOne = NULL;
		for (EndFlag* flag : stage->getFlags()) {
			if (flag->getSection() == section) {
				thisOne = flag;
			}
		}

		return thisOne;
	}

	void forceLoadSection(int i, SoundCollection* soundCol) {
		section = i;

		stage->updateSection(i);
		checkLastFlagRight();
		loadFlag();
		loadSection(soundCol);
		deletePrevSection();
		updateFlags();
		checkObBefore();
	}
	void loadSection(SoundCollection* soundCol) {

		stage->reload(stageName, to_string(section), soundCol);

		loadFlag();

		newTileList = stage->getTiles();
		newZ2List = stage->getZ2List();
		newZ3List = stage->getZ3List();
		newZ4List = stage->getZ4List();
		objects = stage->getObjects();
		newBackgroundObjects = stage->getBackgroundObjects();
		newForegroundObjects = stage->getForegroundObjects();
		deleteEnemies();
		enemies = stage->getEnemies();
		spawners = stage->getSpawners();
		items = stage->getItems();
		for (GameObject* ob : objects) {
			ob->initial();
		}
		for (enemy* e : enemies) {
			e->initial();
		}
		spawner = stage->getAreaSpawner();

		bossCheck();
	}

	void bossCheck() {
		bool bossHere = false;
		for (enemy* e : enemies) {

			if (e->getBar() != NULL) {
				masterHealth = e->getBar();
				bossHere = true;
			}

			if (e->getDeathAnims()[0] != NULL) {
				bossDeath = e->getDeathAnims()[0];
				bossDeath1 = e->getDeathAnims()[1];
				bossDeath2 = e->getDeathAnims()[2];
			}
			else {
				bossDeath = NULL;
				bossDeath1 = NULL;
				bossDeath2 = NULL;
			}

		}
		if (!bossHere) {
			masterHealth = NULL;
		}
	}

	void loadNextSection(SoundCollection* soundCol) {
		section++;

		loadSection(soundCol);
	}

	void deletePrevSection() {
		deleteObjects();
		tileList = newTileList;
		z2List = newZ2List;
		z3List = newZ3List;
		z4List = newZ4List;

		

		backgroundObjects = newBackgroundObjects;
		foregroundObjects = newForegroundObjects;
		newForegroundObjects.clear();
		newBackgroundObjects.clear();
		newTileList.clear();
		newZ2List.clear();
		newZ3List.clear();
		newZ4List.clear();
	}

	void deleteObjects() {
		for (GameObject* g : foregroundObjects) {
			delete g;
		}
		for (GameObject* g : backgroundObjects) {
			delete g;
		}
		
		for (tile* g : tileList) {
			delete g;
		}
		for (tile* g : z2List) {
			delete g;
		}
		for (tile* g : z3List) {
			delete g;
		}
		for (tile* g : z4List) {
			delete g;
		}
	}

	void deleteEnemies() {
		for (enemy* e : enemies) {
			delete e;
		}
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




	void enemyCollisionCheck(list<enemy*> eList, renderer* instance, float targetRate) {

		for (enemy* e : eList) {
			//p->takeDamage(e->getDamage());
			if (e->getAct() && e->getHitbox() != NULL) {
				if (hitboxCheck(e->getHitbox(), p->getHitbox())) {
					if (!p->isInvincible() && p->checkInControl()) {

						p->takeDamage(e->getDamage());
						e->playerHit();

					}

					//Does this need to be here?
					if (e->getIncrease() != NULL) {

						itemGet(instance, targetRate, e);

					}
				}
			}
		}
	}

	void itemCollisionCheck(renderer* instance, float targetRate) {
		for (Item* e : items) {
			if (e->getAct() && e->getHitbox() != NULL) {
				if (hitboxCheck(e->getHitbox(), p->getHitbox())) {
					if (e->getIncrease() != NULL) {
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



			for (object* ob : backgroundObjects) {
				instance->objectAccess(ob, cam);
			}


			for (tile* t : tileList) {
				tileDistanceCheck(instance, t);
			}

			for (tile* t : z4List) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->bObjectDisplay(t->getSprite(), cam);
				}
			}
			for (tile* t : z3List) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->bObjectDisplay(t->getSprite(), cam);
				}
			}
			for (tile* t : z2List) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->bObjectDisplay(t->getSprite(), cam);
				}
			}

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

			for (object* item : items) {
				instance->objectAccess(item, cam);
			}

			for (enemy* t : enemies) {
				if (t->getDamSprite() != NULL) {
					instance->objectDisplay(t->getDamSprite(), cam);
				}
				if (t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}
			instance->objectDisplay(p->getSprite(), cam);

			for (GameObject* g : foregroundObjects) {
				instance->bObjectDisplay(g->getSprite(), cam);
			}

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
			p->updateAmmoWithSound();
			instance->UIDisplay(p->getUI());
			instance->getWindow()->display();
			instance->getWindow()->clear();

		}
	}

	void tileDistanceCheck(renderer* instance, tile* t) {

		Vector2f camPos = Vector2f(cam->getPosition().x - (32 * 4), cam->getPosition().y - (32 * 4));
		Vector2u dist = Vector2u((1920 + camPos.x + (64 * 4)), 1080 + camPos.y + (64 * 4));
		//list<tuple <tile*, bool>>::iterator tileI = tileList.begin();


		bool display = false;

		Vector2f tilePos = t->getSprite()->getPosition();

		if (tilePos.x > camPos.x - (16 * 4) && tilePos.x < dist.x && tilePos.y > camPos.y - (16 * 4) && tilePos.y < dist.y) {
			display = true;
		}

		t->setDisplay(display);
		t->setAct(display);



	}

	/*void backgroundTileDistanceCheck(list<tile*> tiles) {

		for (tile* t : tiles) {
			bool display = false;
			if (t->getSprite()->getCameraPosition().x > -(16 * 4)) {
				if (t->getSprite()->getCameraPosition().x < 1920) {
					if (t->getSprite()->getCameraPosition().y > -(16 * 4)) {
						if (t->getSprite()->getCameraPosition().y < 1080) {
							{
								//t->setAct(true);
								display = true;
							}
						}
					}
				}

			}
			t->setDisplay(display);
		}
	}*/

	void backgroundTileDistanceCheck(list<tile*> tiles) {

		for (tile* t : tiles) {
			float x = t->getSprite()->getCameraPosition().x;
			bool displayX = !(x < -(16 * 4)) && !(1920 < x);

			float y = t->getSprite()->getCameraPosition().y;
			bool displayY = !(y < -(16 * 4)) && !(1080 < y);

			t->setDisplay(displayX && displayY);

		}

	}


	void enemyDistanceCheck(renderer* instance, list<enemy*> objects) {
		float camPos = cam->getPosition().x;
		float camEdge = cam->getPosition().x + 1920;

		enemy* toDelete = NULL;

		for (enemy* e : objects) {
			//If object is still active
			if (e->getAct()) {
				if (e->deleteOverY() && e->deleteOverX()) {
					e->setOffScreen(checkObOffScreen(e, cam->getPosition(), Vector2f(cam->getPosition().x + 1920, cam->getPosition().y + 1080)));
				}
				else if (e->deleteOverX()) {
					e->setOffScreen(checkObOffScreenX(e, cam->getPosition(), cam->getPosition().x + 1920));
				}
				else if (e->deleteOverY()) {
					e->setOffScreen(checkObOffScreenY(e, cam->getPosition(), cam->getPosition().y + 1080));
				}
				//and has just left the screen
				if (e->getOffScreen()) {
					e->setAct(false);
					e->setDisplay(false);
					//e->getSprite()->setPosition(Vector2f(-9999, -9999));
					if (e->isDead(&enemies)) {
						toDelete = e;
					}
					e->updateHitbox();
				}
			}
			else {
				//If object is not active and...

				//initial position has not left the screen
				if (!e->getInitOffScreen()) {
					e->setInitOffScreen(checkInitInScreen(e, camPos, camEdge));
					
				}
				//initial position has left the screen
				else {
					if (e->deleteOverX()) {
						e->setInitOffScreen(checkInitInScreen(e, camPos, camEdge));
					}
					else {
						e->setInitOffScreen(false);
					}
					//and initial position has just re-entered the screen
					if (!e->getInitOffScreen()) {

						e->initial();
						e->setDisplay(true);
						e->setAct(true);
					}
				}
			}
		}

		if (toDelete != NULL) {
			enemies.remove(toDelete);
		}
	}

	bool checkObOffScreen(enemy* e, Vector2f camPos, Vector2f camEdge) {
		if (e->getPosition().x > camEdge.x || e->getPosition().x + e->getSprite()->getSize().x < camPos.x) {
			return true;
		}
		if (e->getPosition().y > camEdge.y || e->getPosition().y + e->getSprite()->getSize().y < camPos.y) {
		
			return true;
		}

		return false;
	}
	bool checkObOffScreenX(enemy* e, Vector2f camPos, float camEdge) {
		if (e->getPosition().x > camEdge || e->getPosition().x + e->getSprite()->getSize().x < camPos.x) {
			return true;
		}

		return false;
	}

	bool checkObOffScreenY(enemy* e, Vector2f camPos, float camEdge) {
		if (e->getPosition().y > camEdge || e->getPosition().y + e->getSprite()->getSize().y < camPos.y) {
			return true;
		}

		return false;
	}

	bool checkInitInScreen(enemy* e, float camPos, float camEdge) {
		if (e->getInitialPosition().x > camEdge || e->getInitialPosition().x < camPos) {
			return true;
		}
		return false;
	}


	bool ladderTileCheck(tile* t) {

		if (t->getAct()) {
			if (t->getLadder() != NULL) {
				if (hitboxCheck(p->getLadderHitbox(), t->getLadder())) {
					p->setPosition(Vector2f(t->getLadder()->getPosition().x - 4, p->getSprite()->getPosition().y));
					return true;
				}
			}
		}

		return false;
	}

	bool headLadderTileCheck(tile* t) {

		if (t->getAct()) {
			if (t->getLadder() != NULL) {
				if (hitboxCheck(p->getHead(), t->getLadder())) {
					return true;
				}
			}
		}

		return false;
	}

	bool ladderBelowTileCheck(tile* t) {

		if (t->getAct()) {
			if (t->getLadder() != NULL) {
				if (hitboxCheck(p->getBelow(), t->getLadder())) {
					return true;
				}
			}
		}

		return false;
	}

	bool ladderAboveTileCheck(tile* t) {

		if (t->getAct()) {
			if (t->getLadder() != NULL) {
				if (hitboxCheck(p->getAbove(), t->getLadder())) {
					return true;
				}
			}
		}

		return false;
	}

	void onLadderTileCheck(list<tile*> t, renderer* instance) {
		p->getControls()->setInfrontOfLadder(false);

		bool lBelow = false;
		bool lAbove = false;
		bool getupAnim = false;

		for (tile* t : tileList) {
			

			if (ladderBelowTileCheck(t)) {
				p->getControls()->setLadderBelow(true);
				lBelow = true;
			}
			if (ladderAboveTileCheck(t)) {
				p->getControls()->setLadderAbove(true);
				lAbove = true;
			}

			if (p->getControls()->getOnLadder()) {
				onLadder = ladderTileCheck(t);
				p->getControls()->setInfrontOfLadder(true);
				ladderAbove(t);
				if (headLadderTileCheck(t)) {
					getupAnim = true;
				}
			}
		}


		if (lBelow == false) {
			p->getControls()->setLadderBelow(false);
		}
		if (lAbove == false) {
			p->getControls()->setLadderAbove(false);
		}
		if (!getupAnim) {
			p->getAnimation()->ladderGetUp();
		}
	}

	void tileCheck(renderer* instance, float* deltaT) {
		
		bool lBelow = false;
		bool lAbove = false;
		bool inWater = false;
		float movement = 0;

		///////////////////////////////////////////////////////////////////////////////////////////////
		for (tile* t : tileList) {
			if (!p->getControls()->getOnLadder()) {

				bool thisGround = false;
				bool thisC = false;

				if (t->getAct()) {

					float currentX = p->getSprite()->getPosition().x;
					if (t->getGround() != NULL) {
						if (!ground) {
							if (hitboxCheck(p->getFoot(), t->getGround())) {
								movement = t->getMovement();
								if (p->getSprite()->getAcceleration().y < 0 || p->getGrounded()) {
									p->getSprite()->setPosition(Vector2f(currentX, t->getGround()->getPosition().y - (p->getHitbox()->getSize().y + 12)));
									//cam->follow();

									ground = true;
									thisGround = true;
								}
							}
						}
					}
					if (!thisGround) {
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
									p->getSprite()->setPosition(Vector2f(t->getLeft()->getPosition().x - t->getLeft()->getSize().x - p->getHitbox()->getSize().x - 16, p->getSprite()->getPosition().y));
									p->updateHitbox();
								}
							}
						}
					}

					if (t->getLadder() != NULL) {
						if (hitboxCheck(p->getLadderHitbox(), t->getLadder())) {
							onLadder = true;
							p->getControls()->setInfrontOfLadder(true);
						}
					}

					if (t->getDeathBox() != NULL) {
						if (hitboxCheck(p->getHitbox(), t->getDeathBox())) {
							p->takeDamage(100);
						}
					}

					if (t->getWaterBox() != NULL) {
						
						if (hitboxCheck(p->getFoot(), t->getWaterBox())) {
							inWater = true;
						}
					}



				}


				

				if (ladderBelowTileCheck(t)) {
					p->getControls()->setLadderBelow(true);
					lBelow = true;
				}
				if (ladderAboveTileCheck(t)) {
					p->getControls()->setLadderAbove(true);
					lAbove = true;
				}

				if (p->getControls()->getOnLadder()) {
					onLadder = ladderTileCheck(t);
					p->getControls()->setInfrontOfLadder(true);
					ladderAbove(t);
					if (!headLadderTileCheck(t)) {
						p->getAnimation()->ladderGetUp();
					}
				}


				/*if (t->getDisplay()) {
					instance->objectDisplay(t->getSprite(), cam);
				}*/

				if (lBelow == false) {
					p->getControls()->setLadderBelow(false);
				}
				if (lAbove == false) {
					p->getControls()->setLadderAbove(false);
				}
			}
			if (t->checkDist()) {
				tileDistanceCheck(instance, t);
			}
		}

		if (inWater) {
			p->setWaterGravity();
		}
		else {
			p->resetGravity();
		}

		p->tileMovement(movement, deltaT);
		
	}


	bool hitboxCheck(objectHitbox* pHit, objectHitbox* hit) {

		return hitboxDetect::hitboxDetection(pHit, hit);


	}



};