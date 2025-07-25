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
#include "door.cpp"
#include "Mouse.cpp"
#include <SFML/Window/Mouse.hpp>
#include "object.cpp"
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

	list<EndFlag*> flagList;

	Door* door1;
	Door* door2;
	Load* l;

	bool checkpoint = false;
	bool cPressed = false;


public:
	ObjectPlacer(Texture* T, string levelN, list<object*> obList) {
		this->levelName = levelN;
		saveFile = levelN;
		tex = T;
		l = new Load();
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
		l->loadFlags(levelName, &flagList, enemyT);
		
		
		for (object* ob : flagList) {
			if (ob->getSection() == section) {
				objects.push_back(ob);
			}
		}
		

		door1 = l->getDoor1();
		door2 = l->getDoor2();
		
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

			for (object* o : flagList) {
				//instance->objectAccess(o, cam);
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

		for (object* ob : flagList) {
			if (ob->getSection() == section) {
				objects.push_back(ob);
			}
		}

		changeZ();

		door1 = l->getDoor1();
		door2 = l->getDoor2();

		if (door1 != NULL) {
			if (door1->getSection() == section) {
				objects.push_back(door1);
			}
		}
		if (door2 != NULL) {
			if (door2->getSection() == section) {
				objects.push_back(door2);
			}
		}

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

	void addFlagList(object* object) {
		EndFlag* temp;
		if (selectedObject->getCode() == "flag-down" || selectedObject->getCode() == "flag-up" || selectedObject->getCode() == "flag") {

			for (EndFlag* flag : flagList) {
				if (flag->getSection() == section) {
					flagList.remove(flag);
					break;
				}
			}

			temp = new EndFlag(selectedObject->getSprite()->getTexture(), selectedObject->getSprite()->getPosition(), selectedObject->getAngle(), section);
			flagList.push_back(temp);



		}
	}

	void hFlagYPos(object* thisFlag) {
		
		int sectCheck = section - 1;
		EndFlag* lastVFlag = getFlag(sectCheck);
		while (lastVFlag != NULL && lastVFlag->getAngle() != UP && lastVFlag->getAngle() != DOWN) {
			sectCheck -= 1;
			lastVFlag = getFlag(sectCheck);
				
		}
		if (lastVFlag != NULL) {
			if (lastVFlag->getAngle() == DOWN) {
				thisFlag->getSprite()->setPosition(Vector2f(thisFlag->getSprite()->getPosition().x, lastVFlag->getSprite()->getPosition().y));
			}
			else {
				thisFlag->getSprite()->setPosition(Vector2f(thisFlag->getSprite()->getPosition().x, lastVFlag->getSprite()->getPosition().y - 1080));
			}
		}
		
	}

	void downFlagYPos(object* thisFlag) {

		EndFlag* lastFlag = getFlag(section - 1);

		if (lastFlag->getCode() == "flag-up") {
			if (thisFlag->getSprite()->getPosition().y < lastFlag->getSprite()->getPosition().y) {
				EndFlag* lastVFlag = getLastFlagOfAngle(thisFlag, UP);

				if (lastVFlag != NULL) {
					thisFlag->getSprite()->setPosition(Vector2f(thisFlag->getSprite()->getPosition().x, lastVFlag->getSprite()->getPosition().y));
				}
			}
		}
		
	}

	void upFlagYPos(object* thisFlag) {
		EndFlag* lastFlag = getFlag(section - 1);

		if (lastFlag->getCode() == "flag-down") {
			if (thisFlag->getSprite()->getPosition().y > lastFlag->getSprite()->getPosition().y) {
				EndFlag* lastVFlag = getLastFlagOfAngle(thisFlag, DOWN);



				if (lastVFlag != NULL) {
					thisFlag->getSprite()->setPosition(Vector2f(thisFlag->getSprite()->getPosition().x, lastVFlag->getSprite()->getPosition().y));
				}
			}
		}
	}

	EndFlag* getLastFlagOfAngle(object* thisFlag, enum transitionAngle ang) {
		int sectCheck = section - 1;
		EndFlag* lastFlag = getFlag(sectCheck);
		while (lastFlag != NULL && lastFlag->getAngle() != ang) {
			sectCheck -= 1;
			lastFlag = getFlag(sectCheck);

		}
		return lastFlag;

		
	}

	EndFlag* getFlag(int sect) {
		for (EndFlag* flag : flagList) {
			if (flag->getSection() == sect) {
				return flag;
			}
		}
		return NULL;
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
				if (selectedPlaced->getCode() == "flag" || selectedPlaced->getCode() == "flag-left") {
					
					hFlagYPos(selectedPlaced);
				}
				else if (selectedPlaced->getCode() == "flag-down") {
					downFlagYPos(selectedPlaced);
				}
				else if (selectedPlaced->getCode() == "flag-up") {
					upFlagYPos(selectedPlaced);
				}
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
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse1Pressed == true) {
			mouse1Pressed = false;
			if (selectedObject != NULL) {
				addFlagList(selectedObject);
			}
		}

		if ((sf::Mouse::isButtonPressed(sf::Mouse::XButton1) && mouseX1Pressed != true )|| sf::Keyboard::isKeyPressed(sf::Keyboard::D)) //specifies
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
			l->loadFlags(levelName, &flagList, enemyT);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !cPressed) {
			checkpoint = !checkpoint;
			cPressed = true;
		}
		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
			cPressed = false;
		}
	}

	std::vector<std::string> splitString(const std::string& str, char delimiter) {
		std::vector<std::string> tokens;
		std::stringstream ss(str);
		std::string token;

		while (std::getline(ss, token, delimiter)) {
			tokens.push_back(token);
		}

		return tokens;
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



		int cameraHeight = 0;

		if (door1 != NULL) {
			door1->setCheckpoint();
		}

		for (object* o : objects) {
			if (o->getCode() == "flag" || o->getCode() == "flag-down" || o->getCode() == "flag-up" || o->getCode() == "flag-left" || o->getCode() == "door") {
				bool skip = false;
				if (o->getCode() == "flag") {
					if (door1 != NULL && door2 != NULL) {
						if (door1->getSection() == o->getSection() || door2->getSection() == o->getSection()) {
							skip = true;
						}
					}
				}

				if (!skip) {

					if (checkpoint) {
						o->setCheckpoint();
					}

					checkFlagDuplicates(o);

					ofstream* flagfile;
					flagfile = new ofstream();
					flagfile->open(saveFile + "-flags.txt", std::ios::app);

					*flagfile << section;
					*flagfile << ",";
					*flagfile << o->getCode() + ",";
					*flagfile << o->getSprite()->getPosition().x;
					*flagfile << ",";
					*flagfile << o->getSprite()->getPosition().y;
					*flagfile << ",";
					if (o->getCheckpoint()) {
						*flagfile << "1";
					}
					else {
						*flagfile << "0";
					}
					*flagfile << "\n";

					flagfile->close();
				}

			}
			else {
				*myfile << o->getCode() + ",";
				*myfile << o->getSprite()->getPosition().x;
				*myfile << ",";
				*myfile << o->getSprite()->getPosition().y;
				*myfile << ",";
				*myfile << o->getSprite()->getZ();


				*myfile << "\n";
			}
		}
		myfile->close();
	}

	void checkFlagDuplicates(object* o) {


		string line;

		char sep = ',';

		bool run = true;

		while (run) {
			run = false;
			ifstream* flagfile;
			flagfile = new ifstream();
			flagfile->open(levelName + "-flags.txt");
			while (getline(*flagfile, line)) {
				vector<string> values = splitString(line, sep);
				vector<string>::iterator valI = values.begin();
				string current = *valI;
				if (stoi(current) == o->getSection()) {
					flagfile->close();
					eraseFileLine(saveFile + "-flags.txt", line);
					run = true;
				}
			}
			flagfile->close();
		}
		//flagfile->close();

	}

	void eraseFileLine(std::string path, std::string eraseLine) {
		std::string line;
		std::ifstream fin;

		fin.open(path);
		// contents of path must be copied to a temp file then
		// renamed back to the path file
		std::ofstream temp;
		temp.open("temp.txt");

		while (getline(fin, line)) {
			// write all lines to temp other than the line marked for erasing
			if (line != eraseLine)
				temp << line << std::endl;
		}

		temp.close();
		fin.close();

		// required conversion for remove and rename functions
		const char* p = path.c_str();
		remove(p);
		rename("temp.txt", p);
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