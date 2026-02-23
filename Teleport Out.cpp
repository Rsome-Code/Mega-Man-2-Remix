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

	shared_ptr<animation> teleAnim;
	shared_ptr<animTimer> teleTimer;
	shared_ptr<movable> sprite;

	float toWait = 3;
	float toWait_left = toWait;
	bool run = true;
	float moveSpeed = 2000;

	shared_ptr<SoundBuffer> soundB;
	shared_ptr<Sound> sound;

public:


	virtual ~TeleportOut() {

	}

	TeleportOut(shared_ptr<movable> sprite) {
		this->sprite = sprite;
		teleAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(163, 136), Vector2i(22, 15)), IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(124, 128), Vector2i(7, 24)) }, sprite));
		teleAnim->setOffsetList(list<Vector2f>{ Vector2f(0 * 4, 6 * 4), Vector2f(0 * 4, 10 * 4), Vector2f(0 * 4, 6 * 4), Vector2f(7 * 4, 0 * 4)});
		teleTimer = shared_ptr<animTimer> (new animTimer(teleAnim, 12, false));

		soundB = shared_ptr<SoundBuffer> (new SoundBuffer());
		soundB->loadFromFile("assets\\sound\\teleport_out.wav");
		sound = shared_ptr<Sound>(new Sound());
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

	void loop(shared_ptr<renderer> instance, float targetRate, shared_ptr<player> player, list<shared_ptr<tile>> tileList, list<shared_ptr<tile>> z2List, list<shared_ptr<tile>> z3List, list<shared_ptr<tile>> z4List, list<shared_ptr<GameObject>> obList, list<shared_ptr<GameObject>> bObjects, shared_ptr<camera> cam) {

		shared_ptr<timer> time = shared_ptr<timer>(new timer());


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


			for (shared_ptr<GameObject> ob : bObjects) {
				instance->bObjectDisplay(ob->getSprite(), cam);
			}

			for (shared_ptr<tile> t : z4List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z3List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z2List) {
				instance->bObjectDisplay(t->getSprite(), cam);
			}



			//tileDistanceCheck(instance, tileList);

			for (shared_ptr<tile> t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			for (shared_ptr<object> ob : obList) {
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
