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
	miscT->loadFromFile("Assets\\misc.png");

	player* col = new player(p1);

	//woodManStage* wood = new woodManStage(enemyT, miscT);


	
	
	RenderWindow window(VideoMode(1920, 1080), "Executable", Style::Default);

	RenderWindow* w = &window;
	renderer* instance = new renderer(w);

	Texture* wT = new Texture();

	Texture* bg;
	bg = new Texture();
	bg->loadFromFile("Assets\\NES - Mega Man 2 - Stage Select.png");
	LevelSelect* levelMenu = new LevelSelect(bg);

	//string word = levelMenu->loop(instance, targetFPS, bg);
	string word = "wood man";
	bool hold = levelMenu->checkA();
	abstractStage* wood = new abstractStage(word);

	scene* area = new scene(col, wood, enemyT);

	Texture* bossT;
	bossT = new Texture ();
	bossT->loadFromFile("assets\\" + word + ".png");

	StageIntro* intro = new StageIntro(word, hold, bg, bossT);
	//intro->loop(instance, targetFPS);

	wT->loadFromFile("assets\\" + word + "-stage.png");
	levelEditor* l = new levelEditor(wT, word);

	list<object*> obList = { new bat(enemyT, Vector2f(600, 600)), new Torch(enemyT, Vector2f(0,0), Color::Red, 1000, 100), new EndFlag(enemyT, Vector2f(0,0))};
	ObjectPlacer* o = new ObjectPlacer(wT, word, obList);

	list<IntRect> testAnim = list<IntRect>{ IntRect(134, 117, 30, 20), IntRect(170, 117, 24, 28), IntRect(206, 117, 18, 19) };
	list<Vector2f> testOffset = list<Vector2f>{ Vector2f(0, 5), Vector2f((12), 2), Vector2f(24, 0)};
	AnimationTest* test = new AnimationTest(testAnim, testOffset, enemyT);

	//test->run(instance, targetFPS);


	//l->loop(instance, targetFPS);
	//o->loop(instance, targetFPS);

	area->loop(instance, targetFPS);

	//mainMenu* menu = new mainMenu();
	//menu->menu(instance, targetFPS, col);
	cout << "hi";

}