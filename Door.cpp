#include "object.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#include "tile.cpp"
#include "render logic.cpp"
#include "camera.cpp"
#include "time.cpp"
#include "player.cpp"
#include "GObject.cpp"
#include <SFML/audio.hpp>
#pragma once

class Door : public GameObject {

protected:
	shared_ptr<animation> anim;
	shared_ptr<animTimer> aTimer;
	shared_ptr<animation> revAnim;
	shared_ptr<animTimer> revTimer;
	int section;
	shared_ptr<timer> time;
	bool checkpoint = false;

	shared_ptr<SoundBuffer> soundB;
	shared_ptr<Sound> sound;

	bool firstOpen = true;

public:

	Door(string levelName, Vector2f pos, int section) {
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\Door\\" + levelName + ".png");
		sprite = shared_ptr<objectSprite>(new objectSprite("door", t, IntRect(0, 0, 16, 64), pos, Vector2f(4, 4)));
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(0, 0, 16, 64), IntRect(0, 0, 16, 48), IntRect(0, 0, 16, 32), IntRect(0, 0, 16, 16), IntRect(0, 0, 0,0)}, sprite));
		revAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(0, 0, 0,0), IntRect(0, 0, 16, 16), IntRect(0, 0, 16, 32), IntRect(0, 0, 16, 48), IntRect(0, 0, 16, 64)}, sprite));
		aTimer = shared_ptr<animTimer> (new animTimer(anim, 6, false));
		revTimer = shared_ptr<animTimer> (new animTimer(revAnim, 6, false));
		this->section = section;
		setCode();

		soundB = shared_ptr<SoundBuffer> (new SoundBuffer());
		soundB->loadFromFile("assets\\sound\\boss_door.wav");
		sound = shared_ptr<Sound>(new Sound());
		sound->setBuffer(*soundB);

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

	virtual void resetAnim() {};

	bool openAnim(float* deltaT){

		resetAnim();

		if (aTimer->run(deltaT)) {
			if (firstOpen) {
				firstOpen = false;
			}
			else {
				sound->play();
			}
			
		}
		return aTimer->isFinished(deltaT);
	}

	bool closeAnim(float* deltaT) {
		if (revTimer->run(deltaT)) {
			if (firstOpen) {
				firstOpen = false;
			}
			else {
				sound->play();
			}
		}
		return revTimer->isFinished();
	}

	virtual void animate(float* deltaT) {};

	void loop(shared_ptr<renderer> instance, shared_ptr<camera> cam, float targetRate, shared_ptr<player> player, shared_ptr<objectSprite> oDoor, list<shared_ptr<tile>> tileList, list<shared_ptr<tile>> z2List, list<shared_ptr<tile>> z3List, list<shared_ptr<tile>> z4List, list<shared_ptr<GameObject>> bObjects, bool open) {
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
					firstOpen = true;
				}
			}
			else {
				if (closeAnim(&deltaT)) {
					run = false;
					firstOpen = true;
				}
			}

			for (shared_ptr<GameObject> bObject : bObjects) {
				instance->bObjectDisplay(bObject->getSprite(), cam);
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



			for (shared_ptr<tile> t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			
			instance->objectDisplay(oDoor, cam);
			instance->objectDisplay(sprite, cam);
			instance->objectDisplay(player->getSprite(), cam);
			instance->UIDisplay(player->getUI());
			instance->getWindow()->display();
			instance->getWindow()->clear();


		}
		anim->reset();
		revAnim->reset();

	}
};