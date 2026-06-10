#include "movable object.cpp"
#include "time.cpp"
#include "render logic.cpp"
#include "xcontrol.cpp"
#pragma once

class NewSaveMenu {

	bool run = true;

	string playerInput;

	

public:

	string getInput() {
		return playerInput;
	}

	bool loop(shared_ptr<renderer> instance, float targetRate, shared_ptr<pController> controller) {

		shared_ptr<Texture> tex = shared_ptr<Texture>(new Texture());
		tex->loadFromFile("assets//Name entry.png");

		shared_ptr<UISprite> background = shared_ptr<UISprite>(new UISprite(tex, IntRect(0, 0, 480, 272), Vector2f(0,0), Vector2f(4,4)));
	
		shared_ptr<timer> time = shared_ptr<timer>(new timer());

		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		

		
		shared_ptr<text> playerText = shared_ptr<text>(new text());

		shared_ptr<text> instructText = shared_ptr<text>(new text());

		shared_ptr<Font> font = shared_ptr<Font>(new Font());
		font->loadFromFile("assets//font.otf");
		playerText->setFont(font);
		playerText->setPosition(Vector2f(149 * 4, 135 * 4));
		playerText->setSize(36);

		instructText->setFont(font);
		instructText->setPosition(Vector2f(149 * 4, 115 * 4));
		instructText->setSize(36);

		instructText->setString("Enter Name:");
		

		bool firstFrame = true;

		shared_ptr<RectangleShape> rect = shared_ptr<RectangleShape>(new RectangleShape());
		rect->setFillColor(Color::White);
		rect->setSize(Vector2f(4, playerText->getSize()));

		rect->setPosition(Vector2f(200, 200));
		//rect->setSize(Vector2f(4, 4));


		while (instance->getWindow()->isOpen() && run) {
			Event event;

			
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();

				if (instance->getWindow()->hasFocus()) {
					if (event.type == sf::Event::TextEntered)
					{

						if (event.text.unicode == 8) {
							playerInput.pop_back();
						}
						else {
							if (playerInput.size() <= 8) {
								playerInput += event.text.unicode;
							}
							if (firstFrame) {
								playerInput.pop_back();
								firstFrame = false;
							}

						}

						playerText->setString(playerInput);

						display = true;
						flashTime_left = flashTime;
					}
				}
			}

			time->frameLimiter(targetRate, startP);
			deltaT = time->checkTimer(startP);
			start = time->timerStart();
			startP = &start;

			instance->UIDisplay(background);

			instance->textDisplay(playerText);
			instance->textDisplay(instructText);

			checkControls(controller);

			rect->setPosition(playerText->getPosition().x + (playerText->getRect().width + (2 *4)), playerText->getPosition().y);

			flashLoop(rect, deltaT);

			if (display) {
				instance->rectDisplay(rect);
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}

		return enter;
	}



private:
	bool startPressed = true;
	bool bPressed = true;
	bool enter = false;
	void checkControls(shared_ptr<pController> con) {
		if (con->checkSTART()) {
			if (!startPressed) {
				run = false;
				startPressed = true;
				enter = true;
			}
		}
		else {
			startPressed = false;
		}

		if (con->checkBOnlyController() || Keyboard::isKeyPressed(Keyboard::Scan::Escape)) {
			if (!bPressed) {
				run = false;
				bPressed = true;
				
			}
		}
		else {
			bPressed = false;
		}
	}

private:
	float flashTime = 0.5;
	float flashTime_left = flashTime;
	bool display = true;
	void flashLoop(shared_ptr<RectangleShape> rect, float deltaT) {
		flashTime_left -= deltaT;

		if (flashTime_left <= 0) {
			display = !display;
			flashTime_left = flashTime;
		}

	}
};