#include "Tile.cpp"
#include "UI Sprite.cpp"
#include "Object Sprite.cpp"
#include "Time.cpp"
#include "Render Logic.cpp"
#include "Menu Selection.cpp"
#include "Object Select.cpp"
#include "Right tile.cpp"
#include "Left tile.cpp"
#include "Ceiling tile.cpp"
#include "Top Tile.cpp"
#include "Solid Tile.cpp"
#include "ladder tile.cpp"
#include "top ladder tile.cpp"
#include "Mouse.cpp"
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "Load.cpp"
#include "Object Tab.cpp"

#pragma once

class ObjectPlacer {
	list<tile*> tileList;
	list<tile*> z2List;
	list<tile*> z3List;
	list<tile*> z4List;
	list<object*> objects;
	list<object*>::iterator objectIt;
	Texture* tex;
	string levelName;
	string saveFile;
	bool run;

	int z = 1;

	int section = 0;

	timer* time;
	camera* cam;
	Vector2i mousePos;
	Vector2i dragStart;
	bool mouse1Pressed;
	bool mouse2Pressed;
	bool mouseX1Pressed;
	bool leftPressed = false;
	bool rightPressed = false;
	Vector2f wSize = Vector2f(1920, 1080);
	bool worldSelect = false;
	
	Tab* tab;
	object* selectedObject = NULL;
	object* selectedPlaced = NULL;
	Texture* enemyT;

public:
	ObjectPlacer(Texture* T, string levelN, list<object*> obList) {
		this->levelName = levelN;
		saveFile = levelN;
		tex = T;
		Load* l = new Load();
		l->load(levelName, tex, &tileList, &z2List, &z3List, &z4List);

		changeZ();
		cam = new camera();
		cam->setZoom(0.5);

		enemyT = new Texture();
		enemyT->loadFromFile("Assets\\enemy.png");
		
		for (object* o : obList) {
			o->setCode();
		}
		tab = new Tab(obList, Vector2f(1920 - 414, 0));

		l->loadObjects(saveFile, &objects, enemyT);
		
	}
	
	void loop(renderer* instance, double targetRate) {
		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;
		run = true;
		mouse* m = new mouse();
		while (instance->getWindow()->isOpen() && run) {
			Event event;



			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();

				if (event.type == Event::Resized) {
					wSize = Vector2f(event.size.width, event.size.height);
				}
			}

			time->frameLimiter(targetRate, startP);
			deltaT = time->checkTimer(startP);
			start = time->timerStart();
			startP = &start;



			for (tile* t : tileList) {
				instance->objectSetup(t->getSprite(), cam);
			}
			for (tile* t : z2List) {
				instance->objectSetup(t->getSprite(), cam);
			}
			for (tile* t : z3List) {
				instance->objectSetup(t->getSprite(), cam);
			}
			if (z != 4) {
				for (tile* t : z4List) {
					instance->bObjectDisplay(t->getSprite(), cam);
				}
			}
			else {
				for (tile* t : z4List) {
					instance->objectAccess(t, cam);
				}
			}

			if (z <= 3) {
				if (z != 3) {
					for (tile* t : z3List) {
						instance->bObjectDisplay(t->getSprite(), cam);
					}
				}
				else {
					for (tile* t : z3List) {
						instance->objectAccess(t, cam);
					}
				}


			}

			if (z <= 2) {
				if (z != 2) {
					for (tile* t : z2List) {
						instance->bObjectDisplay(t->getSprite(), cam);
					}
				}
				else {
					for (tile* t : z2List) {
						instance->objectAccess(t, cam);
					}
				}

			}

			if (z == 1) {
				for (tile* t : tileList) {
					instance->objectAccess(t, cam);
				}
			}

			keyBoardCheck();

			Vector2i mousePos = m->getPosition(instance, wSize);
			if (checkActive(mousePos)) {
				mouseCheck(mousePos);
			}

			for (object* o : objects) {
				instance->objectAccess(o, cam);
			}

			instance->UIDisplay(tab->getSprites());



			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
	}

	void keyBoardCheck() {
		if (Keyboard::isKeyPressed(Keyboard::Scan::Right) && !rightPressed) {
			section++;
			reload();
			rightPressed = true;
		}
		else if (!Keyboard::isKeyPressed(Keyboard::Scan::Right)) {
			rightPressed = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Scan::Left) && !leftPressed) {
			section--;
			reload();
			leftPressed = true;
		}
		else if (!Keyboard::isKeyPressed(Keyboard::Scan::Left)) {
			leftPressed = false;
		}
	}

	void reload() {
		tileList.clear();
		z2List.clear();
		z3List.clear();
		z4List.clear();
		objects.clear();
		Load* load = new Load();
		if (section != 0) {
			load->load(levelName + to_string(section), tex, &tileList, &z2List, &z3List, &z4List);
		}
		else {
			load->load(levelName, tex, &tileList, &z2List, &z3List, &z4List);
		}

		if (section != 0) {
			load->loadObjects(levelName + to_string(section), &objects, enemyT);
		}
		else {
			load->loadObjects(levelName, &objects, enemyT);
		}

		changeZ();

	}

	bool checkActive(Vector2i mousePos) {
		if (mousePos.x < 0 || mousePos.x > 1920 || mousePos.y < 0 || mousePos.y > 1080) {
			return false;
		}
					
		return true;
		
	}

	void worldClick(Vector2i mousePos) {
		Vector2f worldPos = Vector2f(mousePos.x + cam->getPosition().x, mousePos.y + cam->getPosition().y);
		selectedPlaced = NULL;
		selectedPlaced = checkPlaced(worldPos);
		if (selectedObject != NULL && selectedPlaced == NULL) {
			//object temp = *selectedObject;
			object* tempLoc = new object(selectedObject);
			//tempLoc = selectedObject;
			tempLoc->getSprite()->setPosition(worldPos);
			tempLoc->setDisplay(true);
			objects.push_back(tempLoc);
			objectIt = prev(objects.end());
			selectedPlaced = *objectIt;
		}

	}

	object* checkPlaced(Vector2f mousePos) {
		if (!objects.empty()) {
			objectIt = objects.begin();
			for (object* ob : objects) {
				objectSprite* temp = ob->getSprite();
				if (mousePos.x > temp->getPosition().x && mousePos.x < temp->getPosition().x + temp->getSize().x) {
					if (mousePos.y > temp->getPosition().y && mousePos.y < temp->getPosition().y + temp->getSize().y) {
						return ob;
					}
				}
				objectIt = next(objectIt);
			}
			
		}
		return NULL;
	}

	Vector2i mouseCorrection(Vector2i mousePos) {
		Vector2i temp = Vector2i(mousePos.x / cam->getZoom(), mousePos.y / cam->getZoom());
		return temp;
	}

	void objectDrag(Vector2i mousePos) {
		if (selectedPlaced != NULL) {
			Vector2f worldPos = Vector2f(mousePos.x + cam->getPosition().x, mousePos.y + cam->getPosition().y);
			{
				selectedPlaced->getSprite()->setPosition(worldPos);
			}
		}
	}

	void mouseCheck(Vector2i mousePos) {
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse1Pressed != true) //specifies
		{
			mouse1Pressed = true;
			if (tab->checkActive(mousePos)) {
				tab->checkMouse(mousePos);
				selectedObject = tab->getSelected();
				worldSelect = false;
			}
			else {
				worldSelect = true;
				mousePos = mouseCorrection(mousePos);
				worldClick(mousePos);
			}
			mouse1Pressed = true;
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			//drag(mousePos);
			if (worldSelect) {

				mousePos = mouseCorrection(mousePos);
				objectDrag(mousePos);
			}

		}
		else {
			mouse1Pressed = false;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::XButton1) && mouseX1Pressed != true) //specifies
		{
			if (objectIt != objects.end()) {
				objects.erase(objectIt);
				objectIt = objects.end();
			}
		}

		else {
			mouseX1Pressed = false;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mouse2Pressed != true) //specifies
		{
			mouse2Pressed = true;
			dragStart = mousePos;
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

			drag(mousePos);

		}
		else {
			mouse2Pressed = false;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
		{
			save();
		}
	}

	void save() {
		ofstream* myfile;
		myfile = new ofstream();
		if (section == 0) {
			myfile->open(saveFile + "-objects.txt");
		}
		else {
			myfile->open(saveFile + to_string(section) + "-objects.txt");
		}

		for (object* o : objects) {
			*myfile << o->getCode() + ",";
			*myfile << o->getSprite()->getPosition().x;
			*myfile << ",";
			*myfile << o->getSprite()->getPosition().y;
			*myfile << ",";
			*myfile << o->getSprite()->getZ();


			*myfile << "\n";
		}
		myfile->close();
	}

	void drag(Vector2i mouse) {
		Vector2f currentPos = cam->getPosition();
		cam->setPosition(Vector2f(currentPos.x - (dragStart.x - mouse.x) / 2, currentPos.y - (dragStart.y - mouse.y) / 2));
	}


	void changeZ() {

		for (tile* t : z2List) {
			if (z == 1) {
				t->getSprite()->setZ(1.25);
			}
			else {
				t->getSprite()->setZ(1);
			}
		}
		for (tile* t : z3List) {
			if (z == 1) {
				t->getSprite()->setZ(1.5);
			}
			else if (z == 2) {
				t->getSprite()->setZ(1.25);
			}
			else {
				t->getSprite()->setZ(1);
			}
		}
		for (tile* t : z4List) {
			if (z == 1) {
				t->getSprite()->setZ(1.75);
			}
			else if (z == 2) {
				t->getSprite()->setZ(1.5);
			}
			else if (z == 3) {
				t->getSprite()->setZ(1.75);
			}
			else {
				t->getSprite()->setZ(1);
			}
		}
	}

};