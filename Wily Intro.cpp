#include "animation timer.cpp"
#include "sound collection.cpp"
#include "physics object.cpp"
#include "camera.cpp"
#include "time.cpp"
#include "render logic.cpp"
#include "angles.cpp"
#include "movable ui.cpp"
#pragma once

class WilyIntro {
	
	shared_ptr<physicsObject> wily;
	shared_ptr<movable> lid;
	shared_ptr<objectSprite> wBottom;
	shared_ptr<animation> bAnim;
	shared_ptr<animTimer> bTimer;
	shared_ptr<animation> eyeAnim;
	shared_ptr<animTimer> eyeTimer;

	shared_ptr<movableUI> background;
	shared_ptr<camera> cam;

	vector<shared_ptr<UISprite>>* thisSpriteList;

	vector<shared_ptr<UISprite>> pathSpriteList1;
	vector<shared_ptr<UISprite>> pathSpriteList2;
	vector<shared_ptr<UISprite>> pathSpriteList3;
	vector<shared_ptr<UISprite>> pathSpriteList4;
	vector<shared_ptr<UISprite>> pathSpriteList5;
	vector<shared_ptr<UISprite>> pathSpriteList6;
	vector<shared_ptr<UISprite>> markSpriteList;

	vector<shared_ptr<UISprite>> marks;

	IntRect downRight = IntRect(2, 588, 6, 6);
	IntRect upRight = IntRect(11, 588, 6, 6);
	IntRect downLeft = IntRect(20, 588, 6, 6);
	IntRect upLeft = IntRect(29, 588, 6, 6);
	IntRect horizontal = IntRect(38, 588, 8, 4);
	IntRect vertical = IntRect(49, 586, 4, 8);

	enum Direction {
		up, down, left, right, mark
	};

	vector<Direction> thisPath;
	vector<Direction> pathList1;
	vector<Direction> pathList2;
	vector<Direction> pathList3;
	vector<Direction> pathList4;
	vector<Direction> pathList5;
	vector<Direction> pathList6;


	Vector2f currentPos;

	int level;

	enum State {
		taunt, flash, mapHold, map, endHold
	};

	State state;

	shared_ptr<Sound> pathSound;
	shared_ptr<Sound> wilySound;
	shared_ptr<SoundBuffer> wilySoundB;

	shared_ptr<Music> music;

	int backOffset = 144 * 4;

	shared_ptr<RectangleShape> rect;

	float rectTransparency = 255;
	float transSpeed = 180;

public:

	Vector2f backPos() {
		return Vector2f((1920 / 2) - (background->getSize().x / 2), 1080 - background->getSize().y);
	}

	WilyIntro(int level, shared_ptr<SoundCollection> soundCol) {
		shared_ptr<Texture> backTex = shared_ptr<Texture>(new Texture());
		backTex->loadFromFile("assets\\wily castle.png");
		background = shared_ptr<movableUI>(new movableUI(backTex, IntRect(550, 21, 568, 512), Vector2f(400, 0), Vector2f(4, 4)));
		

		rect = shared_ptr<RectangleShape>(new RectangleShape(Vector2f(2000, 2000)));
		
		//rect->setScale(Vector2f(2000, 2000));
		rect->setPosition(Vector2f(0, 0));
		rect->setFillColor(Color::Black);
		
		if (level == 1) {
			shared_ptr<Texture> wilyTex = shared_ptr<Texture>(new Texture());
			wilyTex->loadFromFile("Assets\\Dr Wily.png");

			wily = shared_ptr<physicsObject>(new physicsObject(wilyTex, IntRect(347, 59, 46, 32), Vector2f(-347, 450), Vector2f(4, 4)));
			lid = shared_ptr<movable>(new movable(wilyTex, IntRect(346, 15, 48, 20), Vector2f(0, 0), Vector2f(4, 4)));
			cam = shared_ptr<camera>(new camera());
			state = taunt;
			wily->enableGravity(false);
			wily->setMaxSpeed(800);
			eyeAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(347, 59, 46, 32), IntRect(500, 59, 46, 32)}, wily));
			eyeTimer = shared_ptr<animTimer>(new animTimer(eyeAnim, 4, true));
			rightPos = (1920 / 2) - (wily->getSize().x/2);

			background->setCameraPosition(Vector2f((1920 / 2) - (background->getSize().x / 2), 0));

			wilySoundB = shared_ptr<SoundBuffer>(new SoundBuffer());
			wilySoundB->loadFromFile("assets\\sound\\ufo.wav");
			wilySound = shared_ptr<Sound>(new Sound());
			wilySound->setBuffer(*wilySoundB);
			wilySound->setLoop(true);
			wilySound->play();


			wBottom = shared_ptr<objectSprite>(new objectSprite(wilyTex, IntRect(347, 75, 46, 16), Vector2f(0, 0), Vector2f(4, 4)));
			bAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(347, 75, 46, 16), IntRect(398, 75, 46, 16), IntRect(449, 75, 46, 16)}, wBottom));
			bTimer = shared_ptr<animTimer>(new animTimer(bAnim, 8, true));
		}
		else {
			background->setCameraPosition(backPos());
			state = flash;
		}

		this->level = level;
	
		pathSound = soundCol->getHeal();


		markSetup();
		shared_ptr<UISprite> temp = marks[0];

		currentPos = temp->getCameraPosition();
		path1Setup(backTex);
		
		thisPath = pathList1;
		thisSpriteList = &pathSpriteList1;
		
		if (level > 1) {
			shared_ptr<UISprite> temp = marks[0];

			currentPos = temp->getCameraPosition();
			while (!mapLoop(10, pathList1, &pathSpriteList1));
			path2Setup(backTex);
			thisPath = pathList2;
			thisSpriteList = &pathSpriteList2;
			i = 0;
		}

		if (level > 2) {
			
			shared_ptr<UISprite> temp = marks[1];

			currentPos = temp->getCameraPosition();
			while (!mapLoop(10, pathList2, &pathSpriteList2));
			path3Setup(backTex);
			thisPath = pathList3;
			thisSpriteList = &pathSpriteList3;
			i = 0;
		}

		if (level > 3) {

			shared_ptr<UISprite> temp = marks[2];

			currentPos = temp->getCameraPosition();
			while (!mapLoop(10, pathList3, &pathSpriteList3));
			path4Setup(backTex);
			thisPath = pathList4;
			thisSpriteList = &pathSpriteList4;
			i = 0;
		}

		if (level > 4) {

			shared_ptr<UISprite> temp = marks[3];

			currentPos = temp->getCameraPosition();
			while (!mapLoop(10, pathList4, &pathSpriteList4));
			path5Setup(backTex);
			thisPath = pathList5;
			thisSpriteList = &pathSpriteList5;
			i = 0;
		}

		if (level > 5) {

			shared_ptr<UISprite> temp = marks[4];

			currentPos = temp->getCameraPosition();
			while (!mapLoop(10, pathList5, &pathSpriteList5));
			path6Setup(backTex);
			thisPath = pathList6;
			thisSpriteList = &pathSpriteList6;
			i = 0;

			temp = marks[level - 1];

			currentPos = Vector2f(temp->getCameraPosition().x + 16, temp->getCameraPosition().y + 16);
		}

		else {
			shared_ptr<UISprite> temp = marks[level - 1];

			currentPos = temp->getCameraPosition();
		}


		music = shared_ptr<Music>(new Music());
		music->openFromFile("assets\\sound\\music\\17 - Dr. Wily's Map.mp3");


	}

	void markSetup() {
		marks.push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), IntRect(56, 586, 8, 8), Vector2f((16*4) + background->getCameraPosition().x + backOffset, 1080 - (63 * 4)), Vector2f(4, 4))));

		marks.push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), IntRect(56, 586, 8, 8), Vector2f((64 * 4) + background->getCameraPosition().x + backOffset, 1080 - (119 * 4)), Vector2f(4, 4))));
		marks.push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), IntRect(56, 586, 8, 8), Vector2f((96 * 4) + background->getCameraPosition().x + backOffset, 1080 - (95 * 4)), Vector2f(4, 4))));
		marks.push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), IntRect(56, 586, 8, 8), Vector2f((136 * 4) + background->getCameraPosition().x + backOffset, 1080 - (87 * 4)), Vector2f(4, 4))));
		marks.push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), IntRect(56, 586, 8, 8), Vector2f((152 * 4) + background->getCameraPosition().x + backOffset, 1080 - (143 * 4)), Vector2f(4, 4))));

		marks.push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), IntRect(0, 569, 16, 16), Vector2f((180 * 4) + background->getCameraPosition().x + backOffset, 1080 - (115 * 4)), Vector2f(4, 4))));
		marks.push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), IntRect(0, 569, 16, 16), Vector2f((216 * 4) + background->getCameraPosition().x + backOffset, 1080 - (59 * 4)), Vector2f(4, 4))));
	}

	void path6Setup(shared_ptr<Texture> backTex) {

		
		pathList6.push_back(down); 
		pathList6.push_back(down); 
		pathList6.push_back(down); 
		pathList6.push_back(down); 
		pathList6.push_back(down); 
		pathList6.push_back(down); 
		pathList6.push_back(down);

		pathList6.push_back(right);
		pathList6.push_back(right);
		pathList6.push_back(right);

	}

	void path5Setup(shared_ptr<Texture> backTex) {

		pathList5.push_back(right);
		pathList5.push_back(right);

		pathList5.push_back(down);
		pathList5.push_back(down);
		pathList5.push_back(down);
		pathList5.push_back(down);

		pathList5.push_back(right);
	}

	void path4Setup(shared_ptr<Texture> backTex) {

		pathList4.push_back(right);
		
		pathList4.push_back(up);
		pathList4.push_back(up);
		pathList4.push_back(up);
		pathList4.push_back(up);
		pathList4.push_back(up);
		pathList4.push_back(right);
		pathList4.push_back(up);

	}

	void path3Setup(shared_ptr<Texture> backTex) {
		pathList3.push_back(right);
		pathList3.push_back(right);
		pathList3.push_back(down);
		pathList3.push_back(right);
		pathList3.push_back(right);
	}

	void path1Setup(shared_ptr<Texture> backTex) {

		//pathList1.push_back(mark);
		pathList1.push_back(right);
		pathList1.push_back(right);
		pathList1.push_back(up);
		pathList1.push_back(up);
		pathList1.push_back(up);
		pathList1.push_back(up);
		pathList1.push_back(up);
		pathList1.push_back(right);
		pathList1.push_back(right);
		pathList1.push_back(up);
		pathList1.push_back(up);
		pathList1.push_back(right);
		//pathList1.push_back(mark);
	}

	void path2Setup(shared_ptr<Texture> backTex) {
		
		pathList2.push_back(right);
		pathList2.push_back(down);
		pathList2.push_back(right);
		pathList2.push_back(down);
		pathList2.push_back(down);
		pathList2.push_back(right);
	}


	float mapTime = 0.1;
	float mapTime_left = mapTime;
	bool playSound = false;
	bool mapLoop(float deltaT, vector<Direction> pathList, vector<shared_ptr<UISprite>>* pathSpriteList) {
		mapTime_left -= deltaT;

		if (mapTime_left <= 0) {

			if (playSound) {
				pathSound->play();
			}
			mapTime_left = mapTime;

			checkSprite(pathList[i], pathList, pathSpriteList);
			i++;
		}
		return i >= pathList.size();
	}

	void addVertical(vector<shared_ptr<UISprite>>* pathSpriteList) {
		pathSpriteList->push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), vertical, Vector2f(currentPos.x + 8, currentPos.y), Vector2f(4, 4))));
	}
	void addHorizontal(vector<shared_ptr<UISprite>>* pathSpriteList) {
		pathSpriteList->push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), horizontal, Vector2f(currentPos.x, currentPos.y + 8), Vector2f(4, 4))));
	}
	void addDownRight(vector<shared_ptr<UISprite>>* pathSpriteList) {
		pathSpriteList->push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), downRight, Vector2f(currentPos.x + 8, currentPos.y + 8), Vector2f(4, 4))));

	}
	void addUpRight(vector<shared_ptr<UISprite>>* pathSpriteList) {
		pathSpriteList->push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), upRight, Vector2f(currentPos.x + 8, currentPos.y), Vector2f(4, 4))));
	}
	void addDownLeft(vector<shared_ptr<UISprite>>* pathSpriteList) {
		pathSpriteList->push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), downLeft, Vector2f(currentPos.x, currentPos.y + 8), Vector2f(4, 4))));
	}
	void addUpLeft(vector<shared_ptr<UISprite>>* pathSpriteList) {
		pathSpriteList->push_back(shared_ptr<UISprite>(new UISprite(background->getTexture(), upLeft, Vector2f(currentPos.x, currentPos.y), Vector2f(4, 4))));

	}

	void checkSprite(Direction dir, vector<Direction> pathList, vector<shared_ptr<UISprite>>* pathSpriteList) {

		if (dir == up) {
			currentPos.y -= 8 *4;
		}
		else if (dir == down) {
			currentPos.y += 8 *4;
		}
		else if (dir == left) {
			currentPos.x -= 8 *4;
		}
		else if (dir == right) {
			currentPos.x += 8 *4;
		}

		if (i != pathList.size()-1) {
			if (pathList[i + 1] == up) {
				
				if (dir == up) {
					addVertical(pathSpriteList);
				}
				else if (dir == right) {
					addUpLeft(pathSpriteList);
				}
				else if (dir == left) {
					addUpRight(pathSpriteList);
				}
			}
			else if (pathList[i + 1] == down) {
				
				if (dir == down) {
					addVertical(pathSpriteList);
				}
				else if (dir == right) {
					addDownLeft(pathSpriteList);
				}
				else if (dir == left) {
					addDownRight(pathSpriteList);
				}
			}
			else if (pathList[i + 1] == left) {
				
				if (dir == left) {
					addHorizontal(pathSpriteList);
				}
				else if (dir == up) {
					addDownRight(pathSpriteList);
				}
				else if (dir == down) {
					addUpRight(pathSpriteList);
				}
			}
			else if (pathList[i + 1] == right) {
				
				if (dir == right) {
					addHorizontal(pathSpriteList);
				}
				else if (dir == up) {
					addDownRight(pathSpriteList);
				}
				else if (dir == down) {
					addUpRight(pathSpriteList);
				}
			}
		}
		else {
			if (dir == right || dir == left) {
				addHorizontal(pathSpriteList);
			}
			else if (dir == up || dir == down) {
				addVertical(pathSpriteList);
			}
		}
	}

	
	float holdTime = 2;
	bool holdLoop(float deltaT) {
		holdTime -= deltaT;
		if (holdTime <= 0) {
			return true;
		}
		return false;
	}


	float flashTime = 0.1;
	float flashTime_left = 0;
	bool flashOn = false;
	void flashLoop(float deltaT) {
		flashTime_left -= deltaT;
		if (flashTime_left <= 0) {
			flashTime_left = flashTime;
			
			if (flashOn) {
				background->setRect(IntRect(550, 21, 568, 512));
				flashOn = !flashOn;
			}
			else {
				background->setRect(IntRect(Vector2i(1171, background->getRect().top), background->getRect().getSize()));
				flashOn = !flashOn;
			}
		}

	}

	float mapHoldTime = 1;
	bool mapHoldLoop(float deltaT) {
		mapHoldTime -= deltaT;
		if (mapHoldTime <= 0) {
			return true;
		}
		return false;
	}

	float markerFLashTime = 0.1;
	float markerFLashTime_left = 0.1;

	void flashMarkers(float deltaT) {

		markerFLashTime_left -= deltaT;

		if (markerFLashTime_left <= 0) {
			markerFLashTime_left = markerFLashTime;
			if (flashOn) {
				marks[5]->setRect(IntRect(IntRect(0, 569, 16, 16)));
				marks[6]->setRect(IntRect(IntRect(0, 569, 16, 16)));
				if (level - 1 < 5) {
					marks[level - 1]->setRect(IntRect(56, 586, 8, 8));
				}
			}
			else {
				marks[5]->setRect(IntRect(IntRect(17, 569, 16, 16)));
				marks[6]->setRect(IntRect(IntRect(17, 569, 16, 16)));

				if (level - 1 < 5) {
					marks[level - 1]->setRect(IntRect(65, 586, 8, 8));
				}
			}


			flashOn = !flashOn;
		}

	}


	void rectLoop(float deltaT) {
		rectTransparency -= transSpeed * deltaT;
		rect->setFillColor(Color(rect->getFillColor().r, rect->getFillColor().g, rect->getFillColor().b, rectTransparency));
		if (rectTransparency <= 0) {
			rect->setFillColor(Color(rect->getFillColor().r, rect->getFillColor().g, rect->getFillColor().b, 0));
		}
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum WilyState {
		moveRight, eyebrow, lidFall, moveBack
	};
	WilyState wState = moveRight;
	float rightSpeed = 300;
	float rightPos;

	Vector2f lidPos() {
		//lid->setPosition(Vector2f(wily->getPosition().x, wily->getPosition().y - (4 * 4)));
		return Vector2f(wily->getPosition().x - (1*4), wily->getPosition().y - (4 * 4));
	}

	Vector2f bottomPos() {
		return Vector2f(wily->getPosition().x, wily->getPosition().y + (16 * 4));
	}

	bool lidOn(float* deltaT) {
		lid->move(Angle::down, deltaT, lidSpeed);
		if (lid->getPosition().y >= lidPos().y) {
			return true;
		}
		return false;
	}

	int lidSpeed = 300;

	float eyeTime = 3;

	float zChangeSpeed = -0.5;

	void zChange(float deltaT) {
		wily->setZ(wily->getZ() - (zChangeSpeed * deltaT));
		lid->setZ(wily->getZ());
		if (wily->getZ() <= 0) {
			wily = NULL;
			lid = NULL;
		}
	}

	int diff = 800;
	int minPos = 1920 / 2 - (diff);
	int maxPos = (1920 / 2) + diff;
	bool wRight = true;
	int wilyAccel = 1600;

	void leftRight(float* deltaT) {
		if (wRight) {
			wily->addForce(Vector2f(wilyAccel, 0), deltaT);
			if (wily->getPosition().x > maxPos) {
				wRight = false;
			}
		}
		else {
			wily->addForce(Vector2f(-wilyAccel, 0), deltaT);
			if (wily->getPosition().x < minPos) {
				wRight = true;
			}
		}
		wily->eachFrame(deltaT);

		
	}

	int backgroundSpeed = 30;

	bool backgroundPan(float* deltaT) {
		background->move(Angle::up, deltaT, backgroundSpeed);
		if (background->getCameraPosition().y <= backPos().y) {
			background->setCameraPosition(backPos());
			return true;
		}
		return false;
	}

	bool tauntLoop(float deltaT) {
		
		if (wState == moveRight) {
			wily->move(Angle::right, &deltaT, rightSpeed);
			lid->setPosition(lidPos());
			if (wily->getPosition().x >= rightPos) {
				wily->setPosition(Vector2f(rightPos, wily->getPosition().y));
				wState = eyebrow;

			}
		}
		else if (wState == eyebrow) {
			if (lid->getPosition().y > wily->getPosition().y - (44 * 4)) {
				lid->move(Angle::up, &deltaT, lidSpeed);
				if (lid->getPosition().y <= wily->getPosition().y - (44 * 4)) {
					lid->setPosition(Vector2f(lid->getPosition().x, wily->getPosition().y - (44 * 4)));
					wState = lidFall;
				}
			}
		}
		else if (wState == lidFall) {
				
			eyeTimer->run(&deltaT);
			eyeTime -= deltaT;
			if (eyeTime <= 0) {
				if (lidOn(&deltaT)) {
					
					lid->setPosition(lidPos());
					wState = moveBack;
					music->play();
				}
			}
		}
		

		else if (wState == moveBack) {
			zChange(deltaT);

			leftRight(&deltaT);

				

			lid->setPosition(lidPos());
			return backgroundPan(&deltaT);
		}
		

		return false;

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int i = 0;
	float musicTime = 6.4;

	void loop(shared_ptr<renderer> instance, float targetRate) {

		

		shared_ptr<timer> time = shared_ptr<timer>(new timer());


		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		bool run = true;

		i = 0;

		if (level > 1) {
			music->play();
		}
		music->setVolume(60);

		playSound = true;
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


			


			if (music->getStatus() == Music::Playing) {
				musicTime -= deltaT;
			}

			if (state == taunt) {
				bTimer->run(&deltaT);
				//state = map;
				rectTransparency = 0;
				rect->setFillColor(Color::Transparent);
				
				if (tauntLoop(deltaT)) {
					state = flash;
					wilySound->stop();
				}
				wBottom->setPosition(bottomPos());
				wBottom->setZ(wily->getZ());
			}

			else if (state == flash) {

				rectLoop(deltaT);

				if (musicTime <= 0) {
					flashLoop(deltaT);
					if (music->getStatus() == Music::Stopped) {
						state = mapHold;
						background->setRect(IntRect(Vector2i(1171, background->getRect().top), background->getRect().getSize()));
					}
				}
			}

			else if (state == mapHold) {
				flashMarkers(deltaT);
				if (mapHoldLoop(deltaT)) {
					state = map;
				}
			}

			else if (state == map) {
				flashMarkers(deltaT);
				mapLoop(deltaT, thisPath, thisSpriteList);
				if (i >= thisPath.size()) {
					state = endHold;
				}
			}

			else if (state == endHold) {
				flashMarkers(deltaT);
				if (holdLoop(deltaT)) {
					run = false;
				}
			}


			instance->getWindow()->clear(sf::Color(0, 112, 236));

			instance->UIDisplay(background);
			
			if (state == map || state == mapHold || state == endHold) {
				instance->UIDisplay(pathSpriteList1);

				if (level > 1) {
					instance->UIDisplay(pathSpriteList2);
				}
				if (level > 2) {
					instance->UIDisplay(pathSpriteList3);
				}
				if (level > 3) {
					instance->UIDisplay(pathSpriteList4);
				}
				if (level > 4) {
					instance->UIDisplay(pathSpriteList5);
				}
				if (level > 5) {
					instance->UIDisplay(pathSpriteList6);
				}

				

				instance->UIDisplay(marks);
			}

			if (state == taunt) {
				if (wily != NULL) {
					instance->bObjectDisplay(wily, cam);
					instance->bObjectDisplay(lid, cam);
					instance->bObjectDisplay(wBottom, cam);
					
				}
			}

			instance->rectDisplay(rect);
			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
	}

};