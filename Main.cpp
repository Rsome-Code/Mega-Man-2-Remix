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
#include "Wood Man Stage.cpp"
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
#include "Extra life.cpp"
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
int main() {

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
	LevelSelect* levelMenu = new LevelSelect(bg);

	
	//string bossName = levelMenu->loop(instance, targetFPS, bg);
	string bossName = "wood man";
	bool hold = levelMenu->checkA();
	abstractStage* wood = new abstractStage(bossName);

	wood->reload(string("wood man"));

	scene* area = new scene(col, wood, enemyT);

	Texture* bossT;
	bossT = new Texture ();
	bossT->loadFromFile("assets\\" + bossName + ".png");

	StageIntro* intro = new StageIntro(bossName, hold, bg, bossT);
	//intro->loop(instance, targetFPS);

	wT->loadFromFile("assets\\" + bossName + "-stage.png");
	levelEditor* l = new levelEditor(wT, bossName);

	Texture* misc = new Texture();
	misc->loadFromFile("assets\\misc\\mega buster.png");


//Object Placer setup
	list<object*> obList = { new ExtraLife(misc, Vector2f(0,0)), new ETank(misc, Vector2f(0,0)), new SmallAmmo(misc, Vector2f(0,0)), new BigAmmo(misc, Vector2f(0,0)), new SmallHealth(misc, Vector2f(0,0)) , new BigHealth(misc, Vector2f(0,0)),  new bat(enemyT, Vector2f(600, 600)), new Torch(enemyT, Vector2f(0,0), Color::Red, 1000, 100), new EndFlag(enemyT, Vector2f(0,0), 0), new EndFlag(enemyT, Vector2f(0,0), UP,0),  new EndFlag(enemyT, Vector2f(0,0), DOWN,0) };
	ObjectPlacer* o = new ObjectPlacer(wT, bossName, obList);


	//Test animation setup
	//////////////////////
	list<IntRect> testAnim = list<IntRect>{ IntRect(347, 42, 24, 24), IntRect(374, 44, 20, 20) };
	list<Vector2f> testOffset = list<Vector2f>{ Vector2f(0, 0), Vector2f(8, 8) };

	Texture* testT = new Texture();
	testT->loadFromFile("Assets\\weapons.png");
	AnimationTest* test = new AnimationTest(testAnim, testOffset, testT);

	////////////////////////////////
	//Uncomment this if you want to use the animation tester
	//test->run(instance, targetFPS);


	//Un-comment this if you want to use the level editor
	//l->loop(instance, targetFPS);
	//
	// Un-comment this if you want to use the object placer
	//o->loop(instance, targetFPS);




	area->loop(instance, targetFPS);

	//mainMenu* menu = new mainMenu();
	//menu->menu(instance, targetFPS, col);
	cout << "hi";

}