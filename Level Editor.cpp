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
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "Mouse.cpp"
#include "Load.cpp"
#pragma once

class levelEditor {
	
	UISprite* tab;
	UISprite* typeTab;
	list<tile*> tileList;
	list<tile*> z2List;
	list<tile*> z3List;
	list<tile*> z4List;

	list<objectSelect*> tileDisplay;
	list<menuSelect*> tileSelect;
	list<menuSelect*> typeSelect;
	bool run = true;
	timer* time;
	camera* cam;

	string levelName;
	int section = 0;

	bool mouse1Pressed = false;
	bool mouse2Pressed = false;
	bool mouse3Pressed = false;
	bool xButton1Pressed = false;
	bool xButton2Pressed = false;

	bool rightPressed = false;
	bool leftPressed = false;

	list<menuSelect*>::iterator menuI;
	list<tile*>::iterator worldI;
	bool tileSelection = false;
	bool typeSelection = false;
	bool onZ = false;
	float z = 1;

	bool created = false;

	Vector2f flagLoc;

	int selectedTexture;
	tile* selectedTile;
	// 0 = backgorund, 1 = floor, 2 = Right wall, 3 = ceiling, 4 = left wall, 5 = floor and ceiling, 6 = left and right wall, 7 = full
	int selectedType;
	int typeHover;

	menuSelect* zSelect;

	Texture* tex;

	Vector2i dragStart;
	Vector2i highlightStart;

	sf::RectangleShape worldHighlight;
	sf::RectangleShape textureHighlight;
	sf::RectangleShape typeHighlight;

	bool del = false;

	mouse* m;
	Vector2f wSize = Vector2f(1920, 1080);

public:
	levelEditor(Texture* T, string levelN) {
		this->levelName = levelN;
		Texture* tabT = new Texture;
		tex = T;

		tabT->loadFromFile("Assets\\Tab.png");
		tab = new UISprite("UI", tabT, Vector2i(0, 0), Vector2i(414, 1080), Vector2f(0, 0), Vector2f(1, 1));
		typeTab = new UISprite("UI", tabT, Vector2i(0, 0), Vector2i(414, 1080), Vector2f(1920 - 414, 0), Vector2f(1, 1));

		for (int i = 0; i < 50; i++) {
			
			tileSelect.push_back(new menuSelect(T, Vector2i((i % 4) * 16, (i / 4) * 16), Vector2f((((i % 4) * 20) * 4) + 20, (((i / 4) * 20) * 4) + 20)));
		}
		Texture* typeT = new Texture();
		typeT->loadFromFile("Assets\\Tile Select.png");
		for (int i = 0; i < 10; i++) {
			typeSelect.push_back(new menuSelect(typeT, Vector2i((i* 16), 0), Vector2f((((i % 4) * 20) * 4) + 1600, (((i / 4) * 20) * 4) + 20)));
		}

		Texture* zT = new Texture();
		zT->loadFromFile("Assets\\Z.png");
		zSelect = new menuSelect(zT, Vector2i(0, 0), Vector2f(1800, 900));


		cam = new camera();

		flagCheck();


		//worldHighlight.setPosition(selectedTile->getSprite()->getPosition());
		worldHighlight.setFillColor(sf::Color(0, 255, 0, 128));
		worldHighlight.setSize(Vector2f(16 * 2, 16 * 2));

		textureHighlight.setFillColor(sf::Color(0, 255, 0, 128));
		textureHighlight.setSize(Vector2f(16 * 4, 16 * 4));
		textureHighlight.setPosition(Vector2f((((selectedTexture % 4) * 20) * 4) + 20, (((selectedTexture / 4) * 20) * 4) + 20));

		typeHighlight.setFillColor(sf::Color(0, 255, 0, 128));
		typeHighlight.setSize(Vector2f(16 * 4, 16 * 4));

		m = new mouse();

	}

	void flagCheck() {
		Load* load = new Load();
		list<object*> objects;
		if (section == 0) {
			load->loadObjects(levelName, &objects, new Texture());
		}
		else {
			load->loadObjects(levelName + to_string(section), &objects, new Texture());
		}
		for (object* o : objects) {
			if (o->getCode() == "flag") {
				flagLoc = o->getSprite()->getPosition();
			}
		}
	}

	void loop(renderer* instance, double targetRate) {

		cam->setZoom(0.5);
		
		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		Load* l = new Load();
		l->load(levelName, tex, &tileList, &z2List, &z3List, &z4List);
		changeZ();
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

			Vector2i mousePos = m->getPosition(instance, wSize);

			if (m->checkActive()) {
				if (z == 1) {
					mouseCheck(&tileList, instance, mousePos);
				}
				else if (z == 2) {
					mouseCheck(&z2List, instance, mousePos);
				}
				else if (z == 3) {
					mouseCheck(&z3List, instance, mousePos);
				}
				else if (z == 4) {
					mouseCheck(&z4List, instance, mousePos);
				}
			}
			keyBoardCheck();

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
			

			instance->UIDisplay(list<UISprite*> {tab, typeTab});
			for (menuSelect* t : tileSelect) {
				instance->UIDisplay(t->getSprite());
			}

			for (menuSelect* t : typeSelect) {
				instance->UIDisplay(t->getSprite());
			}
			

			if (selectedTile != NULL) {

				Vector2f size = worldHighlight.getSize();

				if (size.x < 0) {
					
					worldHighlight.setPosition(Vector2f(worldHighlight.getPosition().x + (16 * 2), worldHighlight.getPosition().y));
				}
			
				if (size.y < 0) {
					worldHighlight.setPosition(Vector2f(worldHighlight.getPosition().x, worldHighlight.getPosition().y + (16 * 2)));
				}


				if (created) {
					worldHighlight.setPosition(selectedTile->getSprite()->getCameraPosition() * z);
				}
				else {
					worldHighlight.setPosition(selectedTile->getSprite()->getCameraPosition());
				}

				if (created) {
					selectedTile = NULL;
					tileList.remove(*prev(tileList.end()));
					created = false;
				}
				
			}

			instance->getWindow()->draw(worldHighlight);

			
			instance->getWindow()->draw(textureHighlight);

			for (tile* t : tileList) {
				if (t->getCeiling() != NULL) {
					instance->objectHitboxSetup(t->getCeiling(), cam);
				}
				if (t->getGround() != NULL) {
					instance->objectHitboxSetup(t->getGround(), cam);
				}
				if (t->getLeft() != NULL) {
					instance->objectHitboxSetup(t->getLeft(), cam);
				}
				if (t->getRight() != NULL) {
					instance->objectHitboxSetup(t->getRight(), cam);
				}
			}

			instance->UIDisplay(zSelect->getSprite());

			instance->getWindow()->display();
			instance->getWindow()->clear();



		}
	}

	void keyBoardCheck() {
		if (Keyboard::isKeyPressed(Keyboard::Scan::Right) && !rightPressed) {
			section++;
			//cam->setPosition(Vector2f (flagLoc.x - 900, flagLoc.y - 900));
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
		Load* load = new Load();
		if (section != 0) {
			load->load(levelName + to_string(section), tex, &tileList, &z2List, &z3List, &z4List);
		}
		else {
			load->load(levelName, tex, &tileList, &z2List, &z3List, &z4List);
		}
		flagCheck();
		changeZ();
	}

	void mouseCheck(list<tile*> *tileList, renderer* instance, Vector2i mousePos) {
		

		if (UITextureCheck(mousePos)) {
			tileSelection = true;
			typeSelection = false;
			
		}
		else {
			tileSelection = false;
			if (UITypeCheck(mousePos)) {
				
				typeSelection = true;
				
			}
			else {
				typeSelection = false;
				if (UIHitboxCheck(mousePos, zSelect->getHitbox())) {
					onZ = true;
				}
				else {
					onZ = false;
				}
			}

		}
		



		

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse1Pressed != true) //specifies
		{
			mouse1Click(tileList, mousePos);
		}

		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			if (!tileSelection && !typeSelection) {

				
					mouse1Hold(tileList, mousePos);
				
			}
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse1Pressed) {
			mouse1Pressed = false;
			
			if (!onZ) {
				mouse1Release(tileList, mousePos);
			}
			
			
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

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && mouse3Pressed != true) //specifies
		{
			mouse3Pressed = true;

			save();

		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		}
		else {
			mouse3Pressed = false;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::XButton1) && !xButton1Pressed) {
			del = !del;
			xButton1Pressed = true;

			selectedTile = NULL;
			worldHighlight.setSize(Vector2f(0, 0));

			cout << del;
			cout << ", ";
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::XButton1)) {
			xButton1Pressed = false;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::XButton2) && !xButton2Pressed) {

			xButton2Pressed = true;
			selectedTile = NULL;
			worldHighlight.setSize(Vector2f(0, 0));
			zoom(true);
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::XButton2) && xButton2Pressed == true) {
			zoom(false);
			xButton2Pressed = false;
		}
	}

	void zoom(bool b) {
		if (b) {
			cam->setZoom(1);
			cam->setPosition(Vector2f(cam->getPosition().x + 450, cam->getPosition().y));
		}
		else {
			cam->setZoom(0.5);
			cam->setPosition(Vector2f(cam->getPosition().x - 450, cam->getPosition().y));
		}
	}

	Vector2i mouseWorld(Vector2i mousePos) {
		//return Vector2i(mousePos.x + (cam->getPosition().x / 2), mousePos.y + (cam->getPosition().y / 2));
		return Vector2i(int((mousePos.x + cam->getPosition().x / 2) / (4 * 8)), int((mousePos.y + cam->getPosition().y / 2) / (4 * 8)));
	}

	void mouse1Release(list<tile*> *tileList, Vector2i mousePos) {
		
		Vector2i currentWorld = mouseWorld(mousePos);
		//currentPos = Vector2i(currentPos.x , currentPos.x + (cam->getPosition().x / 2));
		Vector2f highlightW = Vector2f((worldHighlight.getPosition().x + cam->getPosition().x/2) /(16*2), (worldHighlight.getPosition().y +cam->getPosition().y /2 )/ (16 * 2));

		Vector2i start = Vector2i(highlightW.x,highlightW.y);

		Vector2i size = Vector2i(worldHighlight.getSize().x / (16*2), worldHighlight.getSize().y/(16*2));

		list<tile*>::iterator tI = tileList->begin();

		for (int i = 0; i < (size.x); i++) {
			for (int j = 0; j < (size.y ); j++) {

				bool check = false;
				for (tile* t : *tileList) {
					check = rectCheck(Vector2f((i + start.x), (j + start.y)), t->getLocation());
					if (check) {
						break;
					}
					tI = next(tI);
				}

				if (check) {
					tileList->erase(tI);
				}
				tI = tileList->begin();
				if (!del) {
					tile* temp = tileCreation(Vector2f((i + start.x), (j + start.y)), selectedType, selectedTexture);
					//temp->getSprite()->setZ(z);
					tileList->push_back(temp);
				}
				selectedTile = NULL;
			}
		}
		
	}

	bool rectCheck(Vector2f newLoc, Vector2f tileLoc) {
		if (newLoc == tileLoc) {
			return true;
		}
		return false;
	}

	void mouse1Hold(list<tile*> *tileList, Vector2i mousePos) {
		Vector2i currentPos = Vector2i(mousePos.x - (mousePos.x % (16 * 2)), mousePos.y - (mousePos.y % (16 * 2)));
		Vector2f size = Vector2f((currentPos.x - highlightStart.x)+(16*2), (currentPos.y - highlightStart.y) + (16*2));

		if (size.x >= 0) {
			//size.x = size.x + (16 * 2);
		}
		else {
			//size.x = size.x + (16 * 2);
			//worldHighlight.setPosition(Vector2f(worldHighlight.getPosition().x + (16 * 2), worldHighlight.getPosition().y));
		}
		if (size.y >= 0) {
			//size.y = size.y + (16 * 2);
		}
		else {
			//size.y = size.y - (16 * 2);
			//worldHighlight.setPosition(Vector2f(worldHighlight.getPosition().x, worldHighlight.getPosition().y + (16 * 2)));
		}

		worldHighlight.setSize(size);


	}


	void mouse1Click(list<tile*> *tileList, Vector2i mousePos) {
		highlightStart = Vector2i(mousePos.x - (mousePos.x % (16 * 2)), mousePos.y - (mousePos.y % (16 * 2)));
		mouse1Pressed = true;
		
		 if (typeSelection) {


			if (menuI != typeSelect.end()) {
				if (selectedTile == NULL) {
					menuSelect* temp = *menuI;
					selectedType = typeHover;
				}
				else {
					selectedType = typeHover;
					tileList->push_back(tileCreation(selectedTile->getLocation(), selectedType, selectedTexture));
					tileList->erase(worldI);
					
					worldI = prev(tileList->end());
				}
			}
		}
		else if (tileSelection) {
			if (selectedTile == NULL) {
				if (menuI != tileSelect.end()) {
					menuSelect* temp = *menuI;
					selectedTexture = (temp->getSprite()->getRect().getPosition().x / 16) + (temp->getSprite()->getRect().getPosition().y / 16) * 4;
					textureHighlight.setPosition(Vector2f((((selectedTexture % 4) * 20) * 4) + 20, (((selectedTexture / 4) * 20) * 4) + 20));
				}
			}

			else {
				if (menuI != tileSelect.end()) {
					menuSelect* temp = *menuI;
					selectedTile->setTileNum((temp->getSprite()->getRect().getPosition().x / 16) + (temp->getSprite()->getRect().getPosition().y / 16) * 4);

				}

			}

		}
		else if (onZ) {
			z++;
			if (z >= 5) {
				z = 1;
			}

			changeZ();

		}
		else {
			worldInteraction(mousePos);
		}
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
			else if (z == 2){
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


	void drag(Vector2i mouse) {
		Vector2f currentPos = cam->getPosition();
		cam->setPosition(Vector2f(currentPos.x - (dragStart.x - mouse.x)/2, currentPos.y - (dragStart.y - mouse.y)/2));
	}


	void miniSave(list<tile*> tList, ofstream* myfile) {
		for (tile* t : tList) {

			if (t->getLadder() != NULL) {
				if (t->getGround() != NULL) {
					*myfile << "9,";
				}
				else {
					*myfile << "8,";
				}
			}
			else if (t->getCeiling() != NULL && t->getGround() != NULL && t->getLeft() != NULL && t->getRight() != NULL) {
				*myfile << "5,";
			}

			else if (t->getGround() != NULL) {
				*myfile << "1,";
			}
			else if (t->getCeiling() != NULL) {
				*myfile << "3,";
			}
			else if (t->getRight() != NULL) {
				*myfile << "2,";
			}
			else if (t->getLeft() != NULL) {
				*myfile << "4,";
			}
			else {
				*myfile << "0,";
			}

			*myfile << t->getLocation().x;
			*myfile << ",";
			*myfile << t->getLocation().y;
			*myfile << ",";
			*myfile << t->getTileNum();
			*myfile << ",";
			if (t->getZ() != NULL) {
				*myfile << t->getZ();
			}
			else {
				*myfile << 1;
			}
			*myfile << "\n";

		}
	}

	void save() {
		ofstream* myfile;
		myfile = new ofstream();
		if (section == 0) {
			myfile->open(levelName + ".txt");
		}
		else {
			myfile->open(levelName + to_string(section) + ".txt");
		}
		miniSave(tileList, myfile);
		miniSave(z2List, myfile);
		miniSave(z3List, myfile);
		miniSave(z4List, myfile);

		myfile->close();
	}


	tile* tileCreation(Vector2f worldPos,int selectedType, int selectedTexture) {

		if (selectedType == 0) {
			return new tile(worldPos, tex, selectedTexture, z);
		}
		else if (selectedType == 1) {
			return new topTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == 2) {
			return new rightTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == 3) {
			return new ceilingTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == 4) {
			return new leftTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == 8) {
			return new ladderTile(worldPos, tex);
		}
		else if (selectedType == 9) {
			return new topLadder(worldPos, tex);
		}
		else {
			return new solidTile(worldPos, tex, selectedTexture);
		}
	}

	void worldInteraction(Vector2i mousePos) {

		Vector2f worldPos = Vector2f(int((mousePos.x + cam->getPosition().x/2) / (4 * 8)), int((mousePos.y + cam->getPosition().y/2) / (4 * 8)));


		Vector2f screenPos = Vector2f(worldPos.x / (4 * 8), worldPos.y / (4 * 8));
		if (!worldCheck(worldPos, screenPos)) {
			//selectedTile = NULL;
			selectedTile = tileCreation(worldPos, selectedType, 999);
			created = true;
			
			selectedTile->getSprite()->setZ(z);
			tileList.push_back(selectedTile);
			
		}
		else {
			

			if (del) {
				
				selectedTile = *worldI;
				//worldHighlight.setPosition(selectedTile->getSprite()->getCameraPosition());
				tileList.erase(worldI);
				
			}
			else {
				selectedTile = *worldI;
				

				//worldHighlight.setPosition(selectedTile->getSprite()->getCameraPosition());
			}
		}
	}

	bool worldCheck(Vector2f worldPos, Vector2f screenPos) {

		worldI = tileList.begin();

		for (tile* t : tileList) {
			Vector2f loc = t->getLocation();

			if (worldPos.x == loc.x && worldPos.y == loc.y) {
				return true;
			}
			worldI = next(worldI);
		}

		return false;
	}




	bool UITextureCheck(Vector2i mousePos) {

		menuI = tileSelect.begin();
		for (menuSelect* m : tileSelect) {
			if (UIHitboxCheck(mousePos, m->getHitbox())) {
				return true;
			}
			menuI = next(menuI);
		}
		return false;
	}

	bool UITypeCheck(Vector2i mousePos) {

		menuI = typeSelect.begin();
		typeHover = 0;
		for (menuSelect* m : typeSelect) {
			if (UIHitboxCheck(mousePos, m->getHitbox())) {
				
				return true;
			}
			menuI = next(menuI);
			typeHover++;
		}
		return false;
	}

	bool UIHitboxCheck(Vector2i mousePos, UIHitbox* hit) {

		if (hit->getCameraPos().x + hit->getSize().x > mousePos.x && mousePos.x > hit->getCameraPos().x) {
			if (hit->getCameraPos().y + hit->getSize().y > mousePos.y && mousePos.y > hit->getCameraPos().y) {
				return true;
			}
		}

		return false;

	}
	
};