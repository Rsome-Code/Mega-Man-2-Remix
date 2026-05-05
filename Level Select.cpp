#include "UI Sprite.cpp"
#include "render logic.cpp"
#include "time.cpp"
#include "Render Logic.cpp"
#include "Camera.cpp"
#include "controller.cpp"
#include "xcontrol.cpp"
#include <SFML/audio.hpp>
#pragma once

class LevelSelect {
	shared_ptr<UISprite> cursor;
	shared_ptr<UISprite> background;
	shared_ptr<timer> time;
	shared_ptr<camera> cam;
	int selection = 4;
	Vector2f backPos;
	shared_ptr<pController> control;
	bool run = true;
	bool displayC = false;

	bool leftPressed = false;
	bool upPressed = false;
	bool downPressed = false;
	bool rightPressed = false;
	bool startPressed = true;

	float flashTime = 0.1;
	float flashTime_left = flashTime;

	list<shared_ptr<UISprite>> winIcons;
	vector<int> won;
	shared_ptr<Music> music;

	shared_ptr<SoundBuffer> optionB;
	shared_ptr<Sound> optionSound;
	shared_ptr<SoundBuffer> selectB;
	shared_ptr<Sound> selectSound;

	shared_ptr<RectangleShape> rectangle;
	float screenFlashTime = 0.1;
	float screenFlashTime_left = screenFlashTime;
	bool rectDisplay = true;

	int flashes = 5;
	

public:

	virtual ~LevelSelect() {

	}

	LevelSelect(shared_ptr<Texture> bg, bool bubble, bool heat, bool metal, bool wood, bool air, bool quick, bool flash, bool crash) {
		//metalMan = shared_ptr<UISprite>(new UISprite("Option", );
		background = shared_ptr<UISprite>(new UISprite("bg", bg, IntRect(0, 0, 771, 273), Vector2f(-150 * 4, 0), Vector2f(4, 4)));
		backPos = background->getCameraPosition();
		cursor = shared_ptr<UISprite>(new UISprite("pointer", bg, IntRect(204, 284, 42, 42), Vector2f(backPos.x + 300 *4, backPos.y + 32 *4), Vector2f(4, 4)));
		//control = shared_ptr<pController>(new pController();

		winIconSetup(bubble, heat, metal, wood, air, quick, flash, crash);
		music = shared_ptr<Music>(new Music());

		music->openFromFile("assets\\sound\\music\\4 - Stage Select.wav");

		music->setLoop(true);

		music->setLoopPoints({ seconds(1.35), sf::seconds(180) });
		music->setVolume(20);

		optionB = shared_ptr<SoundBuffer> (new SoundBuffer());
		optionB->loadFromFile("Assets\\sound\\cursor_move.wav");

		optionSound = shared_ptr<Sound>(new Sound());
		optionSound->setBuffer(*optionB);

		selectB = shared_ptr<SoundBuffer> (new SoundBuffer());
		selectB->loadFromFile("Assets\\sound\\teleport_out.wav");

		selectSound = shared_ptr<Sound>(new Sound());
		selectSound->setBuffer(*selectB);

		rectangle = shared_ptr<RectangleShape>(new RectangleShape);

		rectangle->setFillColor(Colour(255,255,255,220).getColour());
		rectangle->setPosition(0, 0);
		rectangle->setSize(Vector2f(1920, 1080));
		
	}

	void winIconSetup(bool bubble, bool heat, bool metal, bool wood, bool air, bool quick, bool flash, bool crash) {
		
		shared_ptr<UISprite> templateS = shared_ptr<UISprite>(new UISprite());

		templateS->setTexture(background->getTexture());
		templateS->setRect(IntRect(729, 280, 45, 47));
		templateS->setScale(Vector2f(4, 4));

		if (bubble) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite(templateS));
			temp->setCameraPosition(Vector2f((299-150) * 4, (33-2) * 4));
			winIcons.push_back(temp);
			won.push_back(0);
		}
		if (heat) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite(templateS));
			temp->setCameraPosition(Vector2f((299-150) * 4, (97-2) * 4));
			winIcons.push_back(temp);
			won.push_back(3);
		}
		if (metal) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite(templateS));
			temp->setCameraPosition(Vector2f((299 - 150) * 4, (161-2) * 4));
			winIcons.push_back(temp);
			won.push_back(6);
		}
		if (wood) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite(templateS));
			temp->setCameraPosition(Vector2f((427 - 150) * 4, (97-2) * 4));
			winIcons.push_back(temp);
			won.push_back(5);
		}
		if (air) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite(templateS));
			temp->setCameraPosition(Vector2f((363 - 150) * 4, (33-2) * 4));
			winIcons.push_back(temp);
			won.push_back(1);
		}
		if (quick) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite(templateS));
			temp->setCameraPosition(Vector2f((427 - 150) * 4, (33-2) * 4));
			winIcons.push_back(temp);
			won.push_back(2);
		}
		if (flash) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite(templateS));
			temp->setCameraPosition(Vector2f((363 - 150) * 4, (161-2) * 4));
			winIcons.push_back(temp);
			won.push_back(7);
		}
		if (crash) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite(templateS));
			temp->setCameraPosition(Vector2f((427 - 150) * 4, (161-2) * 4));
			winIcons.push_back(temp);
			won.push_back(8);
		}

		if (wood && crash && flash && air && quick && metal && bubble && heat) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite(templateS));
			temp->setTexture(background->getTexture());
			temp->setRect(IntRect(777, 282, 45, 45));
			temp->setCameraPosition(Vector2f((363 - 150) * 4, 97 * 4));
			temp->setScale(Vector2f(4, 4));
			winIcons.push_back(temp);
			
		}
		else {
			won.push_back(4);
		}
		
	}

	void flash(float deltaT) {
		screenFlashTime_left -= deltaT;
		if (screenFlashTime_left <= 0) {
			screenFlashTime_left = screenFlashTime;
			rectDisplay = !rectDisplay;
			if (!rectDisplay) {
				flashes -= 1;
			}
		}
	}

	string loop(shared_ptr<renderer> instance, double targetRate, shared_ptr<Texture> bg) {

		control = shared_ptr<pController>(new pController(instance->getWindow()));

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		music->play();

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
			
			for (shared_ptr<UISprite> sprite : winIcons) {
				instance->UIDisplay(sprite);
			}

			if (displayC) {
				instance->UIDisplay(cursor);
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}

		run = true;

		if (instance->getWindow()->isOpen()) {
			selectSound->play();
		}
		

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

			

			

			instance->UIDisplay(background);

			for (shared_ptr<UISprite> sprite : winIcons) {
				instance->UIDisplay(sprite);
			}

			
			instance->UIDisplay(cursor);
			
			if (rectDisplay) {
				instance->rectDisplay(rectangle);
			}
			instance->getWindow()->display();
			instance->getWindow()->clear();

			flash(deltaT);
			if (flashes <= 0) {
				run = false;
			}

		}

		string r;
		if (selection == 0) {
			r = "bubble man";
		}
		if (selection == 3) {
			r = "heat man";
		}
		else if (selection == 5) {
			r = "wood man";
		}
		else if (selection == 6) {
			r = "metal man";
		}
		else if (selection == 2) {
			r = "quick man";
		}
		else if (selection == 7) {
			r = "flash man";
		}
		else if (selection == 8) {
			r = "crash man";
		}
		music->stop();
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
		cursor->setCameraPosition(Vector2f(backPos.x + (300 + x)*4, backPos.y + (34 + y)*4));
	}

	void moveUp() {
		selection = ((selection - 3) + 9) % -9;
	}
	void moveDown() {
		selection = (selection + 3) % 9;
	}

	bool checkValid() {
		for (int i : won) {
			if (i == selection) {
				return false;
			}
		}
		return true;
	}

	void checkController() {
		if (control->checkLEFT() && !leftPressed) {
			optionSound->play();
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
			optionSound->play();
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
			optionSound->play();
			moveUp();
			upPressed = true;
		}
		else if (!control->checkUP()) {
			upPressed = false;
		}
		if (control->checkDOWN() && !downPressed) {
			optionSound->play();
			moveDown();
			downPressed = true;
		}
		else if (!control->checkDOWN()) {
			downPressed = false;
		}

		if (control->checkSTART() && !startPressed) {
			startPressed = true;
			if (checkValid()) {
				run = false;
			}
		}
		else if (!control->checkSTART()) {
			startPressed = false;
		}

	}
};