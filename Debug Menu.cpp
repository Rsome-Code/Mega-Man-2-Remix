#include "player.cpp"
#include "Debug Option.cpp"
#include "Resolution change.cpp"
#include "fps change.cpp"
#include "GObject.cpp"
#include "fullscreen.cpp"
#include "time.cpp"

#pragma once

class DebugMenu {
	vector<shared_ptr<DebugOption>> options;
	vector<shared_ptr<Text>> texts;
	int currentOption = 0;

	Vector2f position = Vector2f(600, 300);

	shared_ptr<RectangleShape> background;

	shared_ptr<pController> controller;
	bool downPressed = true;
	bool upPressed = true;
	bool APressed = true;
	bool startPressed = true;
	bool selectPressed = true;

	shared_ptr<UISprite> arrow;

public:

	virtual ~DebugMenu() {
	}

	DebugMenu(shared_ptr<Font> font, shared_ptr<renderer> instance, double* fps) {
		options.push_back(shared_ptr<DebugOption>(new ResolutionChange(instance)));
		options.push_back(shared_ptr<DebugOption>(new FPSChange(fps)));
		options.push_back(shared_ptr<DebugOption>(new FullscreenOption(instance)));

		background = shared_ptr<RectangleShape>(new RectangleShape());
		background->setFillColor(Colour::Black().getColour());
		background->setOutlineColor(Colour::White().getColour());
		background->setOutlineThickness(4);
		background->setPosition(position);
		background->setSize(Vector2f(720, 480));

		shared_ptr<Texture> tex = shared_ptr<Texture> (new Texture());
		tex->loadFromFile("assets\\arrow.png");
		

		textSetup(font);

		arrow = shared_ptr<UISprite>(new UISprite(tex, IntRect(0, 0, 5, 8), Vector2f(0, 0), Vector2f(4, 4)));
		updateArrow();
	}

	void textSetup(shared_ptr<Font> font) {

		Vector2f pos = Vector2f(position.x + 32, position.y + 16);

		for (shared_ptr<DebugOption> opt : options) {
			shared_ptr<Text> t = shared_ptr<Text>( new Text ());
			t->setFont(*font);
			t->setString(opt->getName());
			t->setCharacterSize(24);
			//t->setOrigin(pos);
			t->setPosition(pos);
			pos = Vector2f(pos.x, pos.y + 64);
			t->setFillColor(Colour::White().getColour());
			texts.push_back(t);
		}

	}

	void textUpdate() {
		int i = 0;
		for (shared_ptr<Text> t : texts) {
			t->setString(options[i]->getName());
			i++;
		}
	}

	void updateArrow() {
		arrow->setCameraPosition(Vector2f(texts[currentOption]->getPosition().x - 24, texts[currentOption]->getPosition().y));
	}

	void update(shared_ptr<renderer> instance, double* targetRate) {
		for (shared_ptr<DebugOption> opt : options) {
			opt->update(instance, targetRate);
		}
		textUpdate();
	}

	void loop(shared_ptr<renderer> instance, double* targetRate, list<shared_ptr<tile>> tileList, list<shared_ptr<tile>> z2List, list<shared_ptr<tile>> z3List, list<shared_ptr<tile>> z4List, list<shared_ptr<GameObject>> backgroundObjects, shared_ptr<camera> cam, shared_ptr<player> p) {

		shared_ptr<timer> time = shared_ptr<timer>(new timer());
		bool run = true;

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		controller = p->getController();

		update(instance, targetRate);

		while (instance->getWindow()->isOpen() && run) {
			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();
			}
			time->frameLimiter(*targetRate, startP);
			deltaT = time->checkTimer(startP);
			start = time->timerStart();
			startP = &start;


			for (shared_ptr<object> ob : backgroundObjects) {
				instance->objectAccess(ob, cam);
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



			//tileDistanceCheck(instance, tileList, cam);
			for (shared_ptr<tile> t : tileList) {

				if (t->getDisplay() && t->getSprite() != NULL) {
					instance->objectAccess(t, cam);
				}
			}

			instance->rectDisplay(background);

			instance->textDisplay(texts);

			instance->UIDisplay(arrow);


			instance->getWindow()->display();
			instance->getWindow()->clear();

			inputCheck(p, instance, targetRate, &run);
		}
	}


	void inputCheck(shared_ptr<player> p, shared_ptr<renderer> instance, double* targetFPS, bool* run) {
		
		if (controller->checkDOWN() && downPressed != true) {
			currentOption = (currentOption + 1) % options.size();
			downPressed = true;
			updateArrow();
		}
		else if (!controller->checkDOWN()) {
			downPressed = false;
		}

		if (controller->checkUP() && upPressed != true) {
			currentOption = (currentOption - 1) % options.size();
			upPressed = true;
			updateArrow();
		}
		else if (!controller->checkUP()) {
			upPressed = false;
		}

		if (controller->checkA() && APressed != true) {
			options[currentOption]->action(p, instance, targetFPS);
			APressed = true;
			textUpdate();
		}
		else if (!controller->checkA()) {
			APressed = false;
		}

		if (controller->checkSELECT() && selectPressed != true) {
			*run = false;
			selectPressed = true;
		}
		else if (!controller->checkSELECT()) {
			selectPressed = false;
		}
		
		
	}


	void tileDistanceCheck(shared_ptr<renderer> instance, list<shared_ptr<tile>> tileList, shared_ptr<camera> cam) {

		Vector2f camPos = Vector2f(cam->getPosition().x, cam->getPosition().y);
		Vector2u dist = Vector2u((instance->getWindow()->getSize().x + camPos.x), instance->getWindow()->getSize().y + camPos.y);
		//list<tuple <shared_ptr<tile>, bool>>::iterator tileI = tileList.begin();

		for (shared_ptr<tile> t : tileList) {
			bool display = false;

			Vector2f tilePos = t->getSprite()->getPosition();

			if (tilePos.x > camPos.x - (16 * 4) && tilePos.x < dist.x && tilePos.y > camPos.y - (16 * 4) && tilePos.y < dist.y) {
				display = true;
			}

			t->setDisplay(display);
			t->setAct(display);


		}

	}

};