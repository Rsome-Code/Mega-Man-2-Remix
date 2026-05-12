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
#include "horizontal lava.cpp"
#include "vertical lava.cpp"
#include "pour lava.cpp"
#include "anim Tile.cpp"
#include "water anim.cpp"
#include "water tile.cpp"
#include "conveyor.cpp"
#include "move tile.cpp"
#include "right edge.cpp"
#include "left edge.cpp"
#include "top edge.cpp"
#include "bottom edge.cpp"
#include "top left corner.cpp"
#include "top right corner.cpp"
#include "bottom right corner.cpp"
#include "bottom left corner.cpp"
#include "square flash.cpp"
#include "h tube tile.cpp"
#include "v tube tile.cpp"
#include "top left w.cpp"
#include "top right w.cpp"
#include "bottom left w.cpp"
#include "bottom right w.cpp"
#include "W Ceiling.cpp"
#include "W Roof.cpp"
#include "W wall.cpp"
#include "bottom left corner w.cpp"
#pragma once

class levelEditor {

	shared_ptr<UISprite> tab;
	shared_ptr<UISprite> typeTab;
	list<shared_ptr<tile>> tileList;

	list<shared_ptr<tile>> z2List;
	list<shared_ptr<tile>> z3List;
	list<shared_ptr<tile>> z4List;

	list<objectSelect*> tileDisplay;
	list<shared_ptr<menuSelect>> tileSelect;
	list<shared_ptr<menuSelect>> typeSelect;
	bool run = true;
	shared_ptr<timer> time;
	shared_ptr<camera> cam;

	string levelName;
	int section = 0;

	bool mouse1Pressed = false;
	bool mouse2Pressed = false;
	bool mouse3Pressed = false;
	bool xButton1Pressed = false;
	bool xButton2Pressed = false;

	bool rightPressed = false;
	bool leftPressed = false;

	list<shared_ptr<menuSelect>>::iterator menuI;
	list<shared_ptr<tile>>::iterator worldI;
	bool tileSelection = false;
	bool typeSelection = false;
	bool onZ = false;
	float z = 1;

	bool created = false;

	Vector2f flagLoc;

	int selectedTexture;
	shared_ptr<tile> selectedTile;

	// 0 = backgorund, 1 = floor, 2 = Right wall, 3 = ceiling, 4 = left wall, 5 = floor and ceiling, 6 = left and right wall, 7 = full, 8 = ladder, 9 = top ladder, 10 = hori lava, 11 = vert lava, 12 = pour lava
	int selectedType;
	int typeHover;

	shared_ptr<menuSelect> zSelect;

	shared_ptr<Texture> tex;

	Vector2i dragStart;
	Vector2i highlightStart;

	sf::RectangleShape worldHighlight;
	sf::RectangleShape textureHighlight;
	sf::RectangleShape typeHighlight;

	bool del = false;

	mouse* m;
	Vector2f wSize = Vector2f(1920, 1080);

	bool zoomed = false;

	shared_ptr<Font> font;

public:

	virtual ~levelEditor() {


	}

	levelEditor(shared_ptr<Texture> T, string levelN, shared_ptr<Font> f) {
		this->levelName = levelN;
		shared_ptr<Texture> tabT = shared_ptr<Texture>(new Texture);
		tex = T;

		tabT->loadFromFile("Assets\\Tab.png");
		tab = shared_ptr<UISprite>(new UISprite("UI", tabT, Vector2i(0, 0), Vector2i(414, 1080), Vector2f(0, 0), Vector2f(1, 1)));
		typeTab = shared_ptr<UISprite>(new UISprite("UI", tabT, Vector2i(0, 0), Vector2i(414, 1080), Vector2f(1920 - 414, 0), Vector2f(1, 1)));

		for (int i = 0; i < 50; i++) {
			
			tileSelect.push_back(shared_ptr<menuSelect>(new menuSelect(T, Vector2i((i % 4) * 16, (i / 4) * 16), Vector2f((((i % 4) * 20) * 4) + 20, (((i / 4) * 20) * 4) + 20))));
		}
		shared_ptr<Texture> typeT = shared_ptr<Texture> (new Texture());
		typeT->loadFromFile("Assets\\Tile Select\\" + levelN + ".png");
		for (int i = 0; i < 30; i++) {
			typeSelect.push_back(shared_ptr<menuSelect>(new menuSelect(typeT, Vector2i((i* 16), 0), Vector2f((((i % 4) * 20) * 4) + 1600, (((i / 4) * 20) * 4) + 20))));
		}

		shared_ptr<Texture> zT = shared_ptr<Texture> (new Texture());
		zT->loadFromFile("Assets\\Z.png");
		zSelect = shared_ptr<menuSelect>(new menuSelect(zT, Vector2i(0, 0), Vector2f(1800, 900)));


		cam = shared_ptr<camera>(new camera());

		flagCheck();


		//worldHighlight.setPosition(selectedTile->getSprite()->getPosition());
		worldHighlight.setFillColor(Colour(0, 255, 0, 128).getColour());
		worldHighlight.setSize(Vector2f(16 * 2, 16 * 2));

		textureHighlight.setFillColor(Colour(0, 255, 0, 128).getColour());
		textureHighlight.setSize(Vector2f(16 * 4, 16 * 4));
		textureHighlight.setPosition(Vector2f((((selectedTexture % 4) * 20) * 4) + 20, (((selectedTexture / 4) * 20) * 4) + 20));

		typeHighlight.setFillColor(Colour(0, 255, 0, 128).getColour());
		typeHighlight.setSize(Vector2f(16 * 4, 16 * 4));

		m = new mouse();

		beatSet = 0;
		beatText = shared_ptr<Text> (new Text());
		beatText->setFont(*f);
		beatText->setString("Beat: " + to_string(beatSet + 1));
		beatText->setPosition(Vector2f(20, 900));
		beatText->setFillColor(Colour::Black().getColour());

		font = shared_ptr<Font>(new Font());
		font->loadFromFile("assets\\font.otf");



	}

	void flagCheck() {
		shared_ptr<Load> load = shared_ptr<Load>(new Load());
		list<shared_ptr<GameObject>> objects;

		load->loadObjects(levelName, to_string(section), &objects, shared_ptr<Texture> (new Texture()), cam);

		for (shared_ptr<object> o : objects) {
			if (o->getCode() == "flag") {
				flagLoc = o->getSprite()->getPosition();
			}
		}
	}

	void loop(shared_ptr<renderer> instance, double targetRate) {

		cam->setZoom(0.5);
		
		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		shared_ptr<Load> l = shared_ptr<Load>(new Load());
		l->load(levelName, to_string(section), tex, &tileList, &z2List, &z3List, &z4List);
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

			if (instance->getWindow()->hasFocus()) {
				keyBoardCheck();
			}


			if (zoomed) {
				//cam->setPosition(Vector2f(cam->getPosition().x, ))
			}

			for (shared_ptr<tile> t : tileList) {
				instance->objectSetup(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z2List) {
				instance->objectSetup(t->getSprite(), cam);
			}
			for (shared_ptr<tile> t : z3List) {
				instance->objectSetup(t->getSprite(), cam);
			}
			if (z != 4) {
				for (shared_ptr<tile> t : z4List) {
					instance->bObjectDisplay(t->getSprite(), cam);
				}
			}
			else {
				for (shared_ptr<tile> t : z4List) {
					instance->objectAccess(t, cam);
				}
			}

			if (z <= 3) {
				if (z != 3) {
					for (shared_ptr<tile> t : z3List) {
						instance->bObjectDisplay(t->getSprite(), cam);
					}
				}
				else {
					for (shared_ptr<tile> t : z3List) {
						instance->objectAccess(t, cam);
					}
				}
				

			}

			if (z <= 2) {
				if (z != 2) {
					for (shared_ptr<tile> t : z2List) {
						instance->bObjectDisplay(t->getSprite(), cam);
					}
				}
				else {
					for (shared_ptr<tile> t : z2List) {
						instance->objectAccess(t, cam);
					}
				}

			}

			if (z == 1) {
				for (shared_ptr<tile> t : tileList) {
					instance->objectAccess(t, cam);
				}
			}
			
			if (!zoomed) {

				for (shared_ptr<tile> t : tileList) {

					if (t->getText(font) != NULL) {
						instance->textDisplay(t->getText(font));
					}
					if (levelName != "flash man") {
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
					if (z == 1) {
						if (t->getWaterBox() != NULL) {
							instance->objectHitboxSetup(t->getWaterBox(), cam, Colour::Cyan());
						}
						if (t->getDeathBox() != NULL) {
							instance->objectHitboxSetup(t->getDeathBox(), cam, Colour::Red());
						}
					}
				}
				instance->UIDisplay(list<shared_ptr<UISprite>> {tab, typeTab});
				for (shared_ptr<menuSelect> t : tileSelect) {
					instance->UIDisplay(t->getSprite());
				}

				for (shared_ptr<menuSelect> t : typeSelect) {
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

				if (levelName == "flash man") {
					instance->textDisplay(beatText);
				}

				instance->getWindow()->draw(worldHighlight);


				instance->getWindow()->draw(textureHighlight);

				

				instance->UIDisplay(zSelect->getSprite());
			}


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

		if (Keyboard::isKeyPressed(Keyboard::Scan::Num1) || Keyboard::isKeyPressed(Keyboard::Scan::Numpad1)) {
			beatSet = 0;
			beatText->setString("Beat: " + to_string(beatSet + 1));
		}
		if (Keyboard::isKeyPressed(Keyboard::Scan::Num2) || Keyboard::isKeyPressed(Keyboard::Scan::Numpad2)) {
			beatSet = 1;
			beatText->setString("Beat: " + to_string(beatSet + 1));
		}
		if (Keyboard::isKeyPressed(Keyboard::Scan::Num3) || Keyboard::isKeyPressed(Keyboard::Scan::Numpad3)) {
			beatSet = 2;
			beatText->setString("Beat: " + to_string(beatSet + 1));
		}
	}

	void reload() {
		tileList.clear();
		z2List.clear();
		z3List.clear();
		z4List.clear();
		shared_ptr<Load> load = shared_ptr<Load>(new Load());
		load->load(levelName, to_string(section), tex, &tileList, &z2List, &z3List, &z4List);
		
		flagCheck();
		changeZ();
	}

	void mouseCheck(list<shared_ptr<tile>> *tileList, shared_ptr<renderer> instance, Vector2i mousePos) {
		

		if (UITextureCheck(mousePos)) {
			tileSelection = true;
			typeSelection = false;
			
		}
		else {
			tileSelection = false;

			if (UIHitboxCheck(mousePos, zSelect->getHitbox())) {
				onZ = true;
			}
			else {
				onZ = false;
			}
			
			if (UITypeCheck(mousePos)) {
				
				typeSelection = true;
				
			}
			else {
				typeSelection = false;
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

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && mouse3Pressed != true || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) //specifies
		{
			mouse3Pressed = true;
			z = 1;
			save();
			changeZ();
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

		zoomed = b;
	}

	Vector2i mouseWorld(Vector2i mousePos) {
		//return Vector2i(mousePos.x + (cam->getPosition().x / 2), mousePos.y + (cam->getPosition().y / 2));
		return Vector2i(int((mousePos.x + cam->getPosition().x / 2) / (4 * 8)), int((mousePos.y + cam->getPosition().y / 2) / (4 * 8)));
	}


	void mouse1Release(list<shared_ptr<tile>> *tileList, Vector2i mousePos) {
		
		Vector2i currentWorld = mouseWorld(mousePos);
		//currentPos = Vector2i(currentPos.x , currentPos.x + (cam->getPosition().x / 2));
		Vector2f highlightW = Vector2f((worldHighlight.getPosition().x + cam->getPosition().x/2) /(16*2), (worldHighlight.getPosition().y +cam->getPosition().y /2)/ (16 * 2));

		Vector2i start = Vector2i(highlightW.x,highlightW.y);

		Vector2i size = Vector2i(worldHighlight.getSize().x / (16*2), worldHighlight.getSize().y/(16*2));

		
		list<shared_ptr<tile>>::iterator tI = tileList->begin();

		list<shared_ptr<tile>> tempList;

		int repeat = 1;

		//if (levelName == "crash man") {
		//	repeat = 2;
		//}

		for (int r = 0; r < repeat; r++) {
			for (int i = 0; i < (size.x); i++) {
				for (int j = 0; j < (size.y); j++) {
					tI = tileList->begin();
					bool check = false;

					//Checks for tiles in the same location
					if (tileList->size() > 0) {
						for (shared_ptr<tile> t : *tileList) {
							check = rectCheck(Vector2f((i + start.x), (j + start.y)), t->getLocation());
							if (check) {
								break;
							}
							tI = next(tI);
						}

						if (check) {
							tileList->erase(tI);
						}
					}
					tI = tileList->begin();

					if (!del) {
						shared_ptr<tile> temp = tileCreation(Vector2f((i + start.x), (j + start.y)), selectedType, selectedTexture);

						//temp->getSprite()->setZ(z);
						tileList->push_back(temp);

						tempList.push_back(temp);
					}
					selectedTile = NULL;
				}
			}

			if (levelName == "flash man" && selectedType == 20) {
				for (shared_ptr<tile> t : tempList) {
					FlashTile temp = *flashTileCheck(t->getLocation());
					temp.setTiming(beatSet);
					*t = temp;
				}
			}
		}
		
	}

	bool rectCheck(Vector2f newLoc, Vector2f tileLoc) {
		if (newLoc == tileLoc) {
			return true;
		}
		return false;
	}

	void mouse1Hold(list<shared_ptr<tile>> *tileList, Vector2i mousePos) {
		Vector2i currentPos = Vector2i(mousePos.x - (mousePos.x % (16 * 2)), mousePos.y - (mousePos.y % (16 * 2)));
		Vector2f size = Vector2f((currentPos.x - highlightStart.x)+(16*2), (currentPos.y - highlightStart.y) + (16*2));

		/*if (size.x >= 0) {
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
		}*/

		worldHighlight.setSize(size);


	}


	void mouse1Click(list<shared_ptr<tile>> *tileList, Vector2i mousePos) {
		highlightStart = Vector2i(mousePos.x - (mousePos.x % (16 * 2)), mousePos.y - (mousePos.y % (16 * 2)));
		mouse1Pressed = true;
		
		 if (typeSelection) {


			if (menuI != typeSelect.end()) {
				if (selectedTile == NULL) {
					shared_ptr<menuSelect> temp = *menuI;
					selectedType = typeHover;
				}
				else {
					selectedType = typeHover;
					tileList->push_back(tileCreation(selectedTile->getLocation(), selectedType, selectedTexture));
					tileList->erase(worldI);
					
					worldI = prev(tileList->end());
				}
			}
			else if (onZ) {
				z++;
				if (z >= 5) {
					z = 1;
				}

				selectedType = 0;

				changeZ();

			}
		}
		else if (tileSelection) {
			if (selectedTile == NULL) {
				if (menuI != tileSelect.end()) {
					shared_ptr<menuSelect> temp = *menuI;
					selectedTexture = (temp->getSprite()->getRect().getPosition().x / 16) + (temp->getSprite()->getRect().getPosition().y / 16) * 4;
					textureHighlight.setPosition(Vector2f((((selectedTexture % 4) * 20) * 4) + 20, (((selectedTexture / 4) * 20) * 4) + 20));
				}
			}

			else {
				if (menuI != tileSelect.end()) {
					shared_ptr<menuSelect> temp = *menuI;
					selectedTile->setTileNum((temp->getSprite()->getRect().getPosition().x / 16) + (temp->getSprite()->getRect().getPosition().y / 16) * 4);

				}

			}

		}
		
		else {
			worldInteraction(mousePos);
		}
	}

	void changeZ() {
		
		for (shared_ptr<tile> t : z2List) {
			if (z == 1) {
				t->getSprite()->setZ(1.25);
			}
			else {
				t->getSprite()->setZ(1);
			}
		}
		for (shared_ptr<tile> t : z3List) {
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
		for (shared_ptr<tile> t : z4List) {
			if (z == 1) {
				t->getSprite()->setZ(1.75);
			}
			else if (z == 2) {
				t->getSprite()->setZ(1.5);
			}
			else if (z == 3) {
				t->getSprite()->setZ(1.25);
			}
			else {
				t->getSprite()->setZ(1);
			}
		}
	}


	void drag(Vector2i mouse) {
		Vector2f currentPos = cam->getPosition();
		cam->setPosition(Vector2f(currentPos.x - (dragStart.x - mouse.x) / 2, currentPos.y - (dragStart.y - mouse.y) / 2));
	}


	void miniSave(list<shared_ptr<tile>> tList, shared_ptr<ofstream> myfile) {
		for (shared_ptr<tile> t : tList) {


			*myfile <<t->getType() + ",";


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
		shared_ptr<ofstream> myfile;
		myfile = shared_ptr<ofstream> (new ofstream());

		myfile->open(levelName + "\\" + to_string(section) + ".txt");


		miniSave(tileList, myfile);
		miniSave(z2List, myfile);
		miniSave(z3List, myfile);
		miniSave(z4List, myfile);

		myfile->close();
	}

	void worldInteraction(Vector2i mousePos) {

		Vector2f worldPos = Vector2f(int((mousePos.x + cam->getPosition().x / 2) / (4 * 8)), int((mousePos.y + cam->getPosition().y / 2) / (4 * 8)));



		if (worldPos.x <= 0) {
			worldPos.x -= 1;
		}
		if (worldPos.y <= 0) {
			worldPos.y -= 1;
		}

		Vector2f screenPos = Vector2f(worldPos.x / (4 * 8), worldPos.y / (4 * 8));
		if (!worldCheck(worldPos)) {
			//selectedTile = NULL;
			selectedTile = tileCreation(worldPos, selectedType, selectedTexture);
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

	bool worldCheck(Vector2f worldPos) {
		worldI = tileList.begin();

		for (shared_ptr<tile> t : tileList) {
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
		for (shared_ptr<menuSelect> m : tileSelect) {
			if (UIHitboxCheck(mousePos, m->getHitbox())) {
				return true;
			}
			menuI = next(menuI);
		}
		if (mousePos.x < tab->getCameraPosition().x + tab->getSize().x && mousePos.x > tab->getCameraPosition().x) {
			if (mousePos.y < tab->getCameraPosition().y + tab->getSize().y && mousePos.y > tab->getCameraPosition().y) {
				return true;
			}
		}
		return false;
	}

	bool UITypeCheck(Vector2i mousePos) {

		menuI = typeSelect.begin();
		typeHover = 0;
		for (shared_ptr<menuSelect> m : typeSelect) {
			if (UIHitboxCheck(mousePos, m->getHitbox())) {

				return true;
			}
			menuI = next(menuI);
			typeHover++;
		}
		if (mousePos.x < typeTab->getCameraPosition().x + typeTab->getSize().x && mousePos.x > typeTab->getCameraPosition().x) {
			if (mousePos.y < typeTab->getCameraPosition().y + typeTab->getSize().y && mousePos.y > typeTab->getCameraPosition().y) {
				return true;
			}
		}
		return false;
	}

	bool UIHitboxCheck(Vector2i mousePos, shared_ptr<UIHitbox> hit) {

		if (hit->getCameraPos().x + hit->getSize().x > mousePos.x && mousePos.x > hit->getCameraPos().x) {
			if (hit->getCameraPos().y + hit->getSize().y > mousePos.y && mousePos.y > hit->getCameraPos().y) {
				return true;
			}
		}

		return false;

	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Awfully written
	//Never do the "name check" if statement list again
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	shared_ptr<tile> tileCreation(Vector2f worldPos, int selectedType, int selectedTexture) {


		if (levelName == "flash man") {
			return flashCheck(worldPos);
		}

		

		else if (selectedType == 0) {
			return shared_ptr<tile>(new tile(worldPos, tex, selectedTexture, z));
		}
		else if (selectedType == 1) {
			return shared_ptr<tile>(new topTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == 2) {
			return shared_ptr<tile>(new rightTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == 3) {
			return shared_ptr<tile>(new ceilingTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == 4) {
			return shared_ptr<tile>(new leftTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == 8) {
			return shared_ptr<tile>(new ladderTile(worldPos, tex));
		}
		else if (selectedType == 9) {
			return shared_ptr<tile>(new topLadder(worldPos, tex));
		}
		
		else if (selectedType == 5 || selectedType == 6 ||selectedType == 7) {
			return shared_ptr<tile>(new solidTile(worldPos, tex, selectedTexture));
		}
		else if (levelName == "wood man" || levelName == "bubble man" || levelName == "heat man") {
			return bubbleCheck(worldPos);
		}

		else if (levelName == "crash man") {
			return crashCheck(worldPos);
		}

		else if (levelName == "metal man") {
			return metalCheck(worldPos);
		}
		
		else {
			return shared_ptr<tile>(new solidTile(worldPos, tex, selectedTexture));
		}
	}

	//Ugh

	shared_ptr<tile> crashCheck(Vector2f worldPos) {
		/*if (selectedType == 10) {
			
			shared_ptr<CrashTile> newTile = shared_ptr<CrashTile>(new Crash1(worldPos, tex, selectedTexture));

			for (shared_ptr<CrashTile> t : cList) {
				//if (t->checkCrash()) {
				CrashCheck::checkCrash(&newTile, &t, worldPos, tex);
				//}
			}
			cList.push_back(newTile);
			return newTile;
			
		}*/
		return NULL;
	}

	shared_ptr<tile> bubbleCheck(Vector2f worldPos) {
		if (selectedType == 10) {
			return shared_ptr<tile>(new HorizontalLava(worldPos, tex, z));
		}
		else if (selectedType == 11) {
			return shared_ptr<tile>(new VerticalLava(worldPos, tex, z));
		}
		else if (selectedType == 12) {
			return shared_ptr<tile>(new PourLava(worldPos, tex, z));
		}
		else if (selectedType == 13) {
			return shared_ptr<tile>(new AnimTile(worldPos, tex, 0, 3, z));
		}
		else if (selectedType == 14) {
			return shared_ptr<tile>(new AnimTile(worldPos, tex, 1, 3, z));
		}
		else if (selectedType == 15) {
			return shared_ptr<tile>(new AnimTile(worldPos, tex, 2, 3, z));
		}
		else if (selectedType == 16) {
			return shared_ptr<tile>(new WaterAnim(worldPos, tex, 3, 3, z));
		}
		else if (selectedType == 17) {
			return shared_ptr<tile>(new WaterAnim(worldPos, tex, 4, 3, z));
			}
		else if (selectedType == 18) {
			return shared_ptr<tile>(new WaterAnim(worldPos, tex, 5, 3, z));
		}
		else if (selectedType == 19) {
			return shared_ptr<tile>(new WaterAnim(worldPos, tex, 6, 3, z));
		}
		else if (selectedType == 20) {
			return shared_ptr<tile>(new WaterTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == 21) {
			return shared_ptr<tile>(new DeathTile(worldPos, tex, selectedTexture, z));
		}
	}

	shared_ptr<tile> metalCheck(Vector2f worldPos) {
		if (selectedType == 10) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 0, 2, z));
		}
		if (selectedType == 11) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 1, 2, z));
		}
		if (selectedType == 12) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 2, 2, z));
		}
		if (selectedType == 13) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 3, 2, z));
		}
		if (selectedType == 14) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 4, 2, z));
		}
		if (selectedType == 15) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 5, 2, z));
		}
		if (selectedType == 16) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 6, 2, z));
		}
		if (selectedType == 17) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 7, 2, z));
		}
		if (selectedType == 18) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 8, 2, z));
		}
		if (selectedType == 19) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 9, 2, z));
		}
		if (selectedType == 20) {
			return shared_ptr<AnimTile>(new AnimTile(worldPos, tex, 10, 2, z));
		}
		if (selectedType == 21) {
			shared_ptr<MoveTile> temp = shared_ptr<MoveTile>(new MoveTile(worldPos, tex));
			temp->setMoveRight(false);
			return temp;
		}
		if (selectedType == 22) {
			shared_ptr<MoveTile> temp = shared_ptr<MoveTile>(new MoveTile(worldPos, tex));
			temp->setMoveRight(true);
			return temp;
		}
		if (selectedType == 23) {
			return shared_ptr<ConveyorTile>(new ConveyorTile(worldPos, tex, z, true));
		}
		if (selectedType == 24) {
			return shared_ptr<ConveyorTile>(new ConveyorTile(worldPos, tex, z, false));
		}
	}



	int beatSet = 0;

	shared_ptr<Text> beatText;

	shared_ptr<tile> flashCheck(Vector2f worldPos) {
		if (selectedType == 0) {
			return shared_ptr<tile>(new tile(worldPos, tex, selectedTexture, z));
		}
		if (selectedType == 1) {
			shared_ptr<FlashTile> t = shared_ptr<TopLeftCornerFlash>(new TopLeftCornerFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}

		if (selectedType == 2) {
			shared_ptr<FlashTile> t = shared_ptr<HTubeTile>(new HTubeTile(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 3) {
			shared_ptr<FlashTile> t = shared_ptr<VTubeTile>(new VTubeTile(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 4) {
			shared_ptr<FlashTile> t = shared_ptr<TopRightCornerFlash>(new TopRightCornerFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 5) {
			shared_ptr<FlashTile> t = shared_ptr<TopEdgeFlash>(new TopEdgeFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 6) {
			shared_ptr<FlashTile> t = shared_ptr<BottomRightCornerFlash>(new BottomRightCornerFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 7) {
			shared_ptr<FlashTile> t = shared_ptr<BottomLeftCornerFlash>(new BottomLeftCornerFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 8) {
			shared_ptr<FlashTile> t = shared_ptr<BottomEdgeFlash>(new BottomEdgeFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 9) {
			shared_ptr<FlashTile> t = shared_ptr<SquareFlash>(new SquareFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 10) {
			shared_ptr<FlashTile> t = shared_ptr<LeftEdgeFlash>(new LeftEdgeFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 11) {
			shared_ptr<FlashTile> t = shared_ptr<RightEdgeFlash>(new RightEdgeFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 12) {
			shared_ptr<FlashTile> t = shared_ptr<TopLeftWFlash>(new TopLeftWFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 13) {
			shared_ptr<FlashTile> t = shared_ptr<TopRightWFlash>(new TopRightWFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 14) {
			shared_ptr<FlashTile> t = shared_ptr<WWallFlash>(new WWallFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 15) {
			shared_ptr<FlashTile> t = shared_ptr<BottomLeftWFlash>(new BottomLeftWFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 16) {
			shared_ptr<FlashTile> t = shared_ptr<BottomRightWFlash>(new BottomRightWFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 17) {
			shared_ptr<FlashTile> t = shared_ptr<BottomLeftCornerWFlash>(new BottomLeftCornerWFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 18) {
			shared_ptr<FlashTile> t = shared_ptr<WCeilingFlash>(new WCeilingFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}
		if (selectedType == 19) {
			shared_ptr<FlashTile> t = shared_ptr<WRoofFlash>(new WRoofFlash(worldPos, tex));
			t->setTiming(beatSet);
			return t;
		}

		if (selectedType == 20) {
			shared_ptr<FlashTile> t =flashTileCheck(worldPos);
			t->setTiming(beatSet);
			return t;
		}

		if (selectedType == 21) {
			return shared_ptr<tile>(new rightTile(worldPos, tex, selectedTexture));
		}

		
	}

	shared_ptr<FlashTile> flashTileCheck(Vector2f pos) {

		bool tileBelow = false;
		bool tileAbove = false;

		bool tileLeft = false;
		bool tileRight = false;

		for (shared_ptr<tile> t : tileList) {
			if (t->getLocation().x == pos.x) {
				if (t->getLocation().y == pos.y + 1) {
					if (t->getTiming() == beatSet) {
						tileBelow = true;
					}
				}
				if (t->getLocation().y == pos.y - 1) {
					if (t->getTiming() == beatSet) {
						tileAbove = true;
					}
				}
			}

			if (t->getLocation().y == pos.y) {
				if (t->getLocation().x == pos.x + 1) {
					if (t->getTiming() == beatSet) {
						tileRight = true;
					}
				}
				if (t->getLocation().x == pos.x - 1) {
					if (t->getTiming() == beatSet) {
						tileLeft = true;
					}
				}
			}
		}

		if (tileRight && tileLeft) {
			return shared_ptr<HTubeTile>(new HTubeTile(pos, tex));
		}

		if (tileAbove && tileBelow) {
			return shared_ptr<VTubeTile>(new VTubeTile(pos, tex));
		}

		if (tileAbove && tileLeft) {
			return shared_ptr<BottomRightCornerFlash>(new BottomRightCornerFlash(pos, tex));
		}

		if (tileAbove && tileRight) {
			return shared_ptr<BottomLeftCornerFlash>(new BottomLeftCornerFlash(pos, tex));
		}

		if (tileBelow && tileLeft) {
			return shared_ptr<TopRightCornerFlash>(new TopRightCornerFlash(pos, tex));
		}

		if (tileBelow && tileRight) {
			return shared_ptr<TopLeftCornerFlash>(new TopLeftCornerFlash(pos, tex));
		}

		if (tileLeft) {
			return shared_ptr<RightEdgeFlash>(new RightEdgeFlash(pos, tex));
		}

		if (tileRight) {
			return shared_ptr<LeftEdgeFlash>(new LeftEdgeFlash(pos, tex));
		}

		if (tileAbove) {
			return shared_ptr<BottomEdgeFlash>(new BottomEdgeFlash(pos, tex));
		}

		if (tileBelow) {
			return shared_ptr<TopEdgeFlash>(new TopEdgeFlash(pos, tex));
		}

		return shared_ptr<SquareFlash>(new SquareFlash(pos, tex));
	}


	
};