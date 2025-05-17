#include "UI Sprite.cpp"
#include "Object Sprite.cpp"
#include "movable object.cpp"
#include "render logic.cpp"
#include "time.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#include <random>
#include "physics object.cpp"
#pragma once

class StageIntro{
	Text* text;
	list<movable*> particles;
	UISprite* background;
	string textBuffer;
	bool birds;
	timer* time;
	camera* cam;
	animation* fadeIn;
	animTimer* aTimer;
	int fadeNum = 0;
	list<movable*>::iterator pIt;
	bool flap = false;
	float flapTime = 0.05;
	float flapTime_left = flapTime;
	physicsObject* boss;

public:
	StageIntro(string name, bool aHold, Texture* bg, Texture* bossT) {
		birds = aHold;
		textBuffer = name;
		background = new UISprite("bg", bg, IntRect(0, 359, 700, 293), Vector2f(0, 0), Vector2f(4, 4));
		birds = aHold;
		if (!aHold) {
			dotSetup(bg, IntRect(272, 265, 4, 4));
		}
		else {
			dotSetup(bg, IntRect(289, 281, 8, 8));
		}
		cam = new camera();

		pIt = particles.begin();
		UISprite* start = *pIt;
		if (!birds) {
			fadeIn = new animation(list<IntRect> {IntRect(272, 265, 4, 4), IntRect(272, 274, 4, 4), IntRect(272, 283, 4, 4)}, start);
		}
		else{
			fadeIn = new animation(list<IntRect> {IntRect(280, 263, 8, 8), IntRect(280, 272, 8, 8), IntRect(280, 281, 8, 8)}, start);
		}
		aTimer = new animTimer(fadeIn, 10, false);

		bossSetup(bossT);
	}

	void bossSetup(Texture* bossT) {
		
	}

	void dotSetup(Texture* bg, IntRect startFrame) {
		for (int i = 0; i < 25; i++) {
			float tempZ = (rand() % 100);
			tempZ = (tempZ / 100) + 1;
			//tempZ = 2;

			int tempX = rand();
			tempX = tempX % (1920 + mapLoc(tempZ, 1920));
			tempX = tempX - (mapLoc(tempZ, 1920));

			//int mapped = mapLoc(tempZ, 312);

			float tempY = (rand() % (312 + mapLoc(tempZ, 312)));

			tempY = tempY - (mapLoc(tempZ, 312) * 2);

			movable* temp = new movable("dot", bg, startFrame, Vector2f(tempX, tempY), Vector2f(4, 4), tempZ);
			particles.push_back(temp);
		}

		for (int i = 0; i < 30; i++) {
			float tempZ = (rand() % 100);
			tempZ = (tempZ / 100) + 1;
			//tempZ = 2;

			int tempX = rand();
			tempX = tempX % (1920 + mapLoc(tempZ, 1920));
			tempX = tempX - (mapLoc(tempZ, 1920));

			//int mapped = mapLoc(tempZ, 112*4);

			float tempY = (rand() % (312 + mapLoc(tempZ, (112 * 4))));

			tempY = tempY - (mapLoc(tempZ, (112 * 4) * 2));
			tempY = tempY + ((144 * 4) * (tempZ * 1.3));

			movable* temp = new movable("dot", bg, startFrame.getPosition(), startFrame.getSize(), Vector2f(tempX, tempY), Vector2f(4, 4), tempZ);
			particles.push_back(temp);
		}
	}

	int mapLoc(float tempZ, float norm) {
		float total = norm * tempZ;
		float diff = total - norm;
		//diff = diff / 2;
		return int(diff);
	}

	void moveParticles(float* deltaT, renderer* instance, camera* cam) {
		for (movable* ob : particles) {
			if (ob->getCameraPosition().x > 1920) {
				ob->setPosition(Vector2f(0, ob->getPosition().y));
				while (ob->getCameraPosition().x > 0) {
					instance->bObjectDisplay(ob, cam);
					ob->move(180, 50);
				}
			}
			ob->move(0, deltaT, 500);
			if (birds) {
				animateBirds();
			}
		}
	}

	void animateBirds() {
		bool thisFlap = flap;
		for (UISprite* bird : particles) {
			if (thisFlap) {
				bird->setRect(IntRect(280, bird->getRect().getPosition().y, bird->getRect().getSize().x, bird->getRect().getSize().y));
			}
			else {
				bird->setRect(IntRect(289, bird->getRect().getPosition().y, bird->getRect().getSize().x, bird->getRect().getSize().y));
			}
			thisFlap = !thisFlap;
		}
	}

	void bossAnim() {

	}

	//Copied animation for multiple sprites
	void fadeInAnim(float* deltaT){
		bool fin = false;
		UISprite* temp = *particles.begin();
		fadeIn->setSprite(temp);
		fin = aTimer->run(deltaT);
		if (fin) {
			for (UISprite* ob : particles) {
				fadeIn->setSprite(ob);
				fadeIn->setFrame();
			}
			cout << "f";
		}
		if (fin) {
			fadeNum++;
		}
	}

	void loop(renderer* instance, float targetRate) {
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

			moveParticles(&deltaT, instance, cam);

			bossAnim();

			if (birds) {
				
				if (deltaT >= flapTime_left) {
					flap = !flap;
					animateBirds();
					flapTime_left = flapTime;
				}
				else {
					flapTime_left -= deltaT;
				}
			}

			if (!(fadeNum >= 2)) {
				fadeInAnim(&deltaT);
			}

			instance->UIDisplay(background);
			list<objectSprite*> obs;
			for (objectSprite* ob : particles) {
				obs.push_back(ob);
			}
			instance->bObjectDisplay(obs, cam);

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
	}
};