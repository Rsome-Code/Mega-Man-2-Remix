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
#include "flash man.cpp"
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
#include "flash door.cpp"
#include "crazy cannon.cpp"
#include "checkDown.cpp"
#include "checkUp.cpp"
#include "checkLeft.cpp"
#include "checkRight.cpp"
#include "rail platform.cpp"
#include "crash man.cpp"
#include "neo mettool.cpp"
#pragma once

using namespace std;
using namespace sf;

class Load {
	int z = 1;
	shared_ptr<Texture> tex;
	shared_ptr<Door> door1 = NULL;
	shared_ptr<Door> door2 = NULL;
	//shared_ptr<Sound> colSound;
	//shared_ptr<Sound> yoSound;
	//shared_ptr<Sound> hitSound;


public:

	virtual ~Load() {
	
	}

	void deleteInt() {
		//delete tex;
		//delete door1;
		//delete door2;
	}

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



	void load(string levelName, string section, shared_ptr<Texture> texture,  list<shared_ptr<tile>>* tileList, list<shared_ptr<tile>>* z2List, list<shared_ptr<tile>>* z3List, list<shared_ptr<tile>>* z4List) {
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

				//if (*prev(tileList->end()) == NULL) {
				//	cout << "here";
				//}
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
		

		inputFile.close();


	}

	void loadFlags(string levelName, list<shared_ptr<EndFlag>>* flags, shared_ptr<Texture> t) {
		ifstream inputFile(levelName + "\\flags.txt");
		string line;
		string variable;
		char sep = ',';
		float lastX = NULL;
		transitionAngle lastAngle = RIGHT;
		
		while (getline(inputFile, line)) {

			vector<string> current = splitString(line, sep);
			shared_ptr<EndFlag> temp;

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
			else if (value == "door" || value == "flash door") {
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

		

			temp = shared_ptr<EndFlag>(new EndFlag (t, Vector2f(xPos, yPos), angle, sect));
			if (value == "door") {
				hFlagYPos(temp, flags);
				if (door1 == NULL) {
					door1 = shared_ptr<Door> (new Door(levelName, Vector2f(xPos, yPos), sect));
					door1->setCheckpoint();
				}
				else {
					door2 = shared_ptr<Door> (new Door(levelName, Vector2f(xPos, yPos), sect));
				}
			}

			else if (value == "flash door") {
				hFlagYPos(temp, flags);
				if (door1 == NULL) {
					door1 = shared_ptr<FlashDoor> (new FlashDoor(levelName, Vector2f(xPos, yPos), sect));
					door1->setCheckpoint();
				}
				else {
					door2 = shared_ptr<FlashDoor> (new FlashDoor(levelName, Vector2f(xPos, yPos), sect));
				}
			}

			if (checkpoint) {
				temp->setCheckpoint();
			}
		

			flags->push_back(temp);


		}

		upDownConsistency(flags);
	}

	void upDownConsistency(list<shared_ptr<EndFlag>>* flags) {

		for (int i = 0; i < (flags->size()); i++) {
			shared_ptr<EndFlag> flag = getFlag(i-1, *flags);
			if (flag != NULL) {
				if (getFlag(flag->getSection() - 1, *flags) != NULL) {
					shared_ptr<EndFlag> lastFlag = getFlag(flag->getSection() - 1, *flags);
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

					int vertDist = 1024;

					if (flag->getAngle() == UP) {
						if (lastFlag->getAngle() == UP) {
							flag->getSprite()->setPosition(Vector2f(flag->getPosition().x, lastFlag->getPosition().y - vertDist));
						}
						else {
							flag->getSprite()->setPosition(Vector2f(flag->getPosition().x, lastFlag->getPosition().y));
						}
					}
					else if (flag->getAngle() == DOWN && lastFlag->getAngle() != UP) {
						flag->getSprite()->setPosition(Vector2f(flag->getPosition().x, lastFlag->getPosition().y + vertDist));
					}


					if (flag->getAngle() == RIGHT || flag->getAngle() == LEFT) {
						if (lastFlag->getAngle() != UP) {
							flag->setPosition(Vector2f(flag->getPosition().x, lastFlag->getPosition().y));
						}
						else {
							flag->setPosition(Vector2f(flag->getPosition().x, lastFlag->getPosition().y - 1080));
						}
					}
				}
			}
		}


	}

	shared_ptr<Door> getDoor1() {
		return door1;
	}
	shared_ptr<Door> getDoor2() {
		return door2;
	}

	shared_ptr<EndFlag> getFlag(int sect, list<shared_ptr<EndFlag>> flagList) {
		for (shared_ptr<EndFlag> flag : flagList) {
			if (flag->getSection() == sect) {
				return flag;
			}
		}
		return NULL;
	}

	void hFlagYPos(shared_ptr<EndFlag> thisFlag, list<shared_ptr<EndFlag>>* flagList) {

		int sectCheck = thisFlag->getSection() - 1;
		shared_ptr<EndFlag> lastVFlag = getFlag(sectCheck, *flagList);
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

	void loadSaveFile(shared_ptr<player> p) {
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

			else if (type == "Crash Man") {
				p->setBomb(*valI == "y");
			}

			else if (type == "Air Man") {
				p->setAir(*valI == "y");
			}

		}


	}

	//This function is for loading objects into the object placer
	void loadObjects(string levelName, string section, list<shared_ptr<GameObject>>* objects, shared_ptr<Texture> t, shared_ptr<camera> cam) {

		ifstream inputFile(levelName + "\\" + section + "-objects.txt");



		shared_ptr<Texture> misc = shared_ptr<Texture> (new Texture());
		misc->loadFromFile("Assets\\misc\\mega buster.png");

		shared_ptr<Texture> beamT = shared_ptr<Texture> (new Texture());
		beamT->loadFromFile("assets\\beam.png");

		string line;

		shared_ptr<SpawnArea> sArea = NULL;

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
			shared_ptr<GameObject> add = NULL;
			shared_ptr<GameObject> backAdd = NULL;
			shared_ptr<GameObject> foreAdd = NULL;
			shared_ptr<enemy> enem = NULL;
			shared_ptr<Spawner> spawnAdd = NULL;
			shared_ptr<Item> item = NULL;

			shared_ptr<SoundCollection> soundCol = shared_ptr<SoundCollection> (new SoundCollection());

			shared_ptr<BeamCollection> beamCol = NULL;

			checkCode(type, t, misc, beamT, worldX, worldY, &enem, &add, &backAdd, &foreAdd, &sArea, &spawnAdd, &item, soundCol, &beamCol, objects);


			objects->remove(beamCol);
			//delete beamCol;

			if (add != NULL) {
				add->getSprite()->setPosition(Vector2f(worldX, worldY));
				add->setCode();
				add->setDisplay(true);
			}

			if (enem == NULL && add != NULL) {
				add->initial();
				add->setPallete(levelName);
				objects->push_back(add);
			}
			if (add ==NULL && enem != NULL) {
				objects->push_back(enem);
				enem->initial();
				enem->setPallete(levelName);
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

	void spawnDeconstruction(shared_ptr<SpawnArea> spawn, list<shared_ptr<GameObject>>* objects, shared_ptr<camera> cam) {
		float start = spawn->getStartPos();
		float end = spawn->getEndPos();
		shared_ptr<enemy> en = spawn->getEnemy();
		en->initial();
		shared_ptr<SpawnPoint> startP = shared_ptr<SpawnPoint> (new SpawnPoint(string(en->getCode())));
		startP->getSprite()->setPosition(Vector2f(start, cam->getPosition().y));
		shared_ptr<SpawnPoint> endP = shared_ptr<SpawnPoint> (new SpawnPoint(string(en->getCode())));
		endP->getSprite()->setPosition(Vector2f(end, cam->getPosition().y));
		objects->push_back(startP);
		objects->push_back(endP);
	}
	

	//This function is for loading objects into the game
	void loadObjects(string levelName, string section, list<shared_ptr<GameObject>>* objects, list<shared_ptr<GameObject>>* backgroundOb, list<shared_ptr<GameObject>>* foregroundOb, list<shared_ptr<enemy>>* enemies, shared_ptr<Texture> t, shared_ptr<SpawnArea>* sArea, list<shared_ptr<Spawner>>* spawners, list<shared_ptr<Item>>* items, shared_ptr<SoundCollection> soundCol) {

		ifstream inputFile(levelName + "\\" + section + "-objects.txt");

		shared_ptr<Texture> misc = shared_ptr<Texture> (new Texture());
		misc->loadFromFile("Assets\\misc\\mega buster.png");

		shared_ptr<Texture> beamT = shared_ptr<Texture> (new Texture());
		beamT->loadFromFile("assets\\beam.png");

		string line;

		objects->clear();
		enemies->clear();

		shared_ptr<BeamCollection> beamCol = NULL;
		
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
			shared_ptr<GameObject> add = NULL;
			shared_ptr<GameObject> backAdd = NULL;
			shared_ptr<GameObject> foreAdd = NULL;
			shared_ptr<enemy> enem = NULL;
			shared_ptr<Spawner> spawnAdd = NULL;
			shared_ptr<Item> item = NULL;

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

	void collectionCheck(shared_ptr<Beam> en, list<shared_ptr<GameObject>>* objects, shared_ptr<BeamCollection>* beamCol) {

		//if (en->getCode() == "beam left" || en->getCode() == "beam right") {
			if (*beamCol == NULL) {
				*beamCol = shared_ptr<BeamCollection>(new BeamCollection());
				objects->push_back(*beamCol);
			}
			shared_ptr<BeamCollection> temp = *beamCol;
			temp->addBeam(en);
		//}
		
	}

	void setSounds(list<shared_ptr<GameObject>>* objects, list<shared_ptr<GameObject>>* backgroundOb, list<shared_ptr<enemy>>* enemies, list<shared_ptr<Item>>* items, shared_ptr<SoundCollection> soundCol) {

		//shared_ptr<Sound> hitSound = soundCol->getHit();

		for (shared_ptr<GameObject> ob : *objects) {

			objectCheck(ob, soundCol);
			
		}

		for (shared_ptr<enemy> e : *enemies) {
			e->loadSound(soundCol);
			e->setHitSound(soundCol->getHit());

			if (e->getCode() == "press") {
				e->setSound(soundCol->getPress());
			}
			if (e->getCode() == "flash man") {

			}
		}

		/*shared_ptr<SoundBuffer> colB = shared_ptr<SoundBuffer> (new SoundBuffer());
		colB->loadFromFile("assets\\sound\\land.wav");

		try {
			// Code that might throw an exception
			free(colSound);
		}
		catch (int e) {
			// exception handling code
		}
		/*shared_ptr<Sound> colSound = (shared_ptr<Sound>)malloc(1);
		colSound = shared_ptr<Sound>(new Sound());
		colSound->setBuffer(*colB);
		*/
		for (shared_ptr<Item> item : *items) {
			if (item->getCode() == "Extra Life" || item->getCode() == "E Tank") {
				item->setSoundPointer(soundCol->getLifeGet());
			}
		}

	}

	void objectCheck(shared_ptr<GameObject> ob, shared_ptr<SoundCollection> soundCol) {
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

	void checkCode(string type, shared_ptr<Texture> t, shared_ptr<Texture> misc, shared_ptr<Texture> beamT, float worldX, float worldY, shared_ptr<enemy>* enem, shared_ptr<GameObject>* add, shared_ptr<GameObject>* backAdd, shared_ptr<GameObject>* foreAdd, shared_ptr<SpawnArea>* spawn, shared_ptr<Spawner>* spawnAdd, shared_ptr<Item>* item, shared_ptr<SoundCollection> soundCol, shared_ptr<BeamCollection>* beamCol, list<shared_ptr<GameObject>>* objects) {

		Vector2f worldPos = Vector2f(worldX, worldY);

		if (type == "e1") {
			*enem = shared_ptr<enemy>(new bat(t, Vector2f(worldX, worldY)));
		}
		else if (type == "rabbit") {
			*enem = shared_ptr<enemy>(new Rabbit(t, Vector2f(worldX, worldY)));
		}
		else if (type == "gorilla") {
			*enem = shared_ptr<enemy>(new Gorilla(t, Vector2f(worldX, worldY)));
		}
		else if (type == "wolf") {
			*enem = shared_ptr<enemy>(new Wolf(t, Vector2f(worldX, worldY)));
		}
		else if (type == "wood man") {
			*enem = shared_ptr<enemy>(new WoodMan(Vector2f(worldX, worldY)));
		}
		else if (type == "heat man") {
			*enem = shared_ptr<enemy>(new HeatMan(Vector2f(worldX, worldY)));
		}
		else if (type == "bubble man") {
			*enem = shared_ptr<enemy>(new BubbleMan(Vector2f(worldX, worldY)));
		}
		else if (type == "metal man") {
			*enem = shared_ptr<enemy>(new MetalMan(worldPos));
		}
		else if (type == "quick man") {
			*enem = shared_ptr<enemy>(new QuickMan(worldPos));
		}
		else if (type == "flash man") {
			*enem = shared_ptr<enemy>(new FlashMan(worldPos));
		}
		else if (type == "fly guy") {
			*enem = shared_ptr<enemy>(new FlyGuy(t, Vector2f(worldX, worldY)));
		}
		else if (type == "springer") {
			*enem = shared_ptr<enemy>(new Springer(t, Vector2f(worldX, worldY)));
		}
		else if (type == "break wall") {
			*enem = shared_ptr<enemy>(new BreakWall(t, Vector2f(worldX, worldY)));
		}
		else if (type == "sniper joe") {
			*enem = shared_ptr<enemy>(new SniperJoe(t, Vector2f(worldX, worldY)));
		}
		else if (type == "sniper armour") {
			*enem = shared_ptr<enemy>(new SniperArmour(t, Vector2f(worldX, worldY)));
		}
		else if (type == "crabbot") {
			*enem = shared_ptr<enemy>(new Crabbot(t, Vector2f(worldX, worldY)));
		}
		else if (type == "croaker") {
			*enem = shared_ptr<enemy>(new Croaker(t, Vector2f(worldX, worldY)));
		}
		else if (type == "shrink") {
			*enem = shared_ptr<enemy>(new Shrink(t, Vector2f(worldX, worldY)));
		}
		else if (type == "anko") {
			*enem = shared_ptr<enemy>(new Anko(t, Vector2f(worldX, worldY)));
		}
		else if (type == "press") {
			*enem = shared_ptr<enemy>(new Press(t, worldPos));
			shared_ptr<enemy> temp = *enem;
			temp->setCode();
		}

		else if (type == "neo mettool") {
			*enem = shared_ptr<NeoMettool>(new NeoMettool(t, worldPos));
		}

		else if (type == "blocky") {
			*enem = shared_ptr<enemy> (new Blocky(t, worldPos));
		}

		else if (type == "pie robot") {
			*enem = shared_ptr<enemy>(new PieRobot(t, worldPos));
		}

		else if (type == "crash man") {
			*enem = shared_ptr<enemy>(new CrashMan(worldPos));

		}

		else if (type == "beam left") {
			shared_ptr<BeamLeft> b = shared_ptr<BeamLeft> (new BeamLeft(beamT, worldPos));
			collectionCheck(b, objects, beamCol);
			*enem = b;
		}
		else if (type == "beam right") {
			shared_ptr<BeamRight> b = shared_ptr<BeamRight>(new BeamRight(beamT, worldPos));
			collectionCheck(b, objects, beamCol);
			*enem = b;
		}

		else if (type == "scworm spawn") {
			*enem = shared_ptr<enemy>(new ScwormSpawn(t, worldPos));
		}

		else if (type == "torch guy") {
			*enem = shared_ptr<enemy> (new TorchGuy(t, worldPos));
		}

		else if (type == "bird-spawn") {
			if (*spawn == NULL) {
				*spawn = shared_ptr<SpawnArea>(new BirdSpawner(worldX));
			}
			else {
				shared_ptr<SpawnArea> temp = *spawn;
				temp->setEnd(worldX);
				temp->initial();
			}
		}

		else if (type == "jelly fish-spawn") {
			if (*spawn == NULL) {
				*spawn = shared_ptr<SpawnArea>(new JellyFishSpawner(worldX));
			}
			else {
				shared_ptr<SpawnArea> temp = *spawn;
				temp->setEnd(worldX);
				temp->initial();
			}
		}

		else if (type == "snapper-spawn") {
			if (*spawn == NULL) {
				*spawn = shared_ptr<SpawnArea>(new SnapperSpawn(worldX));
			}
			else {
				shared_ptr<SpawnArea> temp = *spawn;
				temp->setEnd(worldX);
				temp->initial();
			}
		}

		else if (type == "chicken-spawn") {
			if (*spawn == NULL) {
				*spawn = shared_ptr<SpawnArea>(new ChickenSpawner(worldX));
			}
			else {
				shared_ptr<SpawnArea> temp = *spawn;
				temp->setEnd(worldX);
				temp->initial();
			}
		}
		else if (type == "drill-spawn") {
			if (*spawn == NULL) {
				*spawn = shared_ptr<SpawnArea>(new DrillSpawner(worldX));
			}
			else {
				shared_ptr<SpawnArea> temp = *spawn;
				temp->setEnd(worldX);
				temp->initial();
			}
		}

		else if (type == "fall platform") {
			*add = shared_ptr<GameObject> (new FallPlatform(t, Vector2f(worldX, worldY)));
		}

		else if (type == "lighting rect") {
			*add = shared_ptr<GameObject>(new LightingRect());
		}

		else if (type == "fly guy-spawn") {
			*spawnAdd = shared_ptr<Spawner>(new FlyGuySpawner(t, Vector2f(worldX, worldY)));
		}
		else if (type == "telly-spawn") {
			*spawnAdd = shared_ptr<Spawner>(new TellySpawner(t, Vector2f(worldX, worldY)));
		}
		
		
		
		else if (type == "trch-R") {
			*add = shared_ptr<GameObject> (new Torch(t, Vector2f(worldX, worldY), Colour::Red(), 300, 210));
		}
		/*else if (type == "flag") {
			add = shared_ptr<EndFlag> (new EndFlag(t, Vector2f(worldX, worldY));
		}
		else if (type == "flag-up") {
			add = shared_ptr<EndFlag> (new EndFlag(t, Vector2f(worldX, worldY), UP);
		}
		else if (type == "flag-down") {
			add = shared_ptr<EndFlag> (new EndFlag(t, Vector2f(worldX, worldY), DOWN);
		}*/
		else if (type == "health-big") {
			*item = shared_ptr<Item>(new BigHealth(misc, Vector2f(worldX, worldY)));

		}
		else if (type == "health-small") {
			*item = shared_ptr<Item> (new SmallHealth(misc, Vector2f(worldX, worldY)));

		}
		else if (type == "ammo-big") {
			*item = shared_ptr<Item> (new BigAmmo(misc, Vector2f(worldX, worldY)));

		}
		else if (type == "ammo-small") {
			*item = shared_ptr<Item> (new SmallAmmo(misc, Vector2f(worldX, worldY)));

		}
		else if (type == "E Tank") {
			*item = shared_ptr<Item>(new ETank(misc, Vector2f(worldX, worldY)));

		}
		else if (type == "Extra Life") {
			*item = shared_ptr<Item>(new ExtraLife(misc, Vector2f(worldX, worldY)));

		}

		else if (type == "rail platform") {
			*add = shared_ptr<GameObject>(new RailPlatform(t, Vector2f(worldX, worldY)));
		}
		else if (type == "rail checkpoint-down") {
			*add = shared_ptr<GameObject>(new CheckDown(Vector2f(worldX, worldY)));
		}
		else if (type == "rail checkpoint-up") {
			*add = shared_ptr<GameObject>(new CheckUp(Vector2f(worldX, worldY)));
		}
		else if (type == "rail checkpoint-left") {
			*add = shared_ptr<GameObject>(new CheckLeft(Vector2f(worldX, worldY)));
		}
		else if (type == "rail checkpoint-right") {
			*add = shared_ptr<GameObject>(new CheckRight(Vector2f(worldX, worldY)));
		}


		//Checks variable codes
		else {
			vector<string> spl = splitString(type, char('-'));
			string altType = spl[0];
			if (altType == "background") {
				*backAdd = shared_ptr<GameObject>(shared_ptr<Background> (new Background((stoi(spl[1])))));
			}
			if (altType == "disappearing tile") {
				*add = shared_ptr<GameObject>(new DisappearingTile(t, Vector2f(worldX, worldY), stoi(spl[1])));
			}

			if (altType == "crazy cannon") {
				shared_ptr<CrazyCannon> canon = shared_ptr<CrazyCannon>(new CrazyCannon(t, worldPos));
				


				canon->setRight(spl[1] == "y");
				*enem = canon;
			}
		}


		
	}


	shared_ptr<tile> tileCreation(Vector2f worldPos, string selectedType, int selectedTexture) {

		//Instead of using sub-classes to store different types of collision,
		//I should have just used one tile class and activated different collision using functions.
		//That would have been a way to avoid the magic numbering you see below.

		if (selectedType == "0") {
			return shared_ptr<tile>(new tile(worldPos, tex, selectedTexture, z));
		}
		else if (selectedType == "1") {
			return shared_ptr<tile>(new topTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == "2") {
			return shared_ptr<tile>(new rightTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == "3") {
			return shared_ptr<tile>(new ceilingTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == "4") {
			return shared_ptr<tile>(new leftTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == "8") {
			return shared_ptr<tile>(new ladderTile(worldPos, tex));
		}
		else if (selectedType == "9") {
			return shared_ptr<tile>(new topLadder(worldPos, tex));
		}
		else if (selectedType == "10") {
			return shared_ptr<tile>(new HorizontalLava(worldPos, tex, z));
		}
		else if (selectedType == "11") {
			return shared_ptr<tile>(new VerticalLava(worldPos, tex, z));
		}
		else if (selectedType == "12") {
			return shared_ptr<tile>(new PourLava(worldPos, tex, z));
		}
		else if (selectedType == "5" || selectedType == "6" || selectedType == "7") {
			return shared_ptr<tile>(new solidTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == "water") {
			return shared_ptr<tile>(new WaterTile(worldPos, tex, selectedTexture));
		}
		else if (selectedType == "death") {
			return shared_ptr<tile>(new DeathTile(worldPos, tex, selectedTexture, z));
		}
		
		else {
			vector<string> spl = splitString(selectedType, char('-'));
			string altType = spl[0];

			if (altType == "0"){
				return shared_ptr<tile>(new AnimTile(worldPos, tex, stoi(spl[1]), stoi(spl[2]), z));
			}
			else if (altType == "water") {
				return shared_ptr<tile>(new WaterAnim(worldPos, tex, stoi(spl[1]), 3, z));
			}
			else if (altType == "conveyor anim") {
				return shared_ptr<tile>(new ConveyorTile(worldPos, tex, z, spl[1] == "right"));
			}
			else if (altType == "move") {
				shared_ptr<MoveTile> temp = shared_ptr<MoveTile>(new MoveTile(worldPos, tex));
				temp->setMoveRight(spl[1] == "right");
				return temp;
			}

			else if (altType == "square flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new SquareFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "vertical flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new VTubeTile(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "horizontal flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new HTubeTile(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "top right corner flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new TopRightCornerFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "top left corner flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new TopLeftCornerFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "bottom right corner flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new BottomRightCornerFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "bottom left corner flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new BottomLeftCornerFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}

			else if (altType == "bottom right w flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new BottomRightWFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "bottom left w flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new BottomLeftWFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "top left w flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new TopLeftWFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "top right w flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new TopRightWFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "w wall flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new WWallFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "w ceiling flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new WCeilingFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "w roof flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new WRoofFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "left edge flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new LeftEdgeFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "right edge flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new RightEdgeFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "top edge flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new TopEdgeFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "bottom edge flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new BottomEdgeFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}
			else if (altType == "bottom left corner w flash") {
				shared_ptr<FlashTile> temp = shared_ptr<FlashTile>(new BottomLeftCornerWFlash(worldPos, tex));
				temp->setTiming(stoi(spl[1]));
				return temp;
			}



			else {
					cout<<"Unrecognised tile type in file";
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