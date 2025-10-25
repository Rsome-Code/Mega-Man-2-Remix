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
#include "background.cpp"
#include "GObject.cpp"
#include "disappearing tile.cpp"
#include "flyguy.cpp"
#include "spawner.cpp"
#include "springer.cpp"
#include "break wall.cpp"
#include "sniper armour.cpp"
#include "heat man.cpp"
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
		p->setAtomicFire(true);
		return p->getAtomicFire();
	}
}
int main() {
	bool run = true;

	//Set the framerate here
	double targetFPS = 120;

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
	string bossName = "heat man";
	
	

	Texture* bossT;
	bossT = new Texture ();
	bossT->loadFromFile("assets\\" + bossName + ".png");

	

	wT->loadFromFile("assets\\stage\\" + bossName + ".png");
	levelEditor* l = new levelEditor(wT, bossName);

	Texture* misc = new Texture();
	misc->loadFromFile("assets\\misc\\mega buster.png");

	Texture* woodBossT = new Texture();
	woodBossT->loadFromFile("assets\\wood man.png");

	Texture* heatBossT = new Texture();
	heatBossT->loadFromFile("assets\\heat man.png");

//Object Placer setup
	list<GameObject*> woodManObList = { new Background(Color::Color(0, 232, 216)), new WoodMan(woodBossT, Vector2f(0,0)), new SpawnPoint(string("chicken")),new SpawnPoint(string("bird")), new Wolf(enemyT, Vector2f(0,0)), new Gorilla(enemyT, Vector2f(0,0)), new Rabbit(enemyT, Vector2f(0,0)), new Door(bossName, Vector2f(0,0), 0), new ExtraLife(misc, Vector2f(0,0)), new ETank(misc, Vector2f(0,0)), new SmallAmmo(misc, Vector2f(0,0)), new BigAmmo(misc, Vector2f(0,0)), new SmallHealth(misc, Vector2f(0,0)) , new BigHealth(misc, Vector2f(0,0)),  new bat(enemyT, Vector2f(600, 600)), new Torch(enemyT, Vector2f(0,0), Color::Red, 1000, 100), new EndFlag(enemyT, Vector2f(0,0), 0), new EndFlag(enemyT, Vector2f(0,0), UP,0),  new EndFlag(enemyT, Vector2f(0,0), DOWN,0)};
	list <GameObject*> heatManList = { new ExtraLife(misc, Vector2f(0,0)), new HeatMan(heatBossT, Vector2f(0,0)), new SniperArmour(enemyT, Vector2f(0,0)), new BreakWall(enemyT, Vector2f(0,0)), new Springer(enemyT, Vector2f(0,0)), new TellySpawner(enemyT, Vector2f(0,0)), new FlyGuySpawner(enemyT, Vector2f(0,0)), new DisappearingTile(enemyT, Vector2f(0,0), 0), new DisappearingTile(enemyT, Vector2f(0,0), 1), new DisappearingTile(enemyT, Vector2f(0,0), 2), new DisappearingTile(enemyT, Vector2f(0,0), 3), new Door(bossName, Vector2f(0,0), 0), new EndFlag(enemyT, Vector2f(0,0), 0), new EndFlag(enemyT, Vector2f(0,0), UP,0),  new EndFlag(enemyT, Vector2f(0,0), DOWN,0) };
	
	for (GameObject* o : heatManList) {
		o->initial();
	}
	ObjectPlacer* o = new ObjectPlacer(wT, bossName, heatManList);
	


	//Test animation setup
	//////////////////////
	list<IntRect> testAnim = list<IntRect>{ IntRect(2, 28, 29, 25), IntRect(38, 33, 32, 20), IntRect(71, 27, 24, 26),IntRect(97, 28, 29, 25), IntRect(129, 22, 32, 31), IntRect(163, 21, 31, 32) };
	list<Vector2f> testOffset =  list<Vector2f>{Vector2f(0,0), Vector2f(-2 * 4, 5 * 4), Vector2f(2 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(-2 * 4, -6 * 4), Vector2f(-2 * 4, -7 * 4) };

	Texture* testT = new Texture();
	testT->loadFromFile("Assets\\heat man.png");
	AnimationTest* test = new AnimationTest(testAnim, testOffset, testT, true);

	////////////////////////////////
	//Uncomment this if you want to use the animation tester
	//test->run(instance, targetFPS);


	//Un-comment this if you want to use the level editor
	//l->loop(instance, targetFPS);
	//
	// Un-comment this if you want to use the object placer
	//o->loop(instance, targetFPS);


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
			intro->loop(instance, targetFPS);
		}
		restart = false;
		

		col->setGrounded(true);
		col->HPReset();
		col->ammoReset();
		
		if (col->getLives() < 2) {
			col->setLives(2);
		}
		//col->heal(-27);
		//col->setLives(0);

		abstractStage* stage = new abstractStage(bossName);

		//stage->reload(string(bossName), string("0"));

		scene* area = new scene(col, stage, enemyT);

		if (area->loop(instance, targetFPS)) {

		//if (true){
		
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
	//cout << "hi";

}