#include "UI Sprite.cpp"
#include "render logic.cpp"
#include "time.cpp"
#include "Render Logic.cpp"
#include "Camera.cpp"
#include "controller.cpp"
#include "Weapon.cpp"
#include "xcontrol.cpp"
#include <SFML/audio.hpp>
#pragma once

class EquipMenu {
	list<UISprite*> sprites;
	list<Text*> texts;

	UISprite* passwordS;
	UISprite* stageSelectS;
	pController* control;

	bool upPressed = true;
	bool downPressed = true;
	bool startPressed = true;

	bool onPass = true;

	UISprite* arrow;
	float arrowFlashTime = 0.2;
	float flashTime_left = arrowFlashTime;
	bool arrowDisplay = true;
	Vector2f passPos;
	Vector2f stagePos;
	
	SoundBuffer* optionB;
	Sound* optionSound;

public:

	EquipMenu(Texture* t, list<UISprite*> sprites, list<Text*> texts, Vector2f menuPos) {
		this->sprites = sprites;
		this->texts = texts;
		control = new pController();
		passPos = Vector2f(menuPos.x + (45 * 4), menuPos.y + (14 * 4));
		stagePos = Vector2f(menuPos.x + (45 * 4), menuPos.y + (30 * 4));
		passwordS = new UISprite(t, IntRect(1125, 860, 63, 7), passPos, Vector2f(4, 4));

		stageSelectS = new UISprite(t, IntRect(1125, 876, 94, 7), stagePos, Vector2f(4, 4));

		passPos = Vector2f(passPos.x - (16 * 4), passPos.y);
		stagePos = Vector2f(stagePos.x - (16 * 4), stagePos.y);

		arrow = new UISprite(t, IntRect(1711, 693, 5, 8), passPos, Vector2f(4, 4));

		optionB = new SoundBuffer();
		optionB->loadFromFile("Assets\\sound\\cursor_move.wav");

		optionSound = new Sound();
		optionSound->setBuffer(*optionB);
	}

	void moveUp() {
		optionSound->play();
		onPass = !onPass;
		if (onPass) {
			arrow->setCameraPosition(passPos);
		}
		else {
			arrow->setCameraPosition(stagePos);
		}
	}

	//Returns true when start is pressed
	bool checkControls() {

		if (control->checkUP() && !upPressed) {
			//optionSound->play();
			moveUp();
			upPressed = true;
		}
		else if (!control->checkUP()) {
			upPressed = false;
		}
		if (control->checkDOWN() && !downPressed) {
			//optionSound->play();
			moveUp();
			downPressed = true;
		}
		else if (!control->checkDOWN()) {
			downPressed = false;
		}

		if (control->checkSTART() && !startPressed) {
			startPressed = true;
			if (!onPass) {
				return true;
			}
		}
		else if (!control->checkSTART()) {
			startPressed = false;
		}

		return false;
		
	}



	bool menuLoop(float* deltaT) {
		

		return checkControls();

	}

	void arrowFlash(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			flashTime_left = arrowFlashTime;
			arrowDisplay = !arrowDisplay;
		}
	}

	bool loop(renderer* instance, float targetRate, Music* music) {
		timer* time = new timer();

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


			run = !menuLoop(&deltaT);

			arrowFlash(&deltaT);




			instance->UIDisplay(sprites);

			for (Text* t : texts) {
				instance->textDisplay(t);
			}

			instance->UIDisplay(passwordS);
			instance->UIDisplay(stageSelectS);
			if (arrowDisplay) {
				instance->UIDisplay(arrow);
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
		music->stop();

		return onPass;
	}
	
};