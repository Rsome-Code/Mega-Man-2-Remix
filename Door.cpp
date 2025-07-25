#include "object.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#include "tile.cpp"
#include "render logic.cpp"
#include "camera.cpp"
#include "time.cpp"
#pragma once

class Door : public object {

	animation* anim;
	animTimer* aTimer;
	animation* revAnim;
	animTimer* revTimer;
	int section;
	timer* time;
	bool checkpoint = false;

public:
	Door(string levelName, Vector2f pos, int section) {
		Texture* t = new Texture();
		t->loadFromFile("assets\\Door\\" + levelName + ".png");
		sprite = new objectSprite("door", t, IntRect(0, 0, 16, 64), pos, Vector2f(4, 4));
		anim = new animation(list<IntRect>{IntRect(0, 0, 16, 64), IntRect(0, 0, 16, 48), IntRect(0, 0, 16, 32), IntRect(0, 0, 16, 16), IntRect(0, 0, 0,0)}, sprite);
		revAnim = new animation(list<IntRect>{IntRect(0, 0, 0,0), IntRect(0, 0, 16, 16), IntRect(0, 0, 16, 32), IntRect(0, 0, 16, 48), IntRect(0, 0, 16, 64)}, sprite);
		aTimer = new animTimer(anim, 4, false);
		revTimer = new animTimer(revAnim, 4, false);
		this->section = section;
		setCode();
	}

	void setCode() {
		code = "door";
	}

	void setCheckpoint() {
		checkpoint = true;

	}

	int getSection() {
		return section;
	}

	bool openAnim(float* deltaT){
		aTimer->run(deltaT);
		return aTimer->isFinished(deltaT);
	}

	bool closeAnim(float* deltaT) {
		revTimer->run(deltaT);
		return revTimer->isFinished();
	}

	void loop(renderer* instance, camera* cam, float targetRate, objectSprite* player, objectSprite* oDoor, list<tile*> tileList, list<tile*> z2List, list<tile*> z3List, list<tile*> z4List, bool open) {
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

			if (open) {
				if (openAnim(&deltaT)) {
					run = false;
				}
			}
			else {
				if (closeAnim(&deltaT)) {
					run = false;
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



			for (tile* t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			
			instance->objectDisplay(oDoor, cam);
			instance->objectDisplay(sprite, cam);
			instance->objectDisplay(player, cam);
			instance->getWindow()->display();
			instance->getWindow()->clear();


		}
	}
};