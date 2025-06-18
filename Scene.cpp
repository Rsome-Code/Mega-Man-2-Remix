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
	}

public:


	void loop(renderer* instance, double targetRate) {

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		//Texture* t = new Texture();

		//objectSprite* wall = new objectSprite ("left wall", t, Vector2i(0,0), Vector2i(100, 4000), Vector2f(0, 200), Vector2f(1,1), 1);
		//objectHitbox* wallHit = new objectHitbox(IntRect(Vector2i(0,0), Vector2i(100, 4000)), true, wall);
		//objectSprite* wall1 = new objectSprite("right wall", t, Vector2i(0, 0), Vector2i(100, 4000), Vector2f(100, 200), Vector2f(1,1), 1);
		//objectHitbox* wallHit1 = new objectHitbox(IntRect(Vector2i(500, 0), Vector2i(100, 4000)), true, wall1);

		//objectSprite* ladder = new objectSprite("ladder", t, Vector2i(800, 324), Vector2i(400, 500), Vector2f(1000, 902), Vector2f(1, 1),1);
		//objectHitbox* ladderHit = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(400, 1000)), true, ladder);



		//Texture* te = new Texture();
		//te->loadFromFile("Assets\\Woodman.png");



		//cam->setXLimit(Vector2f(currentT->getSprite()->getPosition().x, nextT->getHitbox()->getPosition().x + nextT->getHitbox()->getSize().x - 1920));


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


			p->eachFrame(&deltaT);



			if (!p->isTeleporting()) {
				ground = false;
				onLadder = false;

				if (!p->getControls()->getOnLadder()) {


					tileCheck(tileList);

					p->setGrounded(ground);
					p->getControls()->setInfrontOfLadder(onLadder);

				}

				else {
					onLadder = ladderTileCheck(tileList);
					p->getControls()->setLadder(onLadder);
					p->getControls()->setInfrontOfLadder(onLadder);

					if (!headLadderTileCheck(tileList)) {
						p->getAnimation()->ladderGetUp();
					}

				}

				p->getControls()->setLadderBelow(ladderBelowTileCheck(tileList));
				p->getControls()->setLadderAbove(ladderAboveTileCheck(tileList));

			}

			cam->followX();

			flagCheck(instance, targetRate);



			enemyDistanceCheck(instance, objects);
			enemyCheck(objects);




			for (object* t : objects) {

				if (t->getAct() && t->getHitbox() != NULL) {
					t->eachFrame(&deltaT, p->getSprite());
					t->checkHit(p->getHitbox());


					for (bullet* h : p->getControls()->getBulletObjects()) {
						if (t->checkHurt(h->getHitbox())) {

							t->lowerHP(h->checkDamage(t));
							h->onHit();
						}
					}
				}
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
					if (t->getGround() != NULL) {
						instance->objectHitboxSetup(list<objectHitbox*> {t->getGround()}, cam);
						instance->hitboxDisplay(list<UIHitbox*> {t->getGround()});
					}
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
			instance->objectDisplay(p->getSprite(), cam);
			//instance->screenLightingDisplay(screenLighting->getRectangles());
			instance->UIDisplay(p->getUI());
			//transition* cur = *next(tIterator);
			//instance->objectHitboxSetup(list<objectHitbox*> {cur->getHitbox(), p->getFoot()}, cam);
			//instance->hitboxDisplay(list<UIHitbox*> {cur->getHitbox(), p->getFoot()});
			
			instance->getWindow()->display();
			instance->getWindow()->clear();


			

		}
	}

	void flagCheck(renderer* instance, float targetRate) {
		

		Vector2f flagPos = stage->getFlag();
		

		
		enum transitionAngle ang = stage->getAngle();

		cameraFlagCheck(flagPos, ang);
		if (ang == RIGHT) {
			if (p->getSprite()->getPosition().x + 48 >= flagPos.x) {

				lastFlagRight = true;

				loadNextSection();

				sectionTransition(instance, targetRate, ang, flagPos);

				deletePrevSection();
			}
		}
		else if (ang == DOWN) {
			if (p->getSprite()->getPosition().y + 48 >= flagPos.y) {
				lastFlagRight = false;
				loadNextSection();

				sectionTransition(instance, targetRate, ang, flagPos);

				deletePrevSection();
			}
		}
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

			


			for (tile* t : z4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : newZ4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : newZ3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : z2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : newZ2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (tile* t : tileList) {
				instance->objectAccess(t, cam);
			}
			for (tile* t : newTileList) {
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

			p->updateLighting();
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

	void enemyCheck(list<object*> eList) {
		for (object* e : eList) {
			if (e->getAct() && e->getHitbox() != NULL) {
				if (hitboxCheck(e->getHitbox(), p->getHitbox())) {
					if (!p->getDamage()) {
						p->takeDamage(e->getDamage());
					}
				}
			}
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

		/*if (pHit->getPosition().x > hit->getPosition().x + hit->getSize().x) {

		}
		else if (pHit->getPosition().x + pHit->getSize().x < hit->getPosition().x) {

		}
		else if (pHit->getPosition().y > hit->getPosition().y + hit->getSize().y) {

		}
		else if (pHit->getPosition().y + pHit->getSize().y < hit->getPosition().y) {

		}
		else {		
			return true;
		}
		return false;*/

		return hitboxDetect::hitboxDetection(pHit, hit);


	}
		
	
	
};