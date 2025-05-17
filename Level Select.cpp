#include "UI Sprite.cpp"
#include "render logic.cpp"
#include "time.cpp"
#include "Render Logic.cpp"
#include "Camera.cpp"
#include "controller.cpp"
#include "xcontrol.cpp"
#pragma once

class LevelSelect {
	UISprite* cursor;
	UISprite* background;
	timer* time;
	camera* cam;
	int selection = 0;
	Vector2f backPos;
	pController* control;
	bool run = true;
	bool displayC = false;

	bool leftPressed = false;
	bool upPressed = false;
	bool downPressed = false;
	bool rightPressed = false;

	float flashTime = 0.1;
	float flashTime_left = flashTime;

public:

	LevelSelect(Texture* bg) {
		//metalMan = new UISprite("Option", );
		background = new UISprite("bg", bg, IntRect(0, 0, 771, 255), Vector2f(-150 * 4, 0), Vector2f(4, 4));
		backPos = background->getCameraPosition();
		cursor = new UISprite("pointer", bg, IntRect(204, 266, 42, 42), Vector2f(backPos.x + 300 *4, backPos.y + 27 *4), Vector2f(4, 4));
		control = new pController();
	}

	string loop(renderer* instance, double targetRate, Texture* bg) {

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


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

			checkController();

			setCursor();
			animateCursor(deltaT);

			instance->UIDisplay(background);
			if (displayC) {
				instance->UIDisplay(cursor);
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}

		string r;
		if (selection == 5) {
			r = "wood man";
		}
		return r;
	}

	bool checkA() {
		if (control->checkA()) {
			return true;
		}
		return false;
	}

	void animateCursor(float loopTime) {
		if (flashTime_left <= 0) {
			displayC = !displayC;
			flashTime_left = flashTime;
		}
		else {
			flashTime_left = flashTime_left - loopTime;
		}
	}

	void setCursor() {
		float x = 0;
		x = 64 * (selection % 3);
		float y = 0;
		y = (64 * int(selection / 3));
		cursor->setCameraPosition(Vector2f(backPos.x + (300 + x)*4, backPos.y + (27 + y)*4));
	}

	void moveUp() {
		selection = ((selection - 3) + 9) % -9;
	}
	void moveDown() {
		selection = (selection + 3) % 9;
	}

	void checkController() {
		if (control->checkLEFT() && !leftPressed) {
			selection = ((selection - 1) + 9)%-9;
			if (selection == 2 || selection == 5 || selection == 8) {
				moveDown();
			}
			leftPressed = true;
		}
		else if (!control->checkLEFT()) {
			leftPressed = false;
		}
		if (control->checkRIGHT() && !rightPressed) {
			selection = (selection + 1) % 9;
			if (selection == 3 || selection == 0 || selection == 6) {
				moveUp();
			}
			rightPressed = true;
		}
		else if (!control->checkRIGHT()) {
			rightPressed = false;
		}
		if (control->checkUP() && !upPressed) {
			moveUp();
			upPressed = true;
		}
		else if (!control->checkUP()) {
			upPressed = false;
		}
		if (control->checkDOWN() && !downPressed) {
			moveDown();
			downPressed = true;
		}
		else if (!control->checkDOWN()) {
			downPressed = false;
		}

		if (control->checkSTART() && selection != 4) {
			run = false;
		}

	}
};