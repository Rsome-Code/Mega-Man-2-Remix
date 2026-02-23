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

class GameOverMenu {

	shared_ptr<UISprite> background;
	shared_ptr<UISprite> arrow;
	shared_ptr<Music> music;

	Vector2f contPos = Vector2f(198 * 4, 97 * 4);
	Vector2f stagePos = Vector2f(contPos.x, contPos.y + (16 * 4));
	Vector2f passPos = Vector2f(stagePos.x, stagePos.y + (16 * 4));
	

	shared_ptr<pController> control;
	bool upPressed = true;
	bool downPressed = true;
	bool startPressed = true;

	float arrowFlashTime = 0.2;
	float flashTime_left = arrowFlashTime;
	bool arrowDisplay = true;

	shared_ptr<SoundBuffer> optionB;
	shared_ptr<Sound> optionSound;


public:

	virtual ~GameOverMenu() {

	}

	enum Option {
		Continue, StageSelect, Password
	};
	Option option = Continue;

	GameOverMenu() {
		//control = shared_ptr<pController>(new pController();
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\NES - Mega Man 2 - Miscellaneous - Menus.png");
		background = shared_ptr<UISprite>(new UISprite(t, IntRect(1045, 282, 520, 288), Vector2f(0, 0), Vector2f(4, 4)));
		music = shared_ptr<Music>(new Music());
		music->openFromFile("Assets\\sound\\music\\password.wav");
		music->setLoop(true);
		music->setLoopPoints({ seconds(0), seconds(6.4) });
		arrow = shared_ptr<UISprite>(new UISprite(t, IntRect(1711, 693, 5, 8), passPos, Vector2f(4, 4)));

		optionB = shared_ptr<SoundBuffer> (new SoundBuffer());
		optionB->loadFromFile("Assets\\sound\\cursor_move.wav");

		optionSound = shared_ptr<Sound>(new Sound());
		optionSound->setBuffer(*optionB);

		music->setVolume(50);
	}

	void arrowFlash(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			flashTime_left = arrowFlashTime;
			arrowDisplay = !arrowDisplay;
		}
	}

	void moveUp() {
		option = Option(((option - 1)+3) % 3);
		arrowUpdate();
		optionSound->play();
	}
	void moveDown() {
		option = Option((option + 1) % 3);
		arrowUpdate();
		optionSound->play();
	}

	void arrowUpdate() {
		if (option == Password) {
			arrow->setCameraPosition(passPos);
		}
		else if (option == Continue) {
			arrow->setCameraPosition(contPos);
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
			moveDown();
			downPressed = true;
		}
		else if (!control->checkDOWN()) {
			downPressed = false;
		}

		if (control->checkSTART() && !startPressed) {
			startPressed = true;
			if (option != Password) {
				return true;
			}
		}
		else if (!control->checkSTART()) {
			startPressed = false;
		}

		return false;

	}

	Option loop(shared_ptr<renderer> instance, float targetRate) {
		shared_ptr<timer> time = shared_ptr<timer>(new timer());

		control = shared_ptr<pController>(new pController(instance->getWindow()));

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		bool run = true;

		music->play();

		arrowUpdate();

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


			run = !checkControls();
			arrowFlash(&deltaT);

			instance->UIDisplay(background);
			if (arrowDisplay) {
				instance->UIDisplay(arrow);
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();

		}

		music->stop();
		return option;
	}
	
};