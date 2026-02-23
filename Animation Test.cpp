#include "animation timer.cpp"
#include "render logic.cpp"
#include "Xcontrol.cpp"
#include "time.cpp"
#pragma once

class AnimationTest {
	shared_ptr<objectSprite> grid;
	shared_ptr<objectSprite> object;
	shared_ptr<animation> anim;
	shared_ptr<timer> time;

	bool flipped = false;

	bool leftPressed = false;
	bool rightPressed = false;

	virtual ~AnimationTest(){

	}

public:
	AnimationTest(list<IntRect> animList, list<Vector2f> offsetList, shared_ptr<Texture> t, bool flipped) {
		
		IntRect start = *animList.begin();
		object = shared_ptr<objectSprite>(new objectSprite("o", t, start, Vector2f(100, 100), Vector2f(4,4), 1));
		this->anim = shared_ptr<animation>(new animation(animList, object));
		anim->setOffsetList(offsetList);
		shared_ptr<Texture> bT = shared_ptr<Texture> (new Texture());
		bT->loadFromFile("Assets\\grid.png");
		grid = shared_ptr<objectSprite>(new objectSprite("b",bT, IntRect(0,0,600,600), Vector2f(0,0), Vector2f(4,4), 1));
		time = shared_ptr<timer>(shared_ptr<timer>(new timer()));

		if (flipped) {
			anim->swapAll();
		}
	}

	void run(shared_ptr<renderer> instance, float targetRate) {
		shared_ptr<camera> cam = shared_ptr<camera>(new camera());
		cam->setZoom(2);
		shared_ptr<pController> p = shared_ptr<pController>(new pController(instance->getWindow()));
		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		rightPressed = true;
		anim->nextFrame(false);


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
				if (!anim->checkStart()) {
					anim->prevFramePerm();
				}
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