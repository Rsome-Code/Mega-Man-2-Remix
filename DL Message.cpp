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

class DLMessage {

	list<UISprite*> sprites;
	UISprite* light;


	bool rectDisplay;

	enum State {
		flash, alert, message
	};
	State state = flash;

	int flashNum = 8;
	float flashTime = 0.2;
	float flashTime_left = flashTime;

	string alertText;
	Text* alertT;
	string messageText;
	Text* messageT;

	float textTime = 0.2;
	float textTime_left = textTime;

	int charaNum = 0;

	bool delay = false;
	float delayTime = 0.7;
	float delayTime_left = delayTime;
	UISprite* background;

public:
	DLMessage(Texture* t, list<UISprite*> sprites, Weapon* weap) {
		this->sprites = sprites;



		Font* font = new Font();
		font->loadFromFile("assets\\font.otf");
		alertT = new Text();
		messageT = new Text();
		alertT->setFont(*font);
		messageT->setFont(*font);

		alertT->setPosition(766, 300);
		alertT->setCharacterSize(34);
		alertT->setLineSpacing(2);

		messageT->setPosition(766, 200);
		messageT->setCharacterSize(36);
		messageT->setLineSpacing(2);

		alertText = "Message from \nDr. Light.";
		messageText = weap->getName() + " \nCompleted! \nGet your \nWeapons Ready!";
		rectDisplay = false;

		UISprite* toD;
		for (UISprite* s : sprites) {
			if (s->getType() == "mega") {
				toD = s;
			}
			
			if (s->getType() == "BackGround") {
				background = s;
			}
			
		}
		this->sprites.remove(toD);
		light = new UISprite(t, IntRect(1733, 707, 32, 32), Vector2f(toD->getCameraPosition().x, 300) , Vector2f(4, 4));
		delete toD;

		
	}


	void loop(renderer* instance, float targetRate) {

		timer* time = new timer();


		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		bool run = true;

		while (run || delay) {
			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					run = false;
					instance->getWindow()->close();
				}
			}
			time->frameLimiter(targetRate, startP);
			deltaT = time->checkTimer(startP);
			start = time->timerStart();
			startP = &start;




			if (!delay) {
				instance->UIDisplay(sprites);
				if (state == flash) {
					flashLoop(deltaT);
					if (flashNum <= 0) {
						state = alert;
					}
				}

				else if (state == alert) {
					if (alertLoop(deltaT)) {
						state = message;
						charaNum = 0;
						delay = true;
					}
					instance->textDisplay(alertT);
				}

				else if (state = message) {
					if (messageLoop(deltaT)) {
						delay = true;
						run = false;
					}
					instance->textDisplay(messageT);
				}

				if (state == flash) {

				}
				else {
					instance->UIDisplay(light);
				}
				instance->getWindow()->display();
				instance->getWindow()->clear();
			}
			else {
				delayTime_left -= deltaT;
				if (delayTime_left <= 0) {
					delayTime_left = delayTime;
					delay = false;
				}
				
			}

			
		}
	}

	bool alertLoop(float deltaT) {
		textTime_left -= deltaT;
		if (textTime_left <= 0) {
			textTime_left = textTime;

			return alertNext();
		}
		return false;
	}

	bool messageLoop(float deltaT) {
		textTime_left -= deltaT;
		if (textTime_left <= 0) {
			textTime_left = textTime;

			return messNext();
		}
		return false;
	}

	bool alertNext() {
		charaNum++;
		string temp;
		for (int i = 0; i < charaNum; i++) {
			temp = temp + alertText[i];
		}

		alertT->setString(temp);

		return (charaNum == alertText.size());

	}

	bool messNext() {
		charaNum++;
		string temp;
		for (int i = 0; i < charaNum; i++) {
			temp = temp + messageText[i];
		}

		messageT->setString(temp);

		return (charaNum == messageText.size());

	}

	void flashLoop(float deltaT) {
		flashTime_left -= deltaT;
		if (flashTime_left <= 0) {
			flashTime_left = flashTime;
			rectDisplay = !rectDisplay;
			flashSpr();
			flashNum--;

		}
	}
	void flashSpr() {

		if (rectDisplay) {
			background->setRect(IntRect(Vector2i(background->getRect().getPosition().x, background->getRect().getPosition().y + 272), Vector2i(background->getRect().getSize())));
		}
		else {
			background->setRect(IntRect(Vector2i(background->getRect().getPosition().x, background->getRect().getPosition().y - 272), Vector2i(background->getRect().getSize())));
		}
	}
};