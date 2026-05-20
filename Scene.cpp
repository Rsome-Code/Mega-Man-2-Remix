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
#include "colour.cpp"
#include "debug menu.cpp"
#pragma once

class scene {
	shared_ptr<player> p;
	shared_ptr<timer> time;
	shared_ptr<camera> cam;
	bool run = true;
	//grid* background;

	bool onLadder = false;


	bool interpolating = false;
	Vector2f newCamPos;

	string stageName;

	unique_ptr<abstractStage> stage;
	list<shared_ptr<tile>> tileList;
	list<shared_ptr<tile>> z2List;
	list<shared_ptr<tile>> z3List;
	list<shared_ptr<tile>> z4List;

	list<shared_ptr<tile>> newTileList;
	list<shared_ptr<tile>> newZ2List;
	list<shared_ptr<tile>> newZ3List;
	list<shared_ptr<tile>> newZ4List;

	list<shared_ptr<GameObject>> objects;
	list<shared_ptr<GameObject>> backgroundObjects;
	list<shared_ptr<GameObject>> newBackgroundObjects;
	list <shared_ptr<GameObject>> foregroundObjects;
	list <shared_ptr<GameObject>> newForegroundObjects;

	list<shared_ptr<enemy>> enemies;
	list<shared_ptr<EnemyBullet>> eBullets;
	list<shared_ptr<Item>> items;
	int section = 0;
	int transitionType;

	bool lastFlagRight = true;
	Vector2f lastFlagPos;

	Vector2f lastCheckPoint;

	shared_ptr<Texture> enemyT;

	//ScreenLighting* screenLighting;
	shared_ptr<Pause> pause;

	bool startPressed = true;
	bool paused = false;

	bool selectPressed = true;

	shared_ptr<text> readyText;
	shared_ptr<Font> font;

	shared_ptr<Texture> miscT;

	shared_ptr<EndFlag> currentFlag;
	shared_ptr<EndFlag> lastFlag;
	enum transitionAngle revLastAngle;

	shared_ptr<Door> door1;
	shared_ptr<Door> door2;

	//list<shared_ptr<enemy>> tempEnemies;
	list<shared_ptr<Spawner>> spawners;

	shared_ptr<SpawnArea> spawner;

	shared_ptr<AmmoBar>* masterHealth = NULL;

	shared_ptr<DeathAnim>* bossDeath = NULL;
	shared_ptr<DeathAnim>* bossDeath1 = NULL;
	shared_ptr<DeathAnim>* bossDeath2 = NULL;

	bool levelEnd = false;
	float levelEndTime = 10;
	float levelEndTime_left = levelEndTime;

	bool fallDeath = true;

	shared_ptr<SoundBuffer> masterDeathB;
	shared_ptr<Sound> masterDeathSound;

	bool victoryPlay = false;
	shared_ptr<Music> victoryMusic;

	bool nextFlagActive = true;
	bool lastFlagActive = true;

	shared_ptr<TeleportOut> teleExit;

	list<shared_ptr<ItemBullet>> itemBullets;

	int maxTelePos;

public:

	virtual ~scene() {
		
	}

	scene(shared_ptr<player> pl, unique_ptr<abstractStage> stg, shared_ptr<Texture> en) {

		enemyT = en;

		p = move(pl);
		p->getSprite()->setMovable(true);
		time = shared_ptr<timer>(new timer());
		cam = shared_ptr<camera> (new camera(p->getSprite(), Vector2f(-960, -540)));
		//p->setPosition(stg->getInitialPlayer());
		cam->setPosition(stg->getInitialCamera());
		//background = new grid();
		stage = move(stg);
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

		font = shared_ptr<Font>(new Font());
		font->loadFromFile("Assets//font.otf");



		readyText = shared_ptr<text>(new text(string("READY"), Vector2f(900, 500), float(22), font, Colour::White()));

		loadFlag();

		miscT = shared_ptr<Texture> (new Texture());
		refreshMisc();

		door1 = stage->getDoor1();
		door2 = stage->getDoor2();

		for (shared_ptr<enemy> e : enemies) {
			e->initial();
		}

		masterDeathB = shared_ptr<SoundBuffer> (new SoundBuffer());
		masterDeathSound = shared_ptr<Sound>(new Sound());
		masterDeathB->loadFromFile("assets\\sound\\death.wav");
		masterDeathSound->setBuffer(*masterDeathB);

		victoryMusic = shared_ptr<Music>(new Music());
		victoryMusic->openFromFile("assets\\sound\\music\\15 - Victory.mp3");

		teleExit = shared_ptr<TeleportOut> (new TeleportOut(p->getSprite()));
		maxTelePos = stage->getTelePos();

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
		if (!beforeTileList.empty()) {
			obBeforeTile = beforeTileList[section];
		}
		else {
			obBeforeTile = false;
		}
	}

	void debugCheck(shared_ptr<DebugMenu> dMenu, shared_ptr<player> p, shared_ptr<renderer> instance, double* targetRate) {
		if (p->getController()->checkSELECT() && !selectPressed) {
			dMenu->loop(instance, targetRate, tileList, z2List, z3List, z4List, backgroundObjects, cam, p);
			selectPressed = true;
			deltaT = 0.000000000001;
		}
		else if (!p->getController()->checkSELECT()) {
			selectPressed = false;
		}
	}

	float deltaT = 0.00001;
	bool loop(shared_ptr<renderer> instance, double targetRate, shared_ptr<SoundCollection> soundCol, vector<bool> beforeTileList) {

		shared_ptr<DebugMenu> dMenu = shared_ptr<DebugMenu> (new DebugMenu(font, instance, &targetRate));

		auto start = time->timerStart();
		auto* startP = &start;
		deltaT = 0.00001;

		this->beforeTileList = beforeTileList;


		bool unPaused = false;

		//Change this to the section to be debugged.
		section = 10;

		p->enableControls(true);

		checkLastFlagRight();
		loadFlag();
		updateFlags();

		shared_ptr<Music> music = stage->getMusic();
		music->setVolume(30);


		startAnim(instance, targetRate, music, soundCol);
		respawn();
		deltaT = 0.00001;
		//p->heal(-27);

		pause = shared_ptr<Pause> (new Pause(stageName, p));

		

		while (instance->getWindow()->isOpen() && run) {

			

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
				//for (shared_ptr<tile> t : tileList) {
				//	t->reset();
			//	}

				//Uncomment this for debugger
				//deltaT = time->checkTimer(startP);
			}
			else {
				deltaT = time->checkTimer(startP);
			}

			debugCheck(dMenu, p, instance, &targetRate);

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
				p->teleportForceEnd(prevPosition);

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

			if (door1 != NULL) {
				if (door1->getSection() == section || door2->getSection() == section) {
					//300 is to standardise the minimum distance for teleporting in pre boss rooms.
					p->eachFrame(&deltaT, tileList, &itemBullets, 300, cam);
				}
				else {
					p->eachFrame(&deltaT, tileList, &itemBullets, maxTelePos, cam);
				}
			}
			else {
				p->eachFrame(&deltaT, tileList, &itemBullets, maxTelePos, cam);
			}

			checkFall();

			for (shared_ptr<GameObject> o : objects) {
				o->setCamera(cam);
				o->eachFrame(&deltaT, p->getSprite(), cam);
				o->eachFrame(&deltaT, p, cam, &tileList);
				o->eachFrame(&deltaT, p, cam);
				o->eachFrame(&deltaT, p->getSprite());
				o->eachFrame(&deltaT, p, cam, objects);
				o->eachFrame();
				

				list<shared_ptr<GameObject>> temp;
				for (shared_ptr<GameObject> g : enemies) {
					temp.push_back(g);
				}
				
				o->eachFrame(&deltaT, p->getSprite(), temp);
				//instance->bObjectDisplay(g->getSprite(), cam);
			}

			if (!p->isTeleporting()) {
				p->setTempGround(false);


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


			
			p->cameraLogic(cam, &deltaT);


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

			for (shared_ptr<Spawner> spawn : spawners) {

				spawn->getSprite()->setCameraPosition(Vector2f(spawn->getSprite()->getPosition().x - cam->getPosition().x, spawn->getSprite()->getPosition().y));
				if (spawn->getSprite()->getCameraPosition().x > 0 && spawn->getSprite()->getCameraPosition().x < 1920) {

					spawn->eachFrame(&deltaT, p, &enemies, cam, soundCol);
				}
			}

			for (shared_ptr<object> o : backgroundObjects) {
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

			if (!p->getGroundedOverride()) {
				p->setGrounded(p->getTempGround());
			}


			for (shared_ptr<tile> t : z4List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), t->getDisplay(), cam);
			}
			for (shared_ptr<tile> t : z3List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), t->getDisplay(), cam);

			}
			for (shared_ptr<tile> t : z2List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), t->getDisplay(), cam);

			}

			if (obBeforeTile) {
				for (shared_ptr<enemy> t : enemies) {
					if (t->getDamSprite() != NULL) {
						instance->objectDisplay(t->getDamSprite(), cam);
						
					}
					if (t->getSprite() != NULL) {

						instance->objectAccess(t, cam);
						if (t->getDisplay()) {
							instance->objectDisplay(t->getExtraSprites(), cam);
						}
						
						//instance->objectHitboxSetup(t->getHitbox(), cam);
						//instance->hitboxDisplay(t->getHitbox());


					}
				}
			}

			for (shared_ptr<tile> t : tileList) {
				t->animate(&deltaT);
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
					for (shared_ptr<objectSprite> sp : t->getInternalSprites()) {
						instance->objectDisplay(sp, cam);
					}
					
				}
			}

			if (door1 != NULL) {
				door1->animate(&deltaT);
				door2->animate(&deltaT);
			}
			
			if (door1 != NULL) {
				instance->objectAccess(door1, cam);
			}
			if (door2 != NULL) {
				instance->objectAccess(door2, cam);
			}

			for (shared_ptr<object> t : objects) {
				instance->objectSetup(t->getSprite(), cam);
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			if (!obBeforeTile) {
				for (shared_ptr<enemy> t : enemies) {
					if (t->getDamSprite() != NULL) {
						instance->objectDisplay(t->getDamSprite(), cam);
					}
					if (t->getSprite() != NULL) {

						instance->objectAccess(t, cam);
						if (t->getDisplay()) {
							instance->objectDisplay(t->getExtraSprites(), cam);
						}
						//instance->objectHitboxSetup(t->getHitbox(), cam);
						//instance->hitboxDisplay(t->getHitbox());

						/*if (t->getGround() != NULL) {
							instance->objectHitboxSetup(t->getGround(), cam);
							
							instance->hitboxDisplay(t->getGround());
						}*/

					}
				}
			}

			for (shared_ptr<Spawner> s : spawners) {
				if (s->getDisplay()) {
					instance->objectDisplay(s->getSprite(), cam);
				}
			}

			for (shared_ptr<ItemBullet> i : itemBullets) {
				if (i->getSprite() != NULL) {
					instance->objectDisplay(i->getSprite(), cam);
				}
			}

			if (bossDeath != NULL) {
				shared_ptr<DeathAnim> bD = *bossDeath;
				instance->objectDisplay(bD->getSprite(), cam);
				bD = *bossDeath1;
				instance->objectDisplay(bD->getSprite(), cam);
				bD = *bossDeath2;
				instance->objectDisplay(bD->getSprite(), cam);
			}



			for (shared_ptr<object> i : items) {
				instance->objectAccess(i, cam);
			}

			for (shared_ptr<object> o : eBullets) {
				instance->objectAccess(o, cam);
				instance->objectDisplay(o->getSprites(), cam);
			}

			for (shared_ptr<GameObject> g : foregroundObjects) {
				list<shared_ptr<GameObject>> temp;
				for (shared_ptr<GameObject> g : enemies) {
					temp.push_back(g);
				}
				g->setCamera(cam);
				g->eachFrame(&deltaT, p->getSprite(), temp);
				instance->bObjectDisplay(g->getSprite(), cam);
			}

			if (p->getActiveWeapon()->getName() != "Time Stopper") {

				list<shared_ptr<bullet>> bList = p->getBullets();

				for (shared_ptr<bullet> b : bList) {
					instance->objectDisplay(b->getSprites(), cam);
					instance->objectDisplay(b->getSprite(), cam);
					//instance->objectHitboxDisplay(b->getHitbox(), cam);
				}

			}
			else {
				instance->UIDisplay(p->getUIBullets());
			}

			if (p->getDamEffect() != NULL) {
				instance->objectAccess(p->getDamEffect(), cam);
			}

			

			for (shared_ptr<GameObject> o : backgroundObjects) {
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
				shared_ptr<AmmoBar> a = *masterHealth;
				instance->UIDisplay(a->getSprites());
			}


			/*for (shared_ptr<enemy> e : enemies) {
				instance->objectHitboxSetup(list<shared_ptr<objectHitbox>> {e->getHitbox()}, cam);
				instance->hitboxDisplay(list<shared_ptr<objectHitbox>> { e->getHitbox()});
			}
			*/

			//instance->objectHitboxDisplay(p->getHitbox(), cam);
			//instance->objectHitboxDisplay(p->getHead(), cam);
			



			instance->getWindow()->display();
			instance->getWindow()->clear();

		}



		if (levelEnd) {
			list<shared_ptr<GameObject>> temps = objects;
			temps.push_back(door1);
			temps.push_back(door2);
			teleExit->loop(instance, targetRate, p, tileList, z2List, z3List, z4List, temps, backgroundObjects, cam);
		}

		

		return levelEnd;
	}

	void itemBulletLoop(float deltaT) {
		shared_ptr<ItemBullet> toDelete = NULL;

		

		for (shared_ptr<ItemBullet> iBul : itemBullets) {
			if (iBul->eachFrame(&deltaT)) {
				toDelete = iBul;
			}

			if (p->getSprite()->getVVelocity() <= 0) {
				if (hitboxCheck(iBul->getHit(), p->getFoot())) {
					p->getSprite()->setPosition(Vector2f(p->getSprite()->getPosition().x, iBul->getHit()->getPosition().y - (p->getHitbox()->getSize().y + 12)));
					//cam->follow();
					p->setTempGround(true);
				}
			}
		}



		if (toDelete != NULL) {
			itemBullets.remove(toDelete);
		}
	}

	void allTileOn(list<shared_ptr<tile>> tL) {
		for (shared_ptr<tile> t : tL) {
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

	void spawnLoop(float deltaT, shared_ptr<SoundCollection> soundCol) {
		spawner->eachFrame(p, deltaT, &enemies, cam, soundCol);
	}

	void enemyBullets(float deltaT) {

		list<shared_ptr<EnemyBullet>>::iterator it = eBullets.begin();
		list<shared_ptr<EnemyBullet>>::iterator toDelete = eBullets.end();
		for (shared_ptr<EnemyBullet> b : eBullets) {
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

	bool bulletsCollide(shared_ptr<EnemyBullet> b) {
		list<shared_ptr<bullet>> bullets = p->getWeapon()->getBullets();
		if (b->getCollType() == EnemyBullet::CollisionType::DESTROY) {
			for (shared_ptr<bullet> playerB : bullets) {
				shared_ptr<objectHitbox> playerH = playerB->getHitbox();
				if (hitboxCheck(playerH, b->getHitbox())) {
					playerB->deflect();
				}
			}
		}
		else if (b->getCollType() == EnemyBullet::CollisionType::DESTROY) {
			for (shared_ptr<bullet> playerB : bullets) {
				shared_ptr<objectHitbox> playerH = playerB->getHitbox();
				if (hitboxCheck(playerH, b->getHitbox())) {
					return true;
				}
			}
		}
		return false;
	}



	bool checkEBullOffScreen(shared_ptr<EnemyBullet> b) {
		if (checkObOffScreen(b, cam->getPosition(), Vector2f(cam->getPosition().x + 1920, cam->getPosition().y + 1080))) {
			//eBullets.remove(b);
			//delete b;
			return true;
		}
		return false;
	}

	bool checkObOffScreen(shared_ptr<EnemyBullet> e, Vector2f camPos, Vector2f camEdge) {
		if (e->getPosition().x > camEdge.x || e->getPosition().x + e->getSprite()->getSize().x < camPos.x) {
			return true;
		}
		if (e->getPosition().y > camEdge.y || e->getPosition().y + e->getSprite()->getSize().y < camPos.y) {
			return true;
		}
		return false;
	}

	void pDeathCheck(shared_ptr<renderer> instance, float targetRate, shared_ptr<Music> music, shared_ptr<SoundCollection> soundCol) {
		if (p->getHP() <= 0) {

			music->stop();
			p->getWeapon()->stopSound();
			for (shared_ptr<enemy> e : enemies) {
				e->stopMusic();
			}



			if (death(instance, targetRate, cam)) {
				if (p->getLives() > 0) {
					startAnim(instance, targetRate, music, soundCol);
					respawn();
					//p->heal(p->getMaxHP());
					p->HPReset();
					p->setNotDead();

					//p->setLives(p->getLives() - 1);
				}
				else {
					run = false;

				}

			}


		}
	}

	void itemLoop(float deltaT) {
		for (shared_ptr<Item> o : items) {
			o->eachFrame(&deltaT, p->getSprite(), &tileList);
		}
	}

	bool death(shared_ptr<renderer> instance, float tRate, shared_ptr<camera> cam) {
		if (p->setDead()) {

			list<shared_ptr<GameObject>> tempL = objects;
			if (door1 != NULL) {
				tempL.push_back(door1);
			}
			if (door2 != NULL) {
				tempL.push_back(door2);
			}
			for (shared_ptr<Spawner> s : spawners) {
				tempL.push_back(s);
			}
			Freeze::stop(obBeforeTile, instance, tRate, p, tileList, z2List, z3List, z4List, tempL, enemies, eBullets, backgroundObjects, foregroundObjects, cam, 0.75);

			masterDeathSound->play();

		
			paused = true;
		}
		return p->checkDeathFinish();


	}

	void resetObjects() {
		for (shared_ptr<enemy> e : enemies) {
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

	shared_ptr<EndFlag> getLastCheckpoint() {
		return stage->getLastCheckpoint(section);
	}

	void startAnim(shared_ptr<renderer> instance, float targetRate, shared_ptr<Music> music, shared_ptr<SoundCollection> soundCol) {

		p->setGroundedOverride(false);

		p->setShootemControls(false);
		
		eBullets.clear();
		p->setDeathNull();
		music->play();

		shared_ptr<EndFlag> flag = getLastCheckpoint();

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

			for (shared_ptr<object> o : backgroundObjects) {
				o->setCamera(cam);
				o->eachFrame(&deltaT, p->getSprite());
				instance->objectAccess(o, cam);
			}

			for (shared_ptr<tile> t : z4List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z3List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z2List) {
				t->animate(&deltaT);
				instance->bObjectDisplay(t->getSprite(), cam);
			}

			
			for (shared_ptr<tile> t : tileList) {
				t->animate(&deltaT);
				tileDistanceCheck(instance, t);
			}
			for (shared_ptr<object> o : objects) {
				float* f = new float(0);
				o->eachFrame(f, p->getSprite());
				delete f;
			}
			for (shared_ptr<tile> t : tileList) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
					for (shared_ptr<objectSprite> sp : t->getInternalSprites()) {
						instance->objectDisplay(sp, cam);
					}
				}
			}

			for (shared_ptr<GameObject> g : foregroundObjects) {
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
		
		p->getSprite()->updateCameraPosition(cam->getPosition());
		p->start(cam->getPosition().y + (16 * 4));
		p->swapDirection();

	}

	void spawnItemFromEnemy(shared_ptr<enemy> en, shared_ptr<SoundCollection> soundCol) {

		Vector2f middle = Vector2f((en->getSprite()->getPosition().x + (en->getSprite()->getSize().x / 6)), (en->getSprite()->getPosition().y + (en->getSprite()->getSize().y / 2)));



		en->spawnItem(&items, miscT, middle, soundCol);

	}

	void refreshMisc() {
		miscT->loadFromFile("assets\\misc\\" + p->getActiveWeapon()->getName() + ".png");
		p->iniSplash(miscT);
	}

	void levelEndCheck(shared_ptr<enemy> e, shared_ptr<Music> music) {
		if (e->getCode() == stage->getName()) {

			p->enableControls(false);
			levelEnd = true;
			if (p->getHP() <= 0) {
				p->setHP(1);
			}
			masterDeathSound->play();


		}
	}

	void levelEndSequence(shared_ptr<Music> music, shared_ptr<renderer> instance, float tRate, shared_ptr<enemy> enemy) {
		music->stop();
		list<shared_ptr<GameObject>> tempL = objects;
		tempL.push_back(door1);
		tempL.push_back(door2);
		for (shared_ptr<Spawner> s : spawners) {
			tempL.push_back(s);
		}
		enemy->forceDamSprite();
		Freeze::stop(obBeforeTile, instance, tRate, p, tileList, z2List, z3List, z4List, tempL, enemies, eBullets, backgroundObjects, foregroundObjects, cam, 0.75);
		enemy->forceDamSpriteOff();
		paused = true;
		levelEndCheck(enemy, music);
		p->resetBullets();
	}

	void enemyCheck(float deltaT, shared_ptr<renderer> instance, float tRate, shared_ptr<Music> music, shared_ptr<SoundCollection> soundCol) {
		shared_ptr<enemy> toDelete = NULL;

		for (shared_ptr<enemy> enemy : enemies) {
			

			if (enemy->getHP() > 0) {


				for (shared_ptr<bullet> bull : p->getControls()->getBulletObjects()) {


					bull->specialColl(enemy);
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
									enemy->spawnObject(&objects, soundCol);
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

			if (enemy->eachFrame(&deltaT, p, &tileList, &enemies, &eBullets, &objects, soundCol) || enemyYCheck(enemy)) {
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
			
		}

	}

	
	
	bool enemyYCheck(shared_ptr<enemy> e) {
		if (e->getSprite()->getCameraPosition().y > 1080) {
			return e->yDeath(&enemies);
			return e->isDead(&enemies);
		}
		return false;
	}


	bool  checkPause(shared_ptr<renderer> instance, float targetRate) {
		if (p->getController()->checkSTART() && !startPressed && p->checkInControl()) {
			p->getAtomicFire()->resetHold();

			pause->loop(instance, targetRate, tileList, z2List, z3List, z4List, backgroundObjects, cam);
			refreshMisc();
			for (shared_ptr<object> o : items) {

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

	void ladderAbove(shared_ptr<tile> t) {

		if (t->getCeiling() != NULL) {
			if (hitboxCheck(p->getHead(), t->getCeiling())) {
				p->setPosition(Vector2f(p->getSprite()->getPosition().x, t->getCeiling()->getPosition().y + 2));
			}
		}

	}



	bool flagCheck(shared_ptr<renderer> instance, float targetRate, enum transitionAngle ang, Vector2f flagPos, bool nextSection, bool active, shared_ptr<SoundCollection> soundCol) {


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

	bool doorCheck(shared_ptr<renderer> instance, float targetRate) {



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

	void doorClose(shared_ptr<renderer> instance, float targetRate) {
		if (door1->getSection() == section - 1) {
			door1->loop(instance, cam, targetRate, p, door2->getSprite(), tileList, z2List, z3List, z4List, backgroundObjects, false);

		}
		if (door2->getSection() == section - 1) {
			door2->loop(instance, cam, targetRate, p, door1->getSprite(), tileList, z2List, z3List, z4List, backgroundObjects, false);

		}
	}


	void startTransition(shared_ptr<renderer> instance, float targetRate, transitionAngle ang, Vector2f flagPos, bool nextSection, shared_ptr<SoundCollection> soundCol) {
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

		for (shared_ptr<tile> t : tileList) {
			t->resetBeat();
		}

		sectionTransition(instance, targetRate, ang, flagPos, nextSection);

		deletePrevSection();

		lastFlagPos = lastFlag->getSprite()->getPosition();

	}

	void checkLastFlagRight() {
		shared_ptr<EndFlag> lastFlag = stage->getLastFlag(section);
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




	void sectionTransition(shared_ptr<renderer> instance, float targetRate, transitionAngle ang, Vector2f flagPos, bool forward) {
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

			for (shared_ptr<object> o : backgroundObjects) {
				o->setCamera(cam);
				o->eachFrame(&deltaT, p->getSprite());
				instance->objectAccess(o, cam);

			}
			for (shared_ptr<object> o : newBackgroundObjects) {
				o->setCamera(cam);
				o->eachFrame(&deltaT, p->getSprite());
				instance->objectAccess(o, cam);

			}

			for (shared_ptr<tile> t : newZ4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}


			for (shared_ptr<tile> t : newZ3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}

			for (shared_ptr<tile> t : newZ2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : newTileList) {
				instance->objectAccess(t, cam);
			}
			for (shared_ptr<tile> t : tileList) {
				instance->objectAccess(t, cam);
			}

			for (shared_ptr<GameObject> g : foregroundObjects) {
				instance->objectAccess(g, cam);
			}
			for (shared_ptr<GameObject> g : newForegroundObjects) {
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
		shared_ptr<EndFlag> thisOne = NULL;
		for (shared_ptr<EndFlag> flag : stage->getFlags()) {
			if (flag->getSection() == section) {
				thisOne = flag;
			}
		}
		if (thisOne != NULL) {
			stage->addEndFlag(thisOne);
		}
	}

	shared_ptr<EndFlag> getFlag(int section) {
		shared_ptr<EndFlag> thisOne = NULL;
		for (shared_ptr<EndFlag> flag : stage->getFlags()) {
			if (flag->getSection() == section) {
				thisOne = flag;
			}
		}

		return thisOne;
	}

	void forceLoadSection(int i, shared_ptr<SoundCollection> soundCol) {
		section = i;

		stage->updateSection(i);
		checkLastFlagRight();
		loadFlag();
		loadSection(soundCol);
		deletePrevSection();
		updateFlags();
		checkObBefore();
	}
	void loadSection(shared_ptr<SoundCollection> soundCol) {

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
		for (shared_ptr<GameObject> ob : objects) {
			ob->initial();
		}
		for (shared_ptr<enemy> e : enemies) {
			e->initial();
		}
		spawner = stage->getAreaSpawner();

		bossCheck();
	}

	void bossCheck() {
		bool bossHere = false;
		for (shared_ptr<enemy> e : enemies) {

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

	void loadNextSection(shared_ptr<SoundCollection> soundCol) {
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

		eBullets.clear();
	}

	void deleteObjects() {

	}

	void deleteEnemies() {
		
	}

	void lightingCheck() {
		for (shared_ptr<object> ob : objects) {
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




	void enemyCollisionCheck(list<shared_ptr<enemy>> eList, shared_ptr<renderer> instance, float targetRate) {

		for (shared_ptr<enemy> e : eList) {
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

	void itemCollisionCheck(shared_ptr<renderer> instance, float targetRate) {
		for (shared_ptr<Item> e : items) {
			if (e->getAct() && e->getHitbox() != NULL) {
				if (hitboxCheck(e->getHitbox(), p->getHitbox())) {
					if (e->getIncrease() != NULL) {
						itemGet(instance, targetRate, e);
					}
				}
			}
		}
	}

	void itemGet(shared_ptr<renderer> instance, float targetRate, shared_ptr<object> item) {
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

	void itemLoop(shared_ptr<renderer> instance, float targetRate, shared_ptr<object> item) {
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



			for (shared_ptr<object> ob : backgroundObjects) {
				instance->objectAccess(ob, cam);
			}


			for (shared_ptr<tile> t : tileList) {
				tileDistanceCheck(instance, t);
			}

			for (shared_ptr<tile> t : z4List) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->bObjectDisplay(t->getSprite(), cam);
				}
			}
			for (shared_ptr<tile> t : z3List) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->bObjectDisplay(t->getSprite(), cam);
				}
			}
			for (shared_ptr<tile> t : z2List) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->bObjectDisplay(t->getSprite(), cam);
				}
			}

			for (shared_ptr<tile> t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}
			for (shared_ptr<object> t : objects) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			for (shared_ptr<Spawner> s : spawners) {
				if (s->getDisplay()) {
					instance->objectDisplay(s->getSprite(), cam);
				}
			}

			for (shared_ptr<object> item : items) {
				instance->objectAccess(item, cam);
			}

			for (shared_ptr<enemy> t : enemies) {
				if (t->getDamSprite() != NULL) {
					instance->objectDisplay(t->getDamSprite(), cam);
				}
				if (t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}
			instance->objectDisplay(p->getSprite(), cam);

			for (shared_ptr<GameObject> g : foregroundObjects) {
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

	void tileDistanceCheck(shared_ptr<renderer> instance, shared_ptr<tile> t) {

		Vector2f camPos = Vector2f(cam->getPosition().x - (32 * 4), cam->getPosition().y - (32 * 4));
		Vector2u dist = Vector2u((1920 + camPos.x + (64 * 4)), 1080 + camPos.y + (64 * 4));
		//list<tuple <shared_ptr<tile>, bool>>::iterator tileI = tileList.begin();


		bool display = false;

		Vector2f tilePos = t->getSprite()->getPosition();

		if (tilePos.x > camPos.x - (16 * 4) && tilePos.x < dist.x && tilePos.y > camPos.y - (32 * 4) && tilePos.y < dist.y) {
			display = true;
		}

		t->setDisplay(display);
		t->setAct(display);



	}

	/*void backgroundTileDistanceCheck(list<shared_ptr<tile>> tiles) {

		for (shared_ptr<tile> t : tiles) {
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

	void backgroundTileDistanceCheck(list<shared_ptr<tile>> tiles) {

		for (shared_ptr<tile> t : tiles) {
			float x = t->getSprite()->getCameraPosition().x;
			bool displayX = !(x < -(16 * 4)) && !(1920 < x);

			float y = t->getSprite()->getCameraPosition().y;
			bool displayY = !(y < -(16 * 4)) && !(1080 < y);

			t->setDisplay(displayX && displayY);

		}

	}


	void enemyDistanceCheck(shared_ptr<renderer> instance, list<shared_ptr<enemy>> objects) {
		float camPos = cam->getPosition().x;
		float camEdge = cam->getPosition().x + 1920;

		shared_ptr<enemy> toDelete = NULL;

		for (shared_ptr<enemy> e : objects) {
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

	bool checkObOffScreen(shared_ptr<enemy> e, Vector2f camPos, Vector2f camEdge) {
		if (e->getPosition().x > camEdge.x || e->getPosition().x + e->getSprite()->getSize().x < camPos.x) {
			return true;
		}
		if (e->getPosition().y > camEdge.y || e->getPosition().y + e->getSprite()->getSize().y < camPos.y) {
		
			return true;
		}

		return false;
	}
	bool checkObOffScreenX(shared_ptr<enemy> e, Vector2f camPos, float camEdge) {
		if (e->getPosition().x > camEdge || e->getPosition().x + e->getSprite()->getSize().x < camPos.x) {
			return true;
		}

		return false;
	}

	bool checkObOffScreenY(shared_ptr<enemy> e, Vector2f camPos, float camEdge) {
		if (e->getPosition().y > camEdge || e->getPosition().y + e->getSprite()->getSize().y < camPos.y) {
			return true;
		}

		return false;
	}

	bool checkInitInScreen(shared_ptr<enemy> e, float camPos, float camEdge) {
		if (e->getInitialPosition().x > camEdge || e->getInitialPosition().x < camPos) {
			return true;
		}
		return false;
	}


	bool ladderTileCheck(shared_ptr<tile> t) {

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

	bool headLadderTileCheck(shared_ptr<tile> t) {

		if (t->getAct()) {
			if (t->getLadder() != NULL) {
				if (hitboxCheck(p->getHead(), t->getLadder())) {
					return true;
				}
			}
		}

		return false;
	}

	bool ladderBelowTileCheck(shared_ptr<tile> t) {

		if (t->getAct()) {
			if (t->getLadder() != NULL) {
				if (hitboxCheck(p->getBelow(), t->getLadder())) {
					return true;
				}
			}
		}

		return false;
	}

	bool ladderAboveTileCheck(shared_ptr<tile> t) {

		if (t->getAct()) {
			if (t->getLadder() != NULL) {
				if (hitboxCheck(p->getAbove(), t->getLadder())) {
					return true;
				}
			}
		}

		return false;
	}

	void onLadderTileCheck(list<shared_ptr<tile>> t, shared_ptr<renderer> instance) {
		p->getControls()->setInfrontOfLadder(false);

		bool lBelow = false;
		bool lAbove = false;
		bool getupAnim = false;

		for (shared_ptr<tile> t : tileList) {
			

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

	void tileCheck(shared_ptr<renderer> instance, float* deltaT) {
		
		bool lBelow = false;
		bool lAbove = false;
		bool inWater = false;
		float movement = 0;

		float frictionDecrease = 0;

		p->setFlightPush(false);

		///////////////////////////////////////////////////////////////////////////////////////////////
		for (shared_ptr<tile> t : tileList) {
			if (!p->getControls()->getOnLadder()) {

				bool thisGround = false;
				bool thisC = false;

				if (t->getAct()) {

					float currentX = p->getSprite()->getPosition().x;
					if (t->getGround() != NULL) {
						if (!p->getTempGround()) {
							if (hitboxCheck(p->getFoot(), t->getGround())) {
								movement = t->getMovement();
								frictionDecrease = t->getFrictionDecrease();
								if (p->getSprite()->getAcceleration().y < 0 || p->getGrounded()) {
									p->getSprite()->setPosition(Vector2f(currentX, t->getGround()->getPosition().y - (p->getHitbox()->getSize().y + 12)));
									//cam->follow();

									if (!p->getShootemControls()) {
										p->setTempGround(true);
										thisGround = true;
									}
									else {
										p->getSprite()->setPosition(Vector2f(currentX, t->getGround()->getPosition().y - (p->getHitbox()->getSize().y + (5 * 4))));
										p->updateFlightHitboxes();

										for (shared_ptr<GameObject> ob: objects) {
											if (ob->getCode() == "fly platform") {
												ob->follow(p);
											}
										}
									}
								}
							}
						}
					}
					if (!thisGround) {
						if (t->getCeiling() != NULL) {
							if (hitboxCheck(p->getHead(), t->getCeiling())) {

								if (p->getShootemControls()){
									p->getSprite()->setPosition(Vector2f(currentX, t->getCeiling()->getPosition().y + t->getCeiling()->getSize().y + (0 * 4)));
									p->updateFlightHitboxes();
									for (shared_ptr<GameObject> ob : objects) {
										
										if (ob->getCode() == "fly platform") {
											
											ob->follow(p);
										}

									}
								}
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

									p->setFlightPush(true);
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

		p->decreaseFriction(frictionDecrease);
		
	}


	bool hitboxCheck(shared_ptr<objectHitbox> pHit, shared_ptr<objectHitbox> hit) {

		return hitboxDetect::hitboxDetection(pHit, hit);


	}



};