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
#include "Background.cpp"
#include "horizontal lava.cpp"
#include "vertical lava.cpp"
#include "pour lava.cpp"
#include "disappearing tile.cpp"
#include "flyguy.cpp"
#include "fly guy spawn.cpp"
#include "telly spawner.cpp"
#include "springer.cpp"
#include "break wall.cpp"
#include "sniper armour.cpp"
#include "heat man.cpp"
#include "sound collection.cpp"
#include "anim tile.cpp"
#include "water anim.cpp"
#include "water tile.cpp"
#include "falling platform.cpp"
#include "crabbot.cpp"
#include "croaker.cpp"
#include "jelly spawner.cpp"
#include "shrink.cpp"
#include "snapper spawn.cpp"
#include "anko.cpp"
#include "conveyor.cpp"
#include "bubble man.cpp"
#include "move tile.cpp"
#include "press.cpp"
#include "drill spawner.cpp"
#include "blocky.cpp"
#include "pie robot.cpp"
#include "metal man.cpp"
#include "beam collection.cpp"
#include "beam left.cpp"
#include "beam right.cpp"
#include "torch guy.cpp"
#include "lighting rectangle.cpp"
#include "scworm spawner.cpp"
#include "quick man.cpp"
#pragma once

using namespace std;
using namespace sf;

class Load {
	int z = 1;
	Texture* tex;
	Door* door1 = NULL;
	Door* door2 = NULL;
	//Sound* colSound;
	//Sound* yoSound;
	//Sound* hitSound;


public:

	vector<string> sSplit(string str, char del) {
		stringstream ss(str);

		// Temporary object to store 
		// the splitted string
		string t;



		vector<string> temp;
		while (getline(ss, t, del)) {
			temp.push_back(t);
		}
		return temp;
	}

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
			vector<string> values = sSplit(line, ',');
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
			int tex = stoi(*valI);
			if (next(valI) != val.end()) {
				valI = next(valI);
				z = stoi(*valI);
			}
			else {
				z = 1;
			}
			if (z == 1) {
				tileList->push_back(tileCreation(Vector2f(worldX, worldY), type, tex));

				if (*prev(tileList->end()) == NULL) {
					cout << "here";
				}
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
		float lastX = NULL;
		transitionAngle lastAngle = RIGHT;
		
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

		upDownConsistency(flags);
	}

	void upDownConsistency(list<EndFlag*>* flags) {

		for (int i = 0; i < (flags->size()); i++) {
			EndFlag* flag = getFlag(i-1, *flags);
			if (flag != NULL) {
				if (getFlag(flag->getSection() - 1, *flags) != NULL) {
					EndFlag* lastFlag = getFlag(flag->getSection() - 1, *flags);
					if (lastFlag->getAngle() == UP || lastFlag->getAngle() == DOWN) {
						if (fabs(flag->getPosition().x - lastFlag->getPosition().x) < 1920 && !((lastFlag->getAngle() == UP || lastFlag->getAngle() == DOWN) && flag->getAngle() == RIGHT)) {
							if (flag->getPosition().x > lastFlag->getPosition().x) {
								flag->getSprite()->setPosition(Vector2f(lastFlag->getPosition().x + 1920, flag->getPosition().y));
							}
							else {
								flag->getSprite()->setPosition(Vector2f(lastFlag->getPosition().x - 1920, flag->getPosition().y));
							}

							
						}
					}

					//Keeps distance of upwards and downwards sections consistent
					if (flag->getAngle() == UP && lastFlag->getAngle() == UP) {
						flag->getSprite()->setPosition(Vector2f(flag->getPosition().x, lastFlag->getPosition().y - 1030));
					}
					else if (flag->getAngle() == DOWN && lastFlag->getAngle() == DOWN) {
						flag->getSprite()->setPosition(Vector2f(flag->getPosition().x, lastFlag->getPosition().y + 1030));
					}


					if (flag->getAngle() == RIGHT || flag->getAngle() == LEFT) {
						flag->setPosition(Vector2f(flag->getPosition().x, lastFlag->getPosition().y));
					}
				}
			}
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
			else if (type == "Bubble Man") {
				if (*valI == "y") {
					p->setBubbleLead(true);
				}
				else {
					p->setBubbleLead(false);
				}
			}
			else if (type == "Metal Man") {
				if (*valI == "y") {
					p->setMetalBlade(true);
				}
				else {
					p->setMetalBlade(false);
				}
			}
			else if (type == "Quick Man") {
				if (*valI == "y") {
					p->setBoomerang(true);
				}
				else {
					p->setBoomerang(false);
				}
			}

			else if (type == "Flash Man") {
				p->setTimeStopper(*valI == "y");
			}

		}


	}

	//This function is for loading objects into the object placer
	void loadObjects(string levelName, string section, list<GameObject*>* objects, Texture* t, camera* cam) {

		ifstream inputFile(levelName + "\\" + section + "-objects.txt");



		Texture* misc = new Texture();
		misc->loadFromFile("Assets\\misc\\mega buster.png");

		Texture* beamT = new Texture();
		beamT->loadFromFile("assets\\beam.png");

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
			GameObject* add = NULL;
			GameObject* backAdd = NULL;
			GameObject* foreAdd = NULL;
			enemy* enem = NULL;
			Spawner* spawnAdd = NULL;
			Item* item = NULL;

			SoundCollection* soundCol = new SoundCollection();

			BeamCollection* beamCol = NULL;

			checkCode(type, t, misc, beamT, worldX, worldY, &enem, &add, &backAdd, &foreAdd, &sArea, &spawnAdd, &item, soundCol, &beamCol, objects);

			delete soundCol;

			objects->remove(beamCol);
			delete beamCol;

			if (add != NULL) {
				add->getSprite()->setPosition(Vector2f(worldX, worldY));
				add->setCode();
				add->setDisplay(true);
			}

			if (enem == NULL && add != NULL) {
				add->initial();
				objects->push_back(add);
			}
			if (add ==NULL && enem != NULL) {
				objects->push_back(enem);
				enem->initial();
			}

			if (backAdd != NULL) {
				objects->push_back(backAdd);
			}
			if (foreAdd != NULL) {
				objects->push_back(foreAdd);
			}
			if (spawnAdd != NULL) {
				objects->push_back(spawnAdd);
			}
			if (item != NULL) {
				objects->push_back(item);
			}
		}

		if (sArea != NULL) {
			spawnDeconstruction(sArea, objects, cam);
		}
	}

	void spawnDeconstruction(SpawnArea* spawn, list<GameObject*>* objects, camera* cam) {
		float start = spawn->getStartPos();
		float end = spawn->getEndPos();
		enemy* en = spawn->getEnemy();
		en->initial();
		SpawnPoint* startP = new SpawnPoint(string(en->getCode()));
		startP->getSprite()->setPosition(Vector2f(start, cam->getPosition().y));
		SpawnPoint* endP = new SpawnPoint(string(en->getCode()));
		endP->getSprite()->setPosition(Vector2f(end, cam->getPosition().y));
		objects->push_back(startP);
		objects->push_back(endP);
	}
	

	//This function is for loading objects into the game
	void loadObjects(string levelName, string section, list<GameObject*>* objects, list<GameObject*>* backgroundOb, list<GameObject*>* foregroundOb, list<enemy*>* enemies, Texture* t, SpawnArea** sArea, list<Spawner*>* spawners, list<Item*>* items, SoundCollection* soundCol) {

		ifstream inputFile(levelName + "\\" + section + "-objects.txt");

		Texture* misc = new Texture();
		misc->loadFromFile("Assets\\misc\\mega buster.png");

		Texture* beamT = new Texture();
		beamT->loadFromFile("assets\\beam.png");

		string line;

		objects->clear();
		enemies->clear();

		BeamCollection* beamCol = NULL;
		
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
			GameObject* add = NULL;
			GameObject* backAdd = NULL;
			GameObject* foreAdd = NULL;
			enemy* enem = NULL;
			Spawner* spawnAdd = NULL;
			Item* item = NULL;

			checkCode(type, t, misc, beamT,  worldX, worldY, &enem, &add, &backAdd, &foreAdd, sArea, &spawnAdd, &item, soundCol, &beamCol, objects);
			

			if (add != NULL) {
				add->getSprite()->setPosition(Vector2f(worldX, worldY));
				add->setCode();
				add->setDisplay(true);
				objects->push_back(add);
			}

			if (enem != NULL) {
				enemies->push_back(enem);
			}
			
			if (backAdd != NULL) {
				backgroundOb->push_back(backAdd);
			}
			else if (foreAdd != NULL) {
				foregroundOb->push_back(foreAdd);
			}
			else if (spawnAdd != NULL) {
				spawners->push_back(spawnAdd);
			}
			else if (item != NULL) {
				items->push_back(item);
			}
		}

		setSounds(objects, backgroundOb, enemies, items, soundCol);


	}

	void collectionCheck(Beam* en, list<GameObject*>* objects, BeamCollection** beamCol) {

		//if (en->getCode() == "beam left" || en->getCode() == "beam right") {
			if (*beamCol == NULL) {
				*beamCol = new BeamCollection();
				objects->push_back(*beamCol);
			}
			BeamCollection* temp = *beamCol;
			temp->addBeam(en);
		//}
		
	}

	void setSounds(list<GameObject*>* objects, list<GameObject*>* backgroundOb, list<enemy*>* enemies, list<Item*>* items, SoundCollection* soundCol) {

		//Sound* hitSound = soundCol->getHit();

		for (GameObject* ob : *objects) {

			objectCheck(ob, soundCol);
			
		}

		for (enemy* e : *enemies) {
			e->loadSound(soundCol);
			e->setHitSound(soundCol->getHit());

			if (e->getCode() == "press") {
				e->setSound(soundCol->getPress());
			}
		}

		/*SoundBuffer* colB = new SoundBuffer();
		colB->loadFromFile("assets\\sound\\land.wav");

		try {
			// Code that might throw an exception
			free(colSound);
		}
		catch (int e) {
			// exception handling code
		}
		/*Sound* colSound = (Sound*)malloc(1);
		colSound = new Sound();
		colSound->setBuffer(*colB);
		*/
		for (Item* item : *items) {
			if (item->getCode() == "Extra Life" || item->getCode() == "E Tank") {
				item->setSoundPointer(soundCol->getLifeGet());
			}
		}

	}

	void objectCheck(GameObject* ob, SoundCollection* soundCol) {
		string code = ob->getCode();

		if (code == "beam collection") {
		
			ob->setSoundPointer(soundCol->getBeam());

		}
		else {
			vector<string> spl = splitString(code, char('-'));
			string altType = spl[0];
			if (altType == "disappearing tile") {
				//ob->setSoundB(yokuB);
				ob->setSoundPointer(soundCol->getYoku());
			}
		}
	}

	void checkCode(string type, Texture* t, Texture* misc, Texture* beamT, float worldX, float worldY, enemy** enem, GameObject** add, GameObject** backAdd, GameObject** foreAdd, SpawnArea** spawn, Spawner** spawnAdd, Item** item, SoundCollection* soundCol, BeamCollection** beamCol, list<GameObject*>* objects) {

		Vector2f worldPos = Vector2f(worldX, worldY);

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
			*enem = new WoodMan(Vector2f(worldX, worldY));
		}
		else if (type == "heat man") {
			*enem = new HeatMan(Vector2f(worldX, worldY));
		}
		else if (type == "bubble man") {
			*enem = new BubbleMan(Vector2f(worldX, worldY));
		}
		else if (type == "metal man") {
			*enem = new MetalMan(worldPos);
		}
		else if (type == "quick man") {
			*enem = new QuickMan(worldPos);
		}
		else if (type == "fly guy") {
			*enem = new FlyGuy(t, Vector2f(worldX, worldY));
		}
		else if (type == "springer") {
			*enem = new Springer(t, Vector2f(worldX, worldY));
		}
		else if (type == "break wall") {
			*enem = new BreakWall(t, Vector2f(worldX, worldY));
		}
		else if (type == "sniper joe") {
			*enem = new SniperJoe(t, Vector2f(worldX, worldY));
		}
		else if (type == "sniper armour") {
			*enem = new SniperArmour(t, Vector2f(worldX, worldY));
		}
		else if (type == "crabbot") {
			*enem = new Crabbot(t, Vector2f(worldX, worldY));
		}
		else if (type == "croaker") {
			*enem = new Croaker(t, Vector2f(worldX, worldY));
		}
		else if (type == "shrink") {
			*enem = new Shrink(t, Vector2f(worldX, worldY));
		}
		else if (type == "anko") {
			*enem = new Anko(t, Vector2f(worldX, worldY));
		}
		else if (type == "press") {
			*enem = new Press(t, worldPos);
			enemy* temp = *enem;
			temp->setCode();
		}

		else if (type == "blocky") {
			*enem = new Blocky(t, worldPos);
		}

		else if (type == "pie robot") {
			*enem = new PieRobot(t, worldPos);
		}

		else if (type == "beam left") {
			BeamLeft* b = new BeamLeft(beamT, worldPos);
			collectionCheck(b, objects, beamCol);
			*enem = b;
		}
		else if (type == "beam right") {
			BeamRight* b = new BeamRight(beamT, worldPos);
			collectionCheck(b, objects, beamCol);
			*enem = b;
		}

		else if (type == "scworm spawn") {
			*enem = new ScwormSpawn(t, worldPos);
		}

		else if (type == "torch guy") {
			*enem = new TorchGuy(t, worldPos);
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

		else if (type == "jelly fish-spawn") {
			if (*spawn == NULL) {
				*spawn = new JellyFishSpawner(worldX);
			}
			else {
				SpawnArea* temp = *spawn;
				temp->setEnd(worldX);
				temp->initial();
			}
		}

		else if (type == "snapper-spawn") {
			if (*spawn == NULL) {
				*spawn = new SnapperSpawn(worldX);
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
		else if (type == "drill-spawn") {
			if (*spawn == NULL) {
				*spawn = new DrillSpawner(worldX);
			}
			else {
				SpawnArea* temp = *spawn;
				temp->setEnd(worldX);
				temp->initial();
			}
		}

		else if (type == "fall platform") {
			*add = new FallPlatform(t, Vector2f(worldX, worldY));
		}

		else if (type == "lighting rect") {
			*add = new LightingRect();
		}

		else if (type == "fly guy-spawn") {
			*spawnAdd = new FlyGuySpawner(t, Vector2f(worldX, worldY));
		}
		else if (type == "telly-spawn") {
			*spawnAdd = new TellySpawner(t, Vector2f(worldX, worldY));
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
			*item = new BigHealth(misc, Vector2f(worldX, worldY));

		}
		else if (type == "health-small") {
			*item = new SmallHealth(misc, Vector2f(worldX, worldY));

		}
		else if (type == "ammo-big") {
			*item = new BigAmmo(misc, Vector2f(worldX, worldY));

		}
		else if (type == "ammo-small") {
			*item = new SmallAmmo(misc, Vector2f(worldX, worldY));

		}
		else if (type == "E Tank") {
			*item = new ETank(misc, Vector2f(worldX, worldY));

		}
		else if (type == "Extra Life") {
			*item = new ExtraLife(misc, Vector2f(worldX, worldY));

		}

		//Checks variable codes
		else {
			vector<string> spl = splitString(type, char('-'));
			string altType = spl[0];
			if (altType == "background") {
				*backAdd = new Background(stoi(spl[1]));
			}
			if (altType == "disappearing tile") {
				*add = new DisappearingTile(t, Vector2f(worldX, worldY), stoi(spl[1]));
			}
		}
	}


	tile* tileCreation(Vector2f worldPos, string selectedType, int selectedTexture) {

		//Instead of using sub-classes to store different types of collision,
		//I should have just used one tile class and activated different collision using functions.
		//That would have been a way to avoid the magic numbering you see below.

		if (selectedType == "0") {
			return new tile(worldPos, tex, selectedTexture, z);
		}
		else if (selectedType == "1") {
			return new topTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == "2") {
			return new rightTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == "3") {
			return new ceilingTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == "4") {
			return new leftTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == "8") {
			return new ladderTile(worldPos, tex);
		}
		else if (selectedType == "9") {
			return new topLadder(worldPos, tex);
		}
		else if (selectedType == "10") {
			return new HorizontalLava(worldPos, tex, z);
		}
		else if (selectedType == "11") {
			return new VerticalLava(worldPos, tex, z);
		}
		else if (selectedType == "12") {
			return new PourLava(worldPos, tex, z);
		}
		else if (selectedType == "5" || selectedType == "6" || selectedType == "7") {
			return new solidTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == "water") {
			return new WaterTile(worldPos, tex, selectedTexture);
		}
		else if (selectedType == "death") {
			return new DeathTile(worldPos, tex, selectedTexture, z);
		}
		
		else {
			vector<string> spl = splitString(selectedType, char('-'));
			string altType = spl[0];

			if (altType == "0"){
				return new AnimTile(worldPos, tex, stoi(spl[1]), stoi(spl[2]), z);
			}
			else if (altType == "water") {
				return new WaterAnim(worldPos, tex, stoi(spl[1]), 3, z);
			}
			else if (altType == "conveyor anim") {
				return new ConveyorTile(worldPos, tex, z, spl[1] == "right");
			}
			else if (altType == "move") {
				MoveTile* temp = new MoveTile(worldPos, tex);
				temp->setMoveRight(spl[1] == "right");
				return temp;
			}
		}
	}
	vector<string> split(const string& str, char sep)
	{
		vector<string> tokens;

		string i;
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