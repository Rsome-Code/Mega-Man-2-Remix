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
#include "item.cpp"
#include <list>
#include "temp enemy.cpp"
#include "spawn area.cpp"
#include "ammo bar.cpp"
#include <sfml/audio.hpp>
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

	text* readyText;
	Font font;

	Texture* miscT;

	EndFlag* currentFlag;
	EndFlag* lastFlag;
	enum transitionAngle revLastAngle;

	Door* door1;
	Door* door2;

	list<enemy*> tempEnemies;

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
		enemies = stage->getEnemies();

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
	}

public:

	bool afterT = false;
	bool justAfterT = false;

	void updateFlags() {
		currentFlag = stage->getCurrentFlag(section);
		lastFlag = stage->getCurrentFlag(section - 1);
		lastFlagPos = lastFlag->getSprite()->getPosition();
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

	bool loop(renderer* instance, double targetRate) {

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 8;
		
		bool unPaused = false;
		

		section = 13;

		p->enableControls(true);

		checkLastFlagRight();
		loadFlag();
		updateFlags();
		
		Music* music = stage->getMusic();
		

		startAnim(instance, targetRate, music);
		respawn();
		//p->heal(-27);
		
		

		while (instance->getWindow()->isOpen() && run) {
			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();
			}
			if (time->frameLimiter(targetRate, startP)) {
				deltaT = 0.0333;
			}
			else {
				deltaT = time->checkTimer(startP);
			}
			start = time->timerStart();
			startP = &start;

			pDeathCheck(instance, targetRate, music);

			if (levelEnd) {
				levelEndLoop(deltaT);
			}

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
			}
			p->eachFrame(&deltaT, tileList);

			checkFall();

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

			if (!p->isTeleporting()) {
				cameraFlagCheck(flagPos);
			}

			enemyDistanceCheck(instance, enemies);

			if (spawner != NULL) {
				spawnLoop(deltaT);
			}


			if (!p->isTeleporting()) {
				enemyCheck(deltaT, instance, targetRate, music);
				if (paused) {
					start = time->timerStart();
					startP = &start;
					paused = false;
				}
			}

			enemyBullets(deltaT);

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

			for (enemy* t : enemies) {
				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
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


			instance->objectDisplay(p->getBullets(), cam);
			if (p->getDamEffect() != NULL) {
				instance->objectAccess(p->getDamEffect(), cam);
			}


			instance->objectAccess(door1, cam);
			instance->objectAccess(door2, cam);


			//p->updateLighting();
			//lightingCheck();
			p->checkHold();
			instance->objectDisplay(p->getSprites(), cam);

			p->getSprite()->setRect(IntRect(Vector2i(p->getSprite()->getRect().getPosition().x, p->getBeforeHold()), p->getSprite()->getRect().getSize()));
			//instance->screenLightingDisplay(screenLighting->getRectangles());
			instance->UIDisplay(p->getUI());
			if (masterHealth != NULL) {
				AmmoBar* a = *masterHealth;
				instance->UIDisplay(a->getSprites());
			}
			//transition* cur = *next(tIterator);
			//instance->objectHitboxSetup(list<objectHitbox*> { p->getAbove()}, cam);
			//instance->hitboxDisplay(list<UIHitbox*> { p->getAbove()});






			instance->getWindow()->display();
			instance->getWindow()->clear();



		}
		return levelEnd;
	}

	void checkFall() {
		if (p->getSprite()->getCameraPosition().y > 1080 && fallDeath) {
			
			p->takeDamage(28);
			
		}
	}

	void spawnLoop(float deltaT) {
		spawner->eachFrame(p, deltaT, &enemies, cam);
	}

	void enemyBullets(float deltaT) {

		list<EnemyBullet*>::iterator it = eBullets.begin();
		list<EnemyBullet*>::iterator toDelete = eBullets.end();
		for (EnemyBullet* b : eBullets) {
			b->eachFrame(&deltaT);
			if (hitboxCheck(p->getHitbox(), b->getHitbox())) {
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

	void pDeathCheck(renderer* instance, float targetRate, Music* music) {
		if (p->getHP() <= 0) {

			music->stop();


			if (death(instance, targetRate, cam)) {
				if (p->getLives() > 0) {
					startAnim(instance, targetRate, music);
					respawn();
					p->heal(p->getMaxHP());
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
			
			list<object*> tempL = objects;
			tempL.push_back(door1);
			tempL.push_back(door2);
			Freeze::stop(instance, tRate, p, tileList, z2List, z3List, z4List, tempL, enemies, eBullets, cam, 0.75);

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
			}
		}
	}

	EndFlag* getLastCheckpoint() {
		return stage->getLastCheckpoint(section);
	}

	void startAnim(renderer* instance, float targetRate, Music* music) {

		music->play();

		EndFlag* flag = getLastCheckpoint();

		section = flag->getSection() + 1;

		forceLoadSection(section);
		
		

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
				deltaT = 0.0333;
			}
			else {
				deltaT = time->checkTimer(startP);
			}
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

	void spawnItemFromEnemy(enemy* en) {
		
		Vector2f middle = Vector2f((en->getSprite()->getPosition().x + (en->getSprite()->getSize().x/6)), (en->getSprite()->getPosition().y + (en->getSprite()->getSize().y/2)));

		

		en->spawnItem(&items, miscT, middle);

	}

	void refreshMisc() {
		miscT->loadFromFile("assets\\misc\\" + p->getActiveWeapon()->getName() + ".png");
	}

	void levelEndCheck(enemy* e, Music* music) {
		if (e->getCode() == stage->getName()) {
			p->enableControls(false);
			levelEnd = true;
			masterDeathSound->play();
			

		}
	}

	void enemyCheck(float deltaT, renderer* instance, float tRate, Music* music) {
		enemy* toDelete = NULL;

		for (enemy* enemy : enemies) {

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
								spawnItemFromEnemy(enemy);

								if (enemy->getDeathAnims()[0] != NULL) {
									music->stop();
									list<object*> tempL = objects;
									tempL.push_back(door1);
									tempL.push_back(door2);
									Freeze::stop(instance, tRate, p, tileList, z2List, z3List, z4List, tempL, enemies, eBullets, cam, 0.75);
									paused = true;
									levelEndCheck(enemy, music);
								}
							}
						}
					}
					else if (enemy->checkHit(bull->getHitbox())) {
						bull->deflect();
					}

				}
			}

			if (enemy->eachFrame(&deltaT, p, &tileList, &enemies, &eBullets) || enemyYCheck(enemy)) {
				toDelete = enemy;
			}

			if (!levelEnd) {
				p->enableControls(enemy->getIntroDone());
				music->stop();
			}
			
			
		}
		if (toDelete != NULL) {
			enemies.remove(toDelete);
		}

	}

	bool enemyYCheck(enemy* e) {
		if (e->getSprite()->getCameraPosition().y > 1080) {
			return e->isDead(&enemies);
		}
		return false;
	}
	

	bool  checkPause(renderer* instance, float targetRate) {
		if (p->getController()->checkSTART() && !startPressed && p->checkInControl()) {
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

				bool door = doorCheck(instance, targetRate);

				startTransition(instance, targetRate, ang, flagPos, nextSection);
				
				if (door) {
					doorClose(instance, targetRate);
				}
				return true;
			}
		}
		else if (ang == DOWN && !fallDeath) {
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

	bool doorCheck(renderer* instance, float targetRate) {
		if (door1->getSection() == section) {
			door1->loop(instance, cam, targetRate, p->getSprite(), door2->getSprite(), tileList, z2List, z3List, z4List, true);
			return true;
		}
		if (door2->getSection() == section) {
			door2->loop(instance, cam, targetRate, p->getSprite(), door1->getSprite(), tileList, z2List, z3List, z4List, true);
			return true;
		}
		return false;
	}

	void doorClose(renderer* instance, float targetRate) {
		if (door1->getSection() == section -1) {
			door1->loop(instance, cam, targetRate, p->getSprite(), door2->getSprite(), tileList, z2List, z3List, z4List, false);
			
		}
		if (door2->getSection() == section -1) {
			door2->loop(instance, cam, targetRate, p->getSprite(), door1->getSprite(), tileList, z2List, z3List, z4List, false);
			
		}
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

			if (currentFlag->getAngle() == DOWN || lastFlag->getAngle() == DOWN) {
				fallDeath = false;
			}
			else {
				fallDeath = true;
			}

		}
	}
	void isCameraLeftOfFlag(Vector2f flagPos) {
		if (flagPos != Vector2f(0, 0)) {
			if (cam->getPosition().x <= flagPos.x) {
				cam->setPosition(Vector2f(flagPos.x, cam->getPosition().y));
				if (currentFlag->getAngle() == DOWN || lastFlag->getAngle() ==DOWN) {
					fallDeath = false;
				}
				else {
					fallDeath = true;
				}
			}
		}
	}

	void cameraFlagCheck(Vector2f flagPos) {
		fallDeath = true;
		if (currentFlag->getSprite()->getPosition().x > lastFlagPos.x) {

			if (fabs(currentFlag->getSprite()->getPosition().x - lastFlagPos.x) < 1920) {
				cam->setPosition(Vector2f(lastFlagPos.x, cam->getPosition().y));
			}
#

			//Should be in else statement?
			isCameraRightOfFlag(currentFlag->getSprite()->getPosition());

			isCameraLeftOfFlag(lastFlagPos);


		}
		else {
			if (fabs(currentFlag->getSprite()->getPosition().x - lastFlagPos.x) < 1920) {
				cam->setPosition(Vector2f(lastFlagPos.x - (1920 - (16 * 4)), cam->getPosition().y));
				fallDeath = false;
			}
			else {
				isCameraRightOfFlag(lastFlagPos);

				isCameraLeftOfFlag(currentFlag->getSprite()->getPosition());
			}
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
					p->getAnimation()->toeAnim(&deltaT, false);
				}
				else {
					p->getAnimation()->runJump();
				}
				p->getSprite()->move(180, &deltaT, 50);
				if (cam->getPosition().x + 1920 <= flagPos.x) {
					run = false;
				}
			}

			instance->objectAccess(door1, cam);
			instance->objectAccess(door2, cam);

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
		updateFlags();
	}
	void loadSection() {

		stage->reload(stageName, to_string(section));

		loadFlag();

		newTileList = stage->getTiles();
		newZ2List = stage->getZ2List();
		newZ3List = stage->getZ3List();
		newZ4List = stage->getZ4List();
		objects = stage->getObjects();
		enemies = stage->getEnemies();
		for (enemy* e : enemies) {
			e->initial();
		}
		spawner = stage->getSpawner();

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




	void enemyCollisionCheck(list<enemy*> eList, renderer* instance, float targetRate) {
		for (enemy* e : eList) {
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
			for (object* t : enemies) {
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


	void enemyDistanceCheck(renderer* instance, list<enemy*> objects) {
		float camPos = cam->getPosition().x;
		float camEdge = cam->getPosition().x + instance->getWindow()->getSize().x;

		enemy* toDelete = NULL;

		for (enemy* e : objects) {
			//If object is still active
			if (e->getAct() && e->getDisplay()) {
				e->setOffScreen(checkObOffScreen(e, camPos, camEdge));
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
					e->setInitOffScreen(checkInitInScreen(e, camPos, camEdge));
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

	bool checkObOffScreen(enemy* e, float camPos, float camEdge) {
		if (e->getPosition().x > camEdge || e->getPosition().x + e->getSprite()->getSize().x < camPos) {
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

	bool ladderTileCheck(list<tile*> tileList) {
		for (tile* t : tileList) {
			if (t->getAct()) {
				if (t->getLadder() != NULL) {
					if (hitboxCheck(p->getLadderHitbox(), t->getLadder())) {
						p->setPosition(Vector2f(t->getLadder()->getPosition().x - 4, p->getSprite()->getPosition().y));
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
								p->getSprite()->setPosition(Vector2f(t->getLeft()->getPosition().x - t->getLeft()->getSize().x - p->getHitbox()->getSize().x - 16, p->getSprite()->getPosition().y));
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