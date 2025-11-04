#include "movable object.cpp"
#include "animate.cpp"
#include "Animation Timer.cpp"
#include "tile.cpp"
#include "hitbox detector.cpp"
#include "render logic.cpp"
#include "player.cpp"
#include <SFML/audio.hpp>
#include "time.cpp"
#include "GObject.cpp"

#pragma once

class TeleportOut {

	animation* teleAnim;
	animTimer* teleTimer;
	movable* sprite;

	float toWait = 3;
	float toWait_left = toWait;
	bool run = true;
	float moveSpeed = 2000;

	SoundBuffer* soundB;
	Sound* sound;

public:

	TeleportOut(movable* sprite) {
		this->sprite = sprite;
		teleAnim = new animation(list<IntRect>{IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(163, 136), Vector2i(22, 15)), IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(124, 128), Vector2i(7, 24)) }, sprite);
		teleAnim->setOffsetList(list<Vector2f>{ Vector2f(0 * 4, 6 * 4), Vector2f(0 * 4, 10 * 4), Vector2f(0 * 4, 6 * 4), Vector2f(7 * 4, 0 * 4)});
		teleTimer = new animTimer(teleAnim, 12, false);

		soundB = new SoundBuffer();
		soundB->loadFromFile("assets\\sound\\teleport_out.wav");
		sound = new Sound();
		sound->setBuffer(*soundB);
	
	}

	void move(float* deltaT) {
		sprite->move(270, deltaT, moveSpeed);
	}

	void eachFrame(float* deltaT) {
		

		if (teleTimer->isFinished()) {

			move(deltaT);

			toWait_left -= *deltaT;
			
			if (toWait_left <= 0) {
				toWait_left = toWait;
				run = false;
			}
		}
		else {
			teleTimer->run(deltaT);
		}
	}

	void loop(renderer* instance, float targetRate, player* player, list<tile*> tileList, list<tile*> z2List, list<tile*> z3List, list<tile*> z4List, list<GameObject*> obList, list<GameObject*> bObjects, camera* cam) {

		timer* time = new timer();


		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;



		teleAnim->thisFrame();

		sound->play();

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


			eachFrame(&deltaT);


			for (GameObject* ob : bObjects) {
				instance->bObjectDisplay(ob->getSprite(), cam);
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



			//tileDistanceCheck(instance, tileList);

			for (tile* t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			for (object* ob : obList) {
				if (ob->getDisplay()) {
					instance->objectAccess(ob, cam);
				}
			}

			

			instance->objectDisplay(player->getSprites(), cam);
			instance->objectDisplay(player->getSprite(), cam);

			instance->UIDisplay(player->getUI());

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
		run = true;
	}
};
