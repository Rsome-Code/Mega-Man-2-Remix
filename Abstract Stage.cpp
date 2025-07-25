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
#pragma once

class abstractStage {
protected:
	list<tile*> tileList;
	list<tile*> z2List;
	list<tile*> z3List;
	list<tile*> z4List;

	string levelName;

	list<object*> objects;

	//This should have been a vector
	list<transition*> tList;
	Texture* tileTexture;
	Texture* enemyTexture;
	Vector2f initialCamera;
	Vector2f initalPlayer;
	Texture* bossTexture;
	
	Vector2f flagPos;
	Vector2f lastFlagPos;
	enum transitionAngle transAngle;
	enum transitionAngle lastAngle;

	list<EndFlag*> flags;
	

	float z = 1;

	Door* door1;
	Door* door2;

	Load* l;
	

public:
	abstractStage(string name) {
		l = new Load();
		levelName = name;
		tileTexture = new Texture();
		tileTexture->loadFromFile("Assets\\" + name + "-stage.png");
		setInitialPlayer(Vector2f((8 * 4) * 16, (13 * 4) * 16));
		setInitialCamera(Vector2f(12 * 4 * 16, (2 * 4) * 16));
		bossTexture = new Texture();
		bossTexture->loadFromFile("Assets\\" + name + ".png");

		enemyTexture = new Texture();
		enemyTexture->loadFromFile("Assets\\enemy.png");

		load(name);
		lastFlagPos = Vector2f(0,0);

		

		
	}

protected:
	/*void addMultiTile(multiTile* t) {
		for (tile* tile : *t->getTiles()) {
			tileList.push_back(tile);
		}
	}

	void addLadder(ladder* l) {
		addMultiTile(l->getMulti());
		tileList.push_back(l->getTop());
	}*/

	void addEnemy(enemy* e) {
		objects.push_back(e);
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

	void reload(string name) {
		tileList.clear();
		z2List.clear();
		z3List.clear();
		z4List.clear();
		objects.clear();
		//flags.clear();
		load(name);
	}

	string getName() {
		return levelName;
	}

	Vector2f getFlagPos(int section) {
		for (EndFlag* flag : flags) {
			if (flag->getSection() == section) {
				return flag->getSprite()->getPosition();
			}
		}
	}

	//Only last placed flag is counted for each section
	void addEndFlag(EndFlag* flag) {
		
		//lastFlagPos = flagPos;
		

		
		flagPos = flag->getSprite()->getPosition();
		transAngle = flag->getAngle();
			
		

	}

	void updateSection(int section) {
		for (EndFlag* flag : flags) {
			if (flag->getSection() == section - 1) {
				lastFlagPos = flag->getSprite()->getPosition();
			}
			else if (flag->getSection() == section) {
				flagPos = flag->getSprite()->getPosition();
			}
		}

		
	}

	EndFlag* getLastFlag(int section) {
		for (EndFlag* flag : flags) {
			if (flag->getSection() == section - 1) {
				
				return flag;
			}
		}
		return NULL;
	}
	EndFlag* getCurrentFlag(int section) {
		for (EndFlag* flag : flags) {
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
		for (EndFlag* flag : flags) {
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

	list<tile*> getTiles() {
		return tileList;
	}
	list<object*> getObjects() {
		return objects;
	}
	list<transition*> getTList() {
		return tList;
	}

	void load(string name) {
		
		l->load(name, tileTexture, &tileList, &z2List, &z3List, &z4List);
		zCorrection();
		
		l->loadObjects(name, &objects, enemyTexture);

		if (flags.size() == 0) {
			l->loadFlags(name, &flags, enemyTexture);
		}

		door1 = l->getDoor1();
		door2 = l->getDoor2();
		

	}

	Door* getDoor1() {
		return door1;
	}
	Door* getDoor2() {
		return door2;
	}

	EndFlag* getLastCheckpoint(int sect) {
		bool found = true;
		int current = sect - 1;
		while (found) {
			found = false;
			for (EndFlag* flag : flags) {
				if (flag->getSection() == current) {
					found = true;
					if (flag->getCheckpoint()) {
						return flag;
					}
				}
			}
			current -= 1;
		}
		return NULL;

	}

	list<EndFlag*> getFlags() {
		return flags;
	}

	void zCorrection() {
		for (tile* t : z2List) {
			t->getSprite()->setZ(1.25);
		}
		for (tile* t : z3List) {
			t->getSprite()->setZ(1.5);
		}
		for (tile* t : z4List) {
			t->getSprite()->setZ(1.75);
		}

	}

	list<tile*> getZ2List() {
		return z2List;
	}
	list<tile*> getZ3List() {
		return z3List;
	}
	list<tile*> getZ4List() {
		return z4List;
	}

	tile* tileCreation(Vector2f worldPos, int selectedType, int selectedTexture) {
		if (selectedType == 0) {
			return new tile(worldPos, tileTexture, selectedTexture, z);
		}
		else if (selectedType == 1) {
			return new topTile(worldPos, tileTexture, selectedTexture);
		}
		else if (selectedType == 2) {
			return new rightTile(worldPos, tileTexture, selectedTexture);
		}
		else if (selectedType == 3) {
			return new ceilingTile(worldPos, tileTexture, selectedTexture);
		}
		else if (selectedType == 4) {
			return new leftTile(worldPos, tileTexture, selectedTexture);
		}
		else if (selectedType == 8) {
			return new ladderTile(worldPos, tileTexture);
		}
		else if (selectedType == 9) {
			return new topLadder(worldPos, tileTexture);
		}
		else {
			return new solidTile(worldPos, tileTexture, selectedTexture);
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