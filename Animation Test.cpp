#include "animation timer.cpp"
#include "render logic.cpp"
#include "Xcontrol.cpp"
#include "time.cpp"
#pragma once

class AnimationTest {
	objectSprite* grid;
	objectSprite* object;
	animation* anim;
	timer* time;



	bool leftPressed = false;
	bool rightPressed = false;

public:
	AnimationTest(list<IntRect> animList, list<Vector2f> offsetList, Texture* t) {
		
		IntRect start = *animList.begin();
		object = new objectSprite("o", t, start, Vector2f(900, 500), Vector2f(4, 4), 1);
		this->anim = new animation(animList, object);
		anim->setOffsetList(offsetList);
		Texture* bT = new Texture();
		bT->loadFromFile("Assets\\grids.png");
		grid = new objectSprite("b",bT, IntRect(0,0,4905,4747), Vector2f(0,0), Vector2f(1,1), 1);
		time = new timer();
	}

	void run(renderer* instance, float targetRate) {
		camera* cam = new camera();
		pController* p = new pController();
		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		rightPressed = true;
		anim->nextFrame(false);
		cout << "1";

		while (true) {
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


			if (p->checkLEFT() && !leftPressed) {
				leftPressed = true;
				anim->prevFramePerm();
			}
			else if (leftPressed && !p->checkLEFT()) {
				leftPressed = false;
			}

			if (p->checkRIGHT() && !rightPressed) {
				rightPressed = true;
				anim->nextFrame(false);
				cout << "1";
				//anim->displayNextFrame();
			}
			else if (rightPressed && !p->checkRIGHT()) {
				rightPressed = false;
			}
			instance->objectDisplay(grid, cam);
			instance->objectDisplay(object, cam);
			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
	}
};