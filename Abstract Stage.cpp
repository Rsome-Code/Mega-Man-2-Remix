#include "tile.cpp"
#include "enemy.cpp"
#include "Screen Transition.cpp"
#include "Right tile.cpp"
#include "Left tile.cpp"
#include "Ceiling tile.cpp"
#include "Top Tile.cpp"
#include "Solid Tile.cpp"
#include "ladder tile.cpp"
#include "top ladder tile.cpp"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "load.cpp"
#include "door.cpp"
#include "item.cpp"
#include "sound collection.cpp"
#include <SFML/audio.hpp>
#pragma once

class abstractStage {
protected:
	list<shared_ptr<tile>> tileList;
	list<shared_ptr<tile>> z2List;
	list<shared_ptr<tile>> z3List;
	list<shared_ptr<tile>> z4List;

	string levelName;

	//list<shared_ptr<GameObject>> objects;

	list<shared_ptr<GameObject>> objects;
	list<shared_ptr<GameObject>> backgroundObjects;
	list<shared_ptr<GameObject>> foregroundObjects;
	list<shared_ptr<enemy>> enemies;
	list<shared_ptr<Spawner>> spawners;
	list<shared_ptr<Item>> items;

	shared_ptr<SpawnArea> spawn = NULL;


	//This should have been a vector
	list<transition*> tList;
	shared_ptr<Texture> tileTexture;
	shared_ptr<Texture> enemyTexture;
	Vector2f initialCamera;
	Vector2f initalPlayer;
	shared_ptr<Texture> bossTexture;
	
	Vector2f flagPos;
	Vector2f lastFlagPos;
	enum transitionAngle transAngle;
	enum transitionAngle lastAngle;

	list<shared_ptr<EndFlag>> flags;
	
	float z = 1;

	shared_ptr<Door> door1;
	shared_ptr<Door> door2;

	shared_ptr<Load> l;
	shared_ptr<Music> music;
	

public:

	virtual ~abstractStage() {


		tList.clear();

		

		//delete door1;
		//delete door2;


		
		//delete music;
	}

	abstractStage(string name, shared_ptr<SoundCollection> soundCol) {
		l = shared_ptr<Load>(new Load());
		levelName = name;
		tileTexture = shared_ptr<Texture> (new Texture());
		tileTexture->loadFromFile("Assets\\stage\\" + name + ".png");
		setInitialPlayer(Vector2f((8 * 4) * 16, (13 * 4) * 16));
		setInitialCamera(Vector2f(12 * 4 * 16, (2 * 4) * 16));
		bossTexture = shared_ptr<Texture> (new Texture());
		bossTexture->loadFromFile("Assets\\" + name + ".png");

		enemyTexture = shared_ptr<Texture> (new Texture());
		enemyTexture->loadFromFile("Assets\\enemy.png");

		loadFlags(name);
		lastFlagPos = Vector2f(0,0);


		music = shared_ptr<Music>(new Music());

		music->openFromFile("assets\\sound\\music\\" + name + ".mp3");

		setMusicLoop();
		
	}

	void setMusicLoop() {
		music->setLoop(true);
		if (levelName == "wood man"){
			music->setLoopPoints({ sf::seconds(6.5), seconds(100) });
		}
		else if (levelName == "flash man") {
			music->setLoopPoints({ sf::seconds(25.5629), seconds(1000) });
		}
		else if (levelName == "crash man") {
			music->setLoopPoints({ sf::seconds(12.77), seconds(1000) });
		}
		else {
			music->setLoopPoints({ sf::seconds(0), seconds(1000) });
		}
	}

protected:
	/*void addMultiTile(multiTile* t) {
		for (shared_ptr<tile> tile : *t->getTiles()) {
			tileList.push_back(tile);
		}
	}

	void addLadder(ladder* l) {
		addMultiTile(l->getMulti());
		tileList.push_back(l->getTop());
	}*/

	void addEnemy(shared_ptr<enemy> e) {
		enemies.push_back(e);
	}

	void addTransition(transition* t) {
		tList.push_back(t);
	}
	void setInitialCamera(Vector2f v) {
		initialCamera = v;
	}
	void setInitialPlayer(Vector2f v) {
		initalPlayer = v;
	}

public:

	shared_ptr<Music> getMusic() {
		return music;
	}

	void reload(string name, string section, shared_ptr<SoundCollection> soundCol) {
		
		tileList.clear();

		z2List.clear();
		z3List.clear();
		z4List.clear();
		objects.clear();
		backgroundObjects.clear();
		foregroundObjects.clear();
		spawners.clear();
		items.clear();
		//flags.clear();
		load(name, section, soundCol);
	}

	string getName() {
		return levelName;
	}

	Vector2f getFlagPos(int section) {
		for (shared_ptr<EndFlag> flag : flags) {
			if (flag->getSection() == section) {
				return flag->getSprite()->getPosition();
			}
		}
	}

	//Only last placed flag is counted for each section
	void addEndFlag(shared_ptr<EndFlag> flag) {
		
		//lastFlagPos = flagPos;
		

		
		flagPos = flag->getSprite()->getPosition();
		transAngle = flag->getAngle();
			
		

	}

	void updateSection(int section) {
		for (shared_ptr<EndFlag> flag : flags) {
			if (flag->getSection() == section - 1) {
				lastFlagPos = flag->getSprite()->getPosition();
			}
			else if (flag->getSection() == section) {
				flagPos = flag->getSprite()->getPosition();
			}
		}

		
	}

	shared_ptr<EndFlag> getLastFlag(int section) {
		for (shared_ptr<EndFlag> flag : flags) {
			if (flag->getSection() == section - 1) {
				
				return flag;
			}
		}
		return NULL;
	}
	shared_ptr<EndFlag> getCurrentFlag(int section) {
		for (shared_ptr<EndFlag> flag : flags) {
			if (flag->getSection() == section) {
				return flag;
			}
		}
		return NULL;
	}

	Vector2f getLastFlagPos() {
		return lastFlagPos;
	}

	Vector2f getLastFlagPos(int section) {
		for (shared_ptr<EndFlag> flag : flags) {
			if (flag->getSection() == section - 1) {

				return flag->getSprite()->getPosition();
			}
		}
	}


	enum transitionAngle getAngle() {
		return transAngle;
	}

	Vector2f getInitialCamera() {
		return initialCamera;
	}
	Vector2f getInitialPlayer() {
		return initalPlayer;
	}

	list<shared_ptr<tile>> getTiles() {
		return tileList;
	}
	list<shared_ptr<GameObject>> getObjects() {
		return objects;
	}
	list<shared_ptr<enemy>> getEnemies() {
		return enemies;
	}
	list<transition*> getTList() {
		return tList;
	}

	list<shared_ptr<Item>> getItems() {
		return items;
	}

	void load(string name, string section, shared_ptr<SoundCollection> soundCol) {

		if (section == "3") {
			cout << "here";
		}
		
		l->load(name, section, tileTexture, &tileList, &z2List, &z3List, &z4List);
		zCorrection();
		
		spawn = NULL;

		l->loadObjects(name, section, &objects, &backgroundObjects, &foregroundObjects, &enemies, enemyTexture, &spawn, &spawners, &items, soundCol);

		checkTilesInObjects();

		if (flags.size() == 0) {
			l->loadFlags(name, &flags, enemyTexture);
		}

		door1 = l->getDoor1();
		door2 = l->getDoor2();
		

	}

	void loadFlags(string name) {
		l->loadFlags(name, &flags, enemyTexture);
		door1 = l->getDoor1();
		door2 = l->getDoor2();
	}


	void checkTilesInObjects() {
		for (shared_ptr<GameObject> o : objects) {
			for (shared_ptr<tile> t : o->getTiles()) {
				tileList.push_back(t);
			}
			
		}
	}

	shared_ptr<Door> getDoor1() {
		return door1;
	}
	shared_ptr<Door> getDoor2() {
		return door2;
	}

	shared_ptr<SpawnArea> getAreaSpawner() {
		return spawn;
	}
	list<shared_ptr<Spawner>> getSpawners() {
		return spawners;
	}

	shared_ptr<EndFlag> getLastCheckpoint(int sect) {
		bool found = true;
		int current = sect;
		shared_ptr<EndFlag> thisFlag = NULL;
		while (thisFlag == NULL) {
			current -= 1;
			found = true;
			while (found) {
				found = false;
				for (shared_ptr<EndFlag> flag : flags) {
					if (flag->getSection() == current) {
						found = true;
						if (flag->getCheckpoint()) {
							return flag;
						}
					}
				}
				current -= 1;

			}
		}
		return NULL;

	}

	list<shared_ptr<EndFlag>> getFlags() {
		return flags;
	}

	void zCorrection() {
		for (shared_ptr<tile> t : z2List) {
			t->getSprite()->setZ(1.25);
		}
		for (shared_ptr<tile> t : z3List) {
			t->getSprite()->setZ(1.5);
		}
		for (shared_ptr<tile> t : z4List) {
			t->getSprite()->setZ(1.75);
		}

	}

	list<shared_ptr<tile>> getZ2List() {
		return z2List;
	}
	list<shared_ptr<tile>> getZ3List() {
		return z3List;
	}
	list<shared_ptr<tile>> getZ4List() {
		return z4List;
	}

	list<shared_ptr<GameObject>> getBackgroundObjects() {
		return backgroundObjects;
	}

	list<shared_ptr<GameObject>> getForegroundObjects() {
		return foregroundObjects;
	}

	shared_ptr<tile> tileCreation(Vector2f worldPos, int selectedType, int selectedTexture) {
		if (selectedType == 0) {
			return shared_ptr<tile>(new tile(worldPos, tileTexture, selectedTexture, z));
		}
		else if (selectedType == 1) {
			return shared_ptr<tile>(new topTile(worldPos, tileTexture, selectedTexture));
		}
		else if (selectedType == 2) {
			return shared_ptr<tile>(new rightTile(worldPos, tileTexture, selectedTexture));
		}
		else if (selectedType == 3) {
			return shared_ptr<tile>(new ceilingTile(worldPos, tileTexture, selectedTexture));
		}
		else if (selectedType == 4) {
			return shared_ptr<tile>(new leftTile(worldPos, tileTexture, selectedTexture));
		}
		else if (selectedType == 8) {
			return shared_ptr<tile>(new ladderTile(worldPos, tileTexture));
		}
		else if (selectedType == 9) {
			return shared_ptr<tile>(new topLadder(worldPos, tileTexture));
		}
		else {
			return shared_ptr<tile>(new solidTile(worldPos, tileTexture, selectedTexture));
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

	int getTelePos() {
		if (levelName == "crash man") {
			return 700;
		}
		else {
			return 300;
		}
	}
};