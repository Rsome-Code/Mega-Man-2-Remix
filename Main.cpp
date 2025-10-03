#include <chrono>
#include <thread>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <Xinput.h>
#include "controller.cpp"
#include "xcontrol.cpp"
#include "render logic.cpp"
#include "player.cpp"
#include "scene.cpp"
#include "Energy Bar.cpp"
#include "Level Editor.cpp"
#include "Object Placement.cpp"
#include "Stage Intro.cpp"
#include "Level Select.cpp"
#include "End Flag.cpp"
#include "Animation Test.cpp"
#include <vector>
#include <string>
#include <sstream>
#include "pause.cpp"
#include "load.cpp"
#include "E tank.cpp"
#include "door.cpp"
#include "Extra life.cpp"
#include "gorilla.cpp"
#include "wolf.cpp"
#include "spawn point.cpp"
#include "wood man.cpp"
#include "get equipped.cpp"
#include "equip menu.cpp"
#include "game over.cpp"
#include "game over menu.cpp"
#pragma once
#pragma comment(lib,"winmm.lib")

#include <fstream>
using namespace std;


//using namespace std::chrono; // nanoseconds, system_clock, seconds
using namespace sf;
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
Weapon* updatePlayer(player* p, string levelName) {
	if (levelName == "wood man") {
		return p->getShield();
	}
	else if (levelName == "heat man") {
		return p->getAtomicFire();
	}
}
int main() {
	bool run = true;

	//Set the framerate here
	double targetFPS = 180;

	pController* p1 = new pController();

	Texture* enemyT = new Texture();
	enemyT->loadFromFile("Assets\\enemy.png");

	Texture* miscT = new Texture();
	miscT->loadFromFile("Assets\\misc\\mega buster.png");

	player* col = new player(p1);
	Load* load = new Load();
	load->loadSaveFile(col);

	delete(load);

	//woodManStage* wood = new woodManStage(enemyT, miscT);


	
	
	RenderWindow window(VideoMode(1920, 1080), "Executable", Style::Default);

	RenderWindow* w = &window;
	renderer* instance = new renderer(w);

	Texture* wT = new Texture();

	Texture* bg;
	bg = new Texture();
	bg->loadFromFile("Assets\\NES - Mega Man 2 - Stage Select.png");

	
	//string bossName = levelMenu->loop(instance, targetFPS, bg);
	string bossName = "wood man";
	
	

	

	Texture* bossT;
	bossT = new Texture ();
	bossT->loadFromFile("assets\\" + bossName + ".png");

	

	wT->loadFromFile("assets\\" + bossName + "-stage.png");
	levelEditor* l = new levelEditor(wT, bossName);

	Texture* misc = new Texture();
	misc->loadFromFile("assets\\misc\\mega buster.png");

	Texture* woodBossT = new Texture();
	woodBossT->loadFromFile("assets\\wood man.png");

//Object Placer setup
	list<object*> obList = { new WoodMan(woodBossT, Vector2f(0,0)), new SpawnPoint(string("chicken")),new SpawnPoint(string("bird")), new Wolf(enemyT, Vector2f(0,0)), new Gorilla(enemyT, Vector2f(0,0)), new Rabbit(enemyT, Vector2f(0,0)), new Door(bossName, Vector2f(0,0), 0), new ExtraLife(misc, Vector2f(0,0)), new ETank(misc, Vector2f(0,0)), new SmallAmmo(misc, Vector2f(0,0)), new BigAmmo(misc, Vector2f(0,0)), new SmallHealth(misc, Vector2f(0,0)) , new BigHealth(misc, Vector2f(0,0)),  new bat(enemyT, Vector2f(600, 600)), new Torch(enemyT, Vector2f(0,0), Color::Red, 1000, 100), new EndFlag(enemyT, Vector2f(0,0), 0), new EndFlag(enemyT, Vector2f(0,0), UP,0),  new EndFlag(enemyT, Vector2f(0,0), DOWN,0)};
	for (object* o : obList) {
		o->initial();
	}
	ObjectPlacer* o = new ObjectPlacer(wT, bossName, obList);


	//Test animation setup
	//////////////////////
	list<IntRect> testAnim = list<IntRect>{ IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(163, 136), Vector2i(22, 15)), IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(124, 128), Vector2i(7, 24)) };
	list<Vector2f> testOffset = list<Vector2f>{ Vector2f(0 * 4,6 * 4),Vector2f(0 * 4, 10 * 4),Vector2f(0 * 4, 6 * 4), Vector2f(7 * 4, 0 * 4) };

	Texture* testT = new Texture();
	testT->loadFromFile("Assets\\player\\NES - Mega Man 2 - Mega Man.png");
	AnimationTest* test = new AnimationTest(testAnim, testOffset, testT, true);

	////////////////////////////////
	//Uncomment this if you want to use the animation tester
	//test->run(instance, targetFPS);


	//Un-comment this if you want to use the level editor
	//l->loop(instance, targetFPS);
	//
	// Un-comment this if you want to use the object placer
//	o->loop(instance, targetFPS);


	bg = new Texture();
	bg->loadFromFile("Assets\\NES - Mega Man 2 - Stage Select.png");
	bool hold;
	LevelSelect* levelMenu;
	
	bool restart = false;
	while (run) {
		levelMenu = new LevelSelect(bg, col->checkLead(), col->checkAtomicFire(), col->checkBlade(), col->checkShield(), col->checkTornado(), col->checkBoomerang(), col->checkStopper(), col->checkBomb());
		
		if (!restart) {
			bossName = levelMenu->loop(instance, targetFPS, bg);

			hold = levelMenu->checkA();

			StageIntro* intro = new StageIntro(bossName, hold, bg, bossT);
		}
		restart = false;
		//intro->loop(instance, targetFPS);

		col->setGrounded(true);
		col->HPReset();
		
		if (col->getLives() < 2) {
			col->setLives(2);
		}
		//col->heal(-27);
		//col->setLives(0);

		abstractStage* stage = new abstractStage(bossName);

		//stage->reload(string(bossName), string("0"));

		scene* area = new scene(col, stage, enemyT);
		if (area->loop(instance, targetFPS)) {
		
			Weapon* newW = updatePlayer(col, bossName);

			EquipAnim* equip = new EquipAnim(newW);
			equip->loop(instance, targetFPS);
			EquipMenu* eMenu = new EquipMenu(equip->getTexture(), equip->getSprites(), equip->getText(), Vector2f(500, 650));
			
			bool eLoop = true;
			while (eLoop) {
				eLoop = eMenu->loop(instance, targetFPS, equip->getMusic());
				if (eLoop) {
					//This is where the password screen will be run
				}
			}
		}
		else {
			bool gLoop = true;
			while (gLoop) {
				GameOver* gO = new GameOver();
				gO->loop(instance, targetFPS);
				GameOverMenu* gMenu = new GameOverMenu();
				
				GameOverMenu::Option option = gMenu->loop(instance, targetFPS);
				if (option == GameOverMenu::Option::Password) {
					//This is where the password screen will be run
				}
				else {
					gLoop = false;
					if (option == GameOverMenu::Option::Continue) {
						restart = true;
					}
					else if (option == GameOverMenu::Option::StageSelect) {
						//???
					}
				}
			}
		}
		
		run = instance->getWindow()->isOpen();
		
	}

	//mainMenu* menu = new mainMenu();
	//menu->menu(instance, targetFPS, col);
	cout << "hi";

}