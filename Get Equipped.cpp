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

class EquipAnim {

	enum State {
		Fading, Texting, Flashing, Done
	};

	string symbolText;
	shared_ptr<Text> symbol;
	string getEquipText;
	shared_ptr<Text> getEquip;

	float textTime = 0.2;
	float textTime_left = textTime;
	bool symbolDone = false;
	int charaNum = 0;

	float flashTime = 0.05;
	float flashTime_left = flashTime;
	float flashFinishTime = 2;

	shared_ptr<UISprite> background;
	shared_ptr<UISprite> megaMan;

	shared_ptr<RectangleShape> rectangle;
	float fadeSpeed = 200;
	float transparency = 255;

	IntRect oldColour = IntRect(176, 0, 32, 62);
	IntRect newColour = IntRect(0, 0, 32, 62);

	State state;

	shared_ptr<Music> music;

	bool playMusic = false;


public:

	void init(shared_ptr<Weapon> weapon){
		symbol = shared_ptr<Text> (new Text());
		getEquip = shared_ptr<Text> (new Text());

		symbolText = ("-"+weapon->getSymbol()+"- ");
		getEquipText = ("Get Equipped \nwith \n" + weapon->getTextName());

		shared_ptr<Font> font = shared_ptr<Font>(new Font());
		font->loadFromFile("assets\\font.otf");
		symbol->setFont(*font);
		getEquip->setFont(*font);

		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\NES - Mega Man 2 - Miscellaneous - Menus.png");

		background = shared_ptr<UISprite>(new UISprite("BackGround", t, IntRect(1, 552, 496, 304), Vector2f(0, 0), Vector2f(4, 4)));

		shared_ptr<Texture> mega = shared_ptr<Texture> (new Texture());
		mega->loadFromFile("assets\\weapon get\\" + weapon->getName()+".png");
		megaMan = shared_ptr<UISprite>(new UISprite("mega", mega, oldColour, Vector2f(600, 200), Vector2f(4, 4)));

		symbol->setPosition(Vector2f(766, 200));
		symbol->setCharacterSize(34);
		getEquip->setPosition(800, 260);
		getEquip->setCharacterSize(34);
		getEquip->setLineSpacing(2);
		
		rectangle = shared_ptr<RectangleShape>(new RectangleShape());
		rectangle->setFillColor(Color(0,0,0, 255));
		rectangle->setPosition(megaMan->getCameraPosition());
		rectangle->setSize(megaMan->getSize());

		state = Fading;





	}

	EquipAnim(shared_ptr<Weapon> weapon, bool musicOn) {
		init(weapon);
		playMusic = musicOn;
		if (musicOn) {
			
			music = shared_ptr<Music>(new Music());
			music->openFromFile("assets\\sound\\music\\16 - Get Your Weapons Ready.wav");
			music->setLoop(true);
			music->setLoopPoints({ seconds(0), seconds(2.6720) });
		}
	}

	bool fadeLoop(float* deltaT) {
		transparency = transparency - (fadeSpeed * *deltaT);
		

		if (transparency <= 0) {
			transparency = 0;
			rectangle->setFillColor(Color(0,0,0, transparency));
			return true;
		}
		rectangle->setFillColor(Color(0,0,0, transparency));
		return false;
	}

	bool symbolNext() {
		charaNum++;
		string temp;
		for (int i = 0; i < charaNum; i++) {
			temp = temp + symbolText[i];
		}

		symbol->setString(temp);

		return (charaNum == symbolText.size());

	}

	bool textNext() {
		charaNum++;
		string temp;
		for (int i = 0; i < charaNum; i++) {
			temp = temp + getEquipText[i];
		}

		getEquip->setString(temp);

		return (charaNum == getEquipText.size());

	}
	

	bool textLoop(float* deltaT) {
		textTime_left -= *deltaT;

		if (textTime_left <= 0) {
			textTime_left = textTime;
			if (!symbolDone) {
				symbolDone = symbolNext();
				if (symbolDone) {
					charaNum = 0;
				}
			}
			else {
				if (textNext()) {
					return true;
				}
			}
		}
		return false;
	}

	

	bool flashLoop(float* deltaT) {
		flashTime_left -= *deltaT;
		flashFinishTime -= *deltaT;
		if (flashTime_left <= 0) {
			flashTime_left = flashTime;
			flash();
		}

		return flashFinishTime <= 0;
	}

	void flash() {
		if (megaMan->getRect() == oldColour) {
			megaMan->setRect(newColour);
		}
		else {
			megaMan->setRect(oldColour);
		}
	}

	shared_ptr<Music> getMusic() {
		return music;
	}


	void loop(shared_ptr<renderer> instance, float targetRate) {
		shared_ptr<timer> time = shared_ptr<timer>(new timer());


		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		bool run = true;

		if (playMusic) {
			music->play();
		}

		while (instance->getWindow()->isOpen() && run) {
			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					instance->getWindow()->close();
					run = false;
				}
			}
			time->frameLimiter(targetRate, startP);
			deltaT = time->checkTimer(startP);
			start = time->timerStart();
			startP = &start;


			if (state == Fading) {
				if (fadeLoop(&deltaT)) {
					state = Texting;
				}
			}

			else if (state == Texting) {
				if (textLoop(&deltaT)) {
					state = Flashing;
				}
			}

			else if (state == Flashing) {
				if (flashLoop(&deltaT)) {
					state = Done;
				}
			}

			else if (state = Done) {
				megaMan->setRect(newColour);
			
				run = false;
				
			}

			instance->UIDisplay(background);
			instance->UIDisplay(megaMan);

			instance->textDisplay(symbol);
			instance->textDisplay(getEquip);
			instance->rectDisplay(rectangle);
			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
	}

	list<shared_ptr<UISprite>> getSprites() {
		list<shared_ptr<UISprite>> sprites = { background, megaMan };
		return sprites;
	}
	list<shared_ptr<Text>> getText() {
		list<shared_ptr<Text>> texts = { symbol, getEquip };
		return texts;
	}

	shared_ptr<Texture> getTexture() {
		return background->getTexture();
	}
};