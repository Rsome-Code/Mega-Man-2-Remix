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
#pragma once

class abstractStage {
protected:
	list<tile*> tileList;
	list<tile*> z2List;
	list<tile*> z3List;
	list<tile*> z4List;

	string levelName;

	list<object*> objects;
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
			if (valI != val.end()) {
				z = *valI;
			}
			else {
				z = 1;
			}



			if (z == 1) {
				tileList.push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}
			else if (z == 2) {
				z2List.push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}
			else if (z == 3) {
				z3List.push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}
			else if (z == 4) {
				z4List.push_back(tileCreation(Vector2f(worldX, worldY), type, tex));
			}

		}

		// Close the file
		inputFile.close();
		zCorrection();

		Load* load = new Load();
		load->loadObjects(levelName, &objects, enemyTexture);

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