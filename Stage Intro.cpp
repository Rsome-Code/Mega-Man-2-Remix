#include "UI Sprite.cpp"
#include "Object Sprite.cpp"
#include "movable object.cpp"
#include "render logic.cpp"
#include "time.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#include <random>
#include "physics object.cpp"
#include "wood man.cpp"
#include "heat man.cpp"
#include "bubble man.cpp"
#include "metal man.cpp"
#include "quick man.cpp"
#include "flash man.cpp"
#include "crash man.cpp"
#include "air man.cpp"
#include <SFML/audio.hpp>
#pragma once

class StageIntro{
	bool run;
	shared_ptr<text> Text;
	list<shared_ptr<movable>> particles;
	shared_ptr<UISprite> background;
	string textBuffer;
	bool birds;
	shared_ptr<timer> time;
	shared_ptr<camera> cam;
	shared_ptr<animation> fadeIn;
	shared_ptr<animTimer> aTimer;
	int fadeNum = 0;
	list<shared_ptr<movable>>::iterator pIt;
	bool flap = false;
	float flapTime = 0.05;
	float flapTime_left = flapTime;
	shared_ptr<Master> boss = NULL;

	shared_ptr<objectHitbox> floor;

	bool textStart = false;
	float textTime = 0.2;
	float textTime_left = textTime;

	int charaNum = 0;
	bool textFin = false;

	float timer = 6.6;
	float timer_left = timer;

	shared_ptr<Music> music;

	sf::RectangleShape rectangle;
	float currentTrans = 255;
	float fadeRate = 100;

	shared_ptr<objectSprite> floorContainer;

public:
	StageIntro(string name, bool aHold, shared_ptr<Texture> bg, shared_ptr<Texture> bossT) {
		birds = aHold;
		textBuffer = name;
		transform(textBuffer.begin(), textBuffer.end(), textBuffer.begin(),::toupper);

		background = shared_ptr<UISprite>(new UISprite("bg", bg, IntRect(0, 359, 700, 293), Vector2f(0, 0), Vector2f(4, 4)));
		birds = aHold;
		if (!aHold) {
			dotSetup(bg, IntRect(272, 283, 4, 4));
		}
		else {
			dotSetup(bg, IntRect(289, 281, 8, 8));
		}
		cam = shared_ptr<camera>(new camera());

		pIt = particles.begin();
		shared_ptr<UISprite> start = *pIt;
		if (!birds) {
			fadeIn = shared_ptr<animation>(new animation(list<IntRect> {IntRect(272, 283, 4, 4), IntRect(272, 292, 4, 4), IntRect(272, 301, 4, 4)}, start));
		}
		else{
			fadeIn = shared_ptr<animation>(new animation(list<IntRect> {IntRect(280, 281, 8, 8), IntRect(280, 290, 8, 8), IntRect(280, 299, 8, 8)}, start));
		}
		aTimer = shared_ptr<animTimer> (new animTimer(fadeIn, 10, false));

		bossSetup(name, bossT);

		floorContainer = shared_ptr<objectSprite>(new objectSprite());
		floorContainer->setPosition(Vector2f(0, 0));

		floor = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 430, 1, 1), floorContainer));

		Text = shared_ptr<text> (new text());
		shared_ptr<Font> font = shared_ptr<Font>(new Font());
		font->loadFromFile("assets\\font.otf");
		Text->setFont(font);
		Text->setPosition(Vector2f(850, 570));

		music = shared_ptr<Music>(new Music());
		music->openFromFile("assets\\sound\\music\\5 - Enemy Chosen.mp3");
		music->setVolume(50);

		rectangle.setFillColor(Colour::Black().getColour());
		rectangle.setPosition(0, 0);
		rectangle.setSize(Vector2f(1920, 1080));
		
	}

	void bossSetup(string name, shared_ptr<Texture> bossT) {
		Vector2f pos = Vector2f(900, 0);
		if (name == "wood man") {
			boss = shared_ptr<Master>(new WoodMan(bossT, Vector2f(900, 0)));
			boss->initial();
		}
		else if (name == "heat man") {
			boss = shared_ptr<Master>(new HeatMan(bossT, Vector2f(900, 0)));
			boss->initial();
		}
		else if (name == "bubble man") {
			boss = shared_ptr<Master>(new BubbleMan(bossT, Vector2f(900, 0)));
			boss->initial();
		}
		else if (name == "metal man") {
			boss = shared_ptr<Master>(new MetalMan(bossT, Vector2f(900, 0)));
			boss->initial();
		}
		else if (name == "quick man") {
			boss = shared_ptr<Master>(new QuickMan(bossT, pos));
			boss->initial();
		}
		else if (name == "flash man") {
			boss = shared_ptr<Master>(new FlashMan(bossT, pos));
			boss->initial();
		}
		else if (name == "crash man") {
			boss = shared_ptr<Master>(new CrashMan(bossT, pos));
			boss->initial();
		}
		else if (name == "air man") {
			boss = shared_ptr<Master>(new AirMan(bossT, pos));
			boss->initial();
		}
	}

	void dotSetup(shared_ptr<Texture> bg, IntRect startFrame) {
		for (int i = 0; i < 25; i++) {
			float tempZ = (rand() % 100);
			tempZ = (tempZ / 100) + 1;
			//tempZ = 2;

			int tempX = rand();
			tempX = tempX % (1920 + mapLoc(tempZ, 1920));
			tempX = tempX - (mapLoc(tempZ, 1920));

			//int mapped = mapLoc(tempZ, 312);

			float tempY = (rand() % (312 + mapLoc(tempZ, 312)));

			tempY = tempY - (mapLoc(tempZ, 312) * 2);

			shared_ptr<movable> temp = shared_ptr<movable>(new movable("dot", bg, startFrame, Vector2f(tempX, tempY), Vector2f(4, 4), tempZ));
			particles.push_back(temp);
		}

		for (int i = 0; i < 30; i++) {
			float tempZ = (rand() % 100);
			tempZ = (tempZ / 100) + 1;
			//tempZ = 2;

			int tempX = rand();
			tempX = tempX % (1920 + mapLoc(tempZ, 1920));
			tempX = tempX - (mapLoc(tempZ, 1920));

			//int mapped = mapLoc(tempZ, 112*4);

			float tempY = (rand() % (312 + mapLoc(tempZ, (112 * 4))));

			tempY = tempY - (mapLoc(tempZ, (112 * 4) * 2));
			tempY = tempY + ((148 * 4) * (tempZ * 1.3));

			shared_ptr<movable> temp = shared_ptr<movable>(new movable("dot", bg, startFrame.getPosition(), startFrame.getSize(), Vector2f(tempX, tempY), Vector2f(4, 4), tempZ));
			particles.push_back(temp);
		}
	}

	int mapLoc(float tempZ, float norm) {
		float total = norm * tempZ;
		float diff = total - norm;
		//diff = diff / 2;
		return int(diff);
	}

	void moveParticles(float* deltaT, shared_ptr<renderer> instance, shared_ptr<camera> cam) {
		for (shared_ptr<movable> ob : particles) {
			if (ob->getCameraPosition().x > 1920) {
				ob->setPosition(Vector2f(0, ob->getPosition().y));
				while (ob->getCameraPosition().x > 0) {
					instance->bObjectCalc(ob, cam);
					ob->move(180, 50);
				}
			}
			ob->move(0, deltaT, 800);
			if (birds) {
				animateBirds();
			}
		}
	}

	void animateBirds() {
		bool thisFlap = flap;
		for (shared_ptr<UISprite> bird : particles) {
			if (thisFlap) {
				bird->setRect(IntRect(280, bird->getRect().getPosition().y, bird->getRect().getSize().x, bird->getRect().getSize().y));
			}
			else {
				bird->setRect(IntRect(289, bird->getRect().getPosition().y, bird->getRect().getSize().x, bird->getRect().getSize().y));
			}
			thisFlap = !thisFlap;
		}
	}

	void bossAnim(float deltaT) {
		textStart = boss->titleLoop(&deltaT, floor);
	}

	void textAnim(float deltaT) {
		textTime_left -= deltaT;
		if (textTime_left <= 0) {
			textTime_left = textTime;
			charaNum++;
			string temp;
			for (int i = 0; i < charaNum; i++) {
				temp = temp + textBuffer[i];
			}
			
			Text->setString(temp);

			textFin = (charaNum == textBuffer.size());
			
		}
	}

	//Copied animation for multiple sprites
	void fadeInAnim(float* deltaT){
		bool fin = false;
		shared_ptr<UISprite> temp = *particles.begin();
		fadeIn->setSprite(temp);
		fin = aTimer->run(deltaT);
		if (fin) {
			for (shared_ptr<UISprite> ob : particles) {
				fadeIn->setSprite(ob);
				fadeIn->setFrame();
			}
			//cout << "f";
		}
		if (fin) {
			fadeNum++;
		}
	}

	void timerRun(float deltaT){
		timer_left -= deltaT;
		if (timer_left <= 0) {
			run = false;
		}
	}

	void fadeInRect(float deltaT) {
		
		currentTrans -= fadeRate * deltaT;
		if (currentTrans > 0) {
			rectangle.setFillColor(Colour(0, 0, 0, currentTrans).getColour());
		}
	}

	void loop(shared_ptr<renderer> instance, float targetRate) {
		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		run = true;

		if (instance->getWindow()->isOpen()) {
			music->play();
		}

		boss->tauntStart();
		
		while (instance->getWindow()->isOpen() && run) {
			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();
			}
			if (time->frameLimiter(targetRate, startP)) {
				deltaT = 0.0333;
			}
			else {
				deltaT = time->checkTimer(startP);
			}

			start = time->timerStart();
			startP = &start;

			moveParticles(&deltaT, instance, cam);

			if (!textStart) {
				bossAnim(deltaT);
			}
			else if (!textFin) {
				textAnim(deltaT);
			}

			timerRun(deltaT);

			if (birds) {
				
				if (deltaT >= flapTime_left) {
					flap = !flap;
					animateBirds();
					flapTime_left = flapTime;
				}
				else {
					flapTime_left -= deltaT;
				}
			}

			if (!(fadeNum >= 2)) {
				fadeInAnim(&deltaT);
			}

			
			list<shared_ptr<objectSprite>> obs;
			for (shared_ptr<objectSprite> ob : particles) {
				obs.push_back(ob);
			}
			instance->bObjectDisplay(obs, cam);

			fadeInRect(deltaT);

			instance->getWindow()->draw(rectangle);

			instance->UIDisplay(background);
			instance->objectDisplay(boss->getSprite(), cam);
			instance->textDisplay(Text);

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
		
	}
};