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
	double targetFPS = 60;

	pController* p1 = new pController();

	Texture* enemyT = new Texture();
	enemyT->loadFromFile("Assets\\enemy.png");

	Texture* miscT = new Texture();
	miscT->loadFromFile("Assets\\misc.png");

	player* col = new player(p1);

	//woodManStage* wood = new woodManStage(enemyT, miscT);
	woodManStage* wood = new woodManStage();

	scene* area = new scene(col, wood, enemyT);
	
	RenderWindow window(VideoMode(1920, 1080), "Executable");

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

	Texture* bossT;
	bossT = new Texture ();
	bossT->loadFromFile("assets\\" + word + ".png");

	StageIntro* intro = new StageIntro(word, hold, bg, bossT);
	//intro->loop(instance, targetFPS);

	wT->loadFromFile("assets\\" + word + "-stage.png");
	levelEditor* l = new levelEditor(wT, "myfile");

	ObjectPlacer* o = new ObjectPlacer(wT, "myfile");

	//l->loop(instance, targetFPS);
	//o->loop(instance, targetFPS);

	area->loop(instance, targetFPS);

	//mainMenu* menu = new mainMenu();
	//menu->menu(instance, targetFPS, col);
	cout << "hi";


}