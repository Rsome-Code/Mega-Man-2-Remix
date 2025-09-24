#include <list>
#include "Tile.cpp"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "Right tile.cpp"
#include "Left tile.cpp"
#include "Ceiling tile.cpp"
#include "Top Tile.cpp"
#include "Solid Tile.cpp"
#include "ladder tile.cpp"
#include "top ladder tile.cpp"
#include "End Flag.cpp"
#include "Bat.cpp"
#include "big health.cpp"
#include "torch.cpp"
#include "small health.cpp"
#include "big ammo.cpp"
#include "small ammo.cpp"
#include "player.cpp"
#include "E Tank.cpp"
#include "Extra Life.cpp"
#include "door.cpp"
#include "rabbit.cpp"
#include "gorilla.cpp"
#include "wolf.cpp"
#include "spawn area.cpp"
#include "bird spawner.cpp"
#include "chicken spawner.cpp"
#include "spawn point.cpp"
#include "wood man.cpp"

#pragma once

using namespace std;
using namespace sf;

class Load {
	int z = 1;
	Texture* tex;
	Door* door1 = NULL;
	Door* door2 = NULL;


public:
	void load(string levelName, string section, Texture* texture,  list<tile*>* tileList, list<tile*>* z2List, list<tile*>* z3List, list<tile*>* z4List) {
		// Open the input file
		ifstream inputFile(levelName + "\\" + section + ".txt");

		tex = texture;

		string line;
		string variable;
		// Declare a string variable to store each
		// line of the file

		// Read each line of the file and print it to the
		// standard output stream

		while (getline(inputFile, line)) {

			char sep = ',';
			vector<int> values = split(line, sep);
			list<int> val;


			for (auto& i : values) {
				val.push_back(i);
			}
			list<int>::iterator valI = val.begin();

			int type = *valI;
			valI = next(valI);
			int worldX = *valI;
			valI = next(valI);
			int worldY = *valI;
			valI = next(valI);
			int tex = *valI;
			if (next(valI) != val.end()) {
				valI = next(valI);
				z = *valI;
			}
			else {
				z = 1;
			}
			if (z == 1) {
				tileList->push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}
			else if (z == 2) {
				z2List->push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}
			else if (z == 3) {
				z3List->push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}
			else if (z == 4) {
				z4List->push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}
		}
		z = 1;
		// Close the file
		inputFile.close();

	}

	void loadFlags(string levelName, list<EndFlag*>* flags, Texture* t) {
		ifstream inputFile(levelName + "\\flags.txt");
		string line;
		string variable;
		char sep = ',';
		
		while (getline(inputFile, line)) {

			vector<string> current = splitString(line, sep);
			EndFlag* temp;

			vector<string>::iterator valI = current.begin();

			string value = *valI;

			int sect = stoi(value);
			valI = next(valI);
			value = *valI;
			enum transitionAngle angle;
			if (value == "flag") {
				angle = RIGHT;
			}
			else if (value == "flag-down") {
				angle = DOWN;
			}
			else if (value == "flag-up") {
				angle = UP;
			}
			else if (value == "flag-left") {
				angle = LEFT;
			}
			else if (value == "door") {
				angle = RIGHT;
				
			}
			valI = next(valI);

			int xPos = stoi(*valI);
			valI = next(valI);
			int yPos = stoi(*valI);

			valI = next(valI);
			bool checkpoint;
			if (stoi(*valI) == 1){
				checkpoint = true;
			}
			else {
				checkpoint = false;
			}

		

			temp = new EndFlag (t, Vector2f(xPos, yPos), angle, sect);
			if (value == "door") {
				hFlagYPos(temp, flags);
				if (door1 == NULL) {
					door1 = new Door(levelName, Vector2f(xPos, yPos), sect);
					door1->setCheckpoint();
				}
				else {
					door2 = new Door(levelName, Vector2f(xPos, yPos), sect);
				}
			}

			if (checkpoint) {
				temp->setCheckpoint();
			}
		

			flags->push_back(temp);

		}
	}

	Door* getDoor1() {
		return door1;
	}
	Door* getDoor2() {
		return door2;
	}

	EndFlag* getFlag(int sect, list<EndFlag*> flagList) {
		for (EndFlag* flag : flagList) {
			if (flag->getSection() == sect) {
				return flag;
			}
		}
		return NULL;
	}

	void hFlagYPos(EndFlag* thisFlag, list<EndFlag*>* flagList) {

		int sectCheck = thisFlag->getSection() - 1;
		EndFlag* lastVFlag = getFlag(sectCheck, *flagList);
		while (lastVFlag != NULL && lastVFlag->getAngle() != UP && lastVFlag->getAngle() != DOWN) {
			sectCheck -= 1;
			lastVFlag = getFlag(sectCheck, *flagList);

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


	std::vector<std::string> splitString(const std::string& str, char delimiter) {
		std::vector<std::string> tokens;
		std::stringstream ss(str);
		std::string token;

		while (std::getline(ss, token, delimiter)) {
			tokens.push_back(token);
		}

		return tokens;
	}

	void loadSaveFile(player* p) {
		ifstream inputFile("save file.txt");

		string line;


		while (getline(inputFile, line)) {
			char sep = ',';
			vector<string> values = splitString(line, sep);
			list<string> val;
			for (auto& i : values) {
				val.push_back(i);
			}
			list<string>::iterator valI = val.begin();
			string type = *valI;

			valI = next(valI);

			if (type == "lives") {
				p->setLives(stoi(*valI));
			}
			else if (type == "E-tanks") {
				p->setETanks(stoi(*valI));
			}
			else if (type == "Heat Man") {
				if (*valI == "y") {
					p->setAtomicFire(true);
				}
				else {
					p->setAtomicFire(false);
				}
			}

		}


	}
	void loadObjects(string levelName, string section, list<object*>* objects, Texture* t, camera* cam) {

		ifstream inputFile(levelName + "\\" + section + "-objects.txt");



		Texture* misc = new Texture();
		misc->loadFromFile("Assets\\misc\\mega buster.png");

		string line;

		SpawnArea* sArea = NULL;

		while (getline(inputFile, line)) {

			char sep = ',';
			vector<string> values = splitString(line, sep);
			list<string> val;
			for (auto& i : values) {
				val.push_back(i);
			}
			list<string>::iterator valI = val.begin();

			string type = *valI;
			valI = next(valI);
			int worldX = stoi(*valI);
			valI = next(valI);
			int worldY = stoi(*valI);
			valI = next(valI);
			if (next(valI) != val.end()) {
				valI = next(valI);
				z = stoi(*valI);
			}
			object* add = NULL;
			enemy* enem = NULL;

			checkCode(type, t, misc, worldX, worldY, &enem, &add, &sArea);

			if (add != NULL) {
				add->getSprite()->setPosition(Vector2f(worldX, worldY));
				add->setCode();
				add->setDisplay(true);
			}

			if (enem == NULL && add != NULL) {
				add->initial();
				objects->push_back(add);
			}
			else if (!(add ==NULL && enem == NULL)) {
				objects->push_back(enem);
				enem->initial();
			}
		}

		if (sArea != NULL) {
			spawnDeconstruction(sArea, objects, cam);
		}
	}

	void spawnDeconstruction(SpawnArea* spawn, list<object*>* objects, camera* cam) {
		float start = spawn->getStartPos();
		float end = spawn->getEndPos();
		enemy* en = spawn->getEnemy();
		SpawnPoint* startP = new SpawnPoint(string(en->getCode()));
		startP->getSprite()->setPosition(Vector2f(start, cam->getPosition().y));
		SpawnPoint* endP = new SpawnPoint(string(en->getCode()));
		endP->getSprite()->setPosition(Vector2f(end, cam->getPosition().y));
		objects->push_back(startP);
		objects->push_back(endP);
	}
	
	void loadObjects(string levelName, string section, list<object*>* objects, list<enemy*>* enemies, Texture* t, SpawnArea** sArea) {

		ifstream inputFile(levelName + "\\" + section + "-objects.txt");

		Texture* misc = new Texture();
		misc->loadFromFile("Assets\\misc\\mega buster.png");

		string line;

		objects->clear();
		enemies->clear();
		
		while (getline(inputFile, line)) {

			char sep = ',';
			vector<string> values = splitString(line, sep);
			list<string> val;
			for (auto& i : values) {
				val.push_back(i);
			}
			list<string>::iterator valI = val.begin();

			string type = *valI;
			valI = next(valI);
			int worldX = stoi(*valI);
			valI = next(valI);
			int worldY = stoi(*valI);
			valI = next(valI);
			if (next(valI) != val.end()) {
				valI = next(valI);
				z = stoi(*valI);
			}
			object* add = NULL;
			enemy* enem = NULL;

			checkCode(type, t, misc, worldX, worldY, &enem, &add, sArea);
			

			if (add != NULL) {
				add->getSprite()->setPosition(Vector2f(worldX, worldY));
				add->setCode();
				add->setDisplay(true);
				objects->push_back(add);
			}

			if (enem == NULL) {
				
			}
			else {
				enemies->push_back(enem);
			}
		}
	}

	void checkCode(string type, Texture* t, Texture* misc, float worldX, float worldY, enemy** enem, object** add, SpawnArea** spawn) {
		


		if (type == "e1") {
			*enem = new bat(t, Vector2f(worldX, worldY));
		}
		else if (type == "rabbit") {
			*enem = new Rabbit(t, Vector2f(worldX, worldY));
		}
		else if (type == "gorilla") {
			*enem = new Gorilla(t, Vector2f(worldX, worldY));
		}
		else if (type == "wolf") {
			*enem = new Wolf(t, Vector2f(worldX, worldY));
		}
		else if (type == "wood man") {
			Texture* woodBossT = new Texture();
			woodBossT->loadFromFile("assets\\wood man.png");
			*enem = new WoodMan(woodBossT, Vector2f(worldX, worldY));
		}
		else if (type == "bird-spawn") {
			if (*spawn == NULL) {
				*spawn = new BirdSpawner(worldX);
			}
			else {
				SpawnArea* temp = *spawn;
				temp->setEnd(worldX);
				temp->initial();
			}
		}
		else if (type == "chicken-spawn") {
			if (*spawn == NULL) {
				*spawn = new ChickenSpawner(worldX);
			}
			else {
				SpawnArea* temp = *spawn;
				temp->setEnd(worldX);
				temp->initial();
			}
		}
		else if (type == "trch-R") {
			*add = new Torch(t, Vector2f(worldX, worldY), Color::Red, 300, 210);

		}
		/*else if (type == "flag") {
			add = new EndFlag(t, Vector2f(worldX, worldY));
		}
		else if (type == "flag-up") {
			add = new EndFlag(t, Vector2f(worldX, worldY), UP);
		}
		else if (type == "flag-down") {
			add = new EndFlag(t, Vector2f(worldX, worldY), DOWN);
		}*/
		else if (type == "health-big") {
			*add = new BigHealth(misc, Vector2f(worldX, worldY));

		}
		else if (type == "health-small") {
			*add = new SmallHealth(misc, Vector2f(worldX, worldY));

		}
		else if (type == "ammo-big") {
			*add = new BigAmmo(misc, Vector2f(worldX, worldY));

		}
		else if (type == "ammo-small") {
			*add = new SmallAmmo(misc, Vector2f(worldX, worldY));

		}
		else if (type == "E Tank") {
			*add = new ETank(misc, Vector2f(worldX, worldY));

		}
		else if (type == "Extra Life") {
			*add = new ExtraLife(misc, Vector2f(worldX, worldY));

		}
	}

	tile* tileCreation(Vector2f worldPos, int selectedType, int selectedTexture) {

		//Instead of using sub-classes to store different types of collision,
		//I should have just used one tile class and activated different collision using functions.
		//That would have been a way to avoid the magic numbering you see below.

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
	vector<int> split(const string& str, char sep)
	{
		vector<int> tokens;

		int i;
		stringstream ss(str);
		while (ss >> i) {
			tokens.push_back(i);
			if (ss.peek() == sep) {
				ss.ignore();
			}
		}

		return tokens;
	}
};