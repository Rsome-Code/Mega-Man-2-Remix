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
#pragma once

class abstractStage {
protected:
	list<tile*> tileList;
	list<tile*> bTList;
	list<enemy*> enemyList;
	list<transition*> tList;
	Texture* tileTexture;
	Texture* enemyTexture;
	Vector2f initialCamera;
	Vector2f initalPlayer;

	float z = 1;
	

public:


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
		enemyList.push_back(e);
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

	Vector2f getInitialCamera() {
		return initialCamera;
	}
	Vector2f getInitialPlayer() {
		return initalPlayer;
	}

	list<tile*> getTiles() {
		return tileList;
	}
	list<enemy*> getEnemies() {
		return enemyList;
	}
	list<transition*> getTList() {
		return tList;
	}

	void load() {
		// Open the input file named "input.txt"
		ifstream inputFile("myfile.txt");

		// Check if the file is successfully opened


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
			valI = next(valI);
			z = *valI;



			if (z == 1) {
				tileList.push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}
			else {
				bTList.push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}

		}

		// Close the file
		inputFile.close();
		zCorrection();

	}

	void zCorrection() {
		for (tile* t : bTList) {
			t->getSprite()->setZ(1.25);
		}
	}

	list<tile*> getBList() {
		return bTList;
	}

	/*void load() {
		// Open the input file named "input.txt"
		ifstream inputFile("myfile.txt");

		// Check if the file is successfully opened


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




			tileList.push_back(tileCreation(Vector2f(worldX, worldY), type, tex));

		}

		// Close the file
		inputFile.close();

	}*/
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