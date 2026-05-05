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
#include "spawn point.cpp"
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
#include "sound collection.cpp"
#include "falling platform.cpp"
#include "crabbot.cpp"
#include "croaker.cpp"
#include "shrink.cpp"
#include "anko.cpp"
#include "DL Message.cpp"
#include "bubble man.cpp"
#include "press.cpp"
#include "drill.cpp"
#include "blocky.cpp"
#include "pie robot.cpp"
#include "metal man.cpp"
#include "beam left.cpp"
#include "beam right.cpp"
#include "torch guy.cpp"
#include "quick man.cpp"
#include "flash door.cpp"
#include "crazy cannon.cpp"
#include "flash man.cpp"
#include "cannon right.cpp"
#include "checkDown.cpp"
#include "checkUp.cpp"
#include "checkLeft.cpp"
#include "checkRight.cpp"
#include "rail platform.cpp"
#include "crash man.cpp"
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
shared_ptr<Weapon> updatePlayer(shared_ptr<player> p, string levelName) {
	if (levelName == "wood man") {
		
		return p->getShield();
	}
	else if (levelName == "heat man") {
		p->setAtomicFire(true);
		return p->getAtomicFire();
	}
	else if (levelName == "bubble man") {
		p->setBubbleLead(true);
		return p->getBubbleLead();
	}
	else if (levelName == "metal man") {
		p->setMetalBlade(true);
		return p->getMetalBlade();
	}
	else if (levelName == "quick man") {
		p->setBoomerang(true);
		return p->getBoomerang();
	}
	else if (levelName == "flash man") {
		p->setTimeStopper(true);
		return p->getTimeStopper();
	}
	return NULL;
}

vector<bool> beforeTileList;
void beforeLevelCheck(string name) {
	if (name == "metal man") {
		beforeTileList = { true, true, false, false, false };
	}
	else {
		beforeTileList = {};
	}
}

shared_ptr<Weapon> checkItem(shared_ptr<player> p, string levelName) {
	if (levelName == "heat man") {
		
		return p->getItem1();
	}
	return NULL;
}



int main() {

	shared_ptr<SoundCollection> soundCol (new SoundCollection());
	bool run = true;

	//Set the framerate here
	double targetFPS = 120;

	//Set the resolution here
	int verticalRes = 1080;


	int horiRes = verticalRes * (float(1920) / float(1080));

	shared_ptr <RenderWindow> window = shared_ptr <RenderWindow>(new RenderWindow(VideoMode(horiRes, verticalRes), "Executable", Style::Default));
	shared_ptr<pController> p1 = shared_ptr<pController>(new pController(window));

	shared_ptr<Texture> enemyT = shared_ptr<Texture> (new Texture());
	enemyT->loadFromFile("Assets\\enemy.png");

	shared_ptr<Texture> miscT = shared_ptr<Texture> (new Texture());
	miscT->loadFromFile("Assets\\misc\\mega buster.png");

	shared_ptr<player> col = shared_ptr<player>(new player(p1, soundCol));
	shared_ptr<Load> load = shared_ptr<Load>(new Load());
	load->loadSaveFile(col);



	//woodManStage* wood = new woodManStage(enemyT, miscT);


	
	shared_ptr<Font> font = shared_ptr<Font>(new Font());
	font->loadFromFile("assets//font.otf");


	shared_ptr<RenderWindow> w = move(window);
	shared_ptr<renderer> instance = shared_ptr<renderer> (new renderer(w, false));

	shared_ptr<Texture> wT = shared_ptr<Texture> (new Texture());

	shared_ptr<Texture> bg;
	bg = shared_ptr<Texture> (new Texture());
	bg->loadFromFile("Assets\\NES - Mega Man 2 - Stage Select.png");

	
	//string bossName = levelMenu->loop(instance, targetFPS, bg);
	string bossName = "crash man";
	
	

	shared_ptr<Texture> bossT;
	bossT = shared_ptr<Texture>(new Texture());
	bossT->loadFromFile("assets\\" + bossName + ".png");

	

	wT->loadFromFile("assets\\stage\\" + bossName + ".png");
	levelEditor* l = new levelEditor(wT, bossName, font);

	shared_ptr<Texture> misc = shared_ptr<Texture> (new Texture());
	misc->loadFromFile("assets\\misc\\mega buster.png");

	shared_ptr<Texture> woodBossT = shared_ptr<Texture> (new Texture());
	woodBossT->loadFromFile("assets\\wood man.png");

	shared_ptr<Texture> heatBossT = shared_ptr<Texture> (new Texture());
	heatBossT->loadFromFile("assets\\heat man.png");;

	shared_ptr<Texture> bubbleBossT = shared_ptr<Texture> (new Texture());
	bubbleBossT->loadFromFile("assets\\bubble man.png");

	shared_ptr<Texture> flashBossT = shared_ptr<Texture>(new Texture());
	flashBossT->loadFromFile("assets\\bubble man.png");
	
	shared_ptr<Texture> beamT = shared_ptr<Texture> (new Texture());
	beamT->loadFromFile("assets\\beam.png");

//Object Placer setup
	//list<shared_ptr<GameObject>> woodManObList = { shared_ptr<GameObject>(new Background((Colour::Colour(0, 232, 216)))), shared_ptr<GameObject>(new WoodMan(Vector2f(0,0))), shared_ptr<GameObject>(new SpawnPoint(string("chicken"))),shared_ptr<GameObject>(new SpawnPoint(string("bird"))), shared_ptr<GameObject>(new Wolf(enemyT, Vector2f(0,0))), shared_ptr<GameObject> (new Gorilla(enemyT, Vector2f(0,0))), shared_ptr<GameObject>(new Rabbit(enemyT, Vector2f(0,0))), shared_ptr<GameObject>(shared_ptr<Door> (new Door(bossName, Vector2f(0,0), 0)), shared_ptr<GameObject>(shared_ptr<ExtraLife> (new ExtraLife(misc, Vector2f(0,0))), shared_ptr<GameObject>(new ETank(misc, Vector2f(0,0))), shared_ptr<GameObject>(shared_ptr<SmallAmmo> (new SmallAmmo(misc, Vector2f(0,0))), shared_ptr<GameObject>(shared_ptr<BigAmmo> (new BigAmmo(misc, Vector2f(0,0))), shared_ptr<GameObject>(shared_ptr<SmallHealth> (new SmallHealth(misc, Vector2f(0,0))) , shared_ptr<GameObject>(shared_ptr<BigHealth> (new BigHealth(misc, Vector2f(0,0))),  new bat(enemyT, Vector2f(600, 600)), shared_ptr<GameObject> (new Torch(enemyT, Vector2f(0,0), Colour::Red, 1000, 100)), shared_ptr<GameObject>(shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), 0)), shared_ptr<GameObject>(shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), UP,0)),  shared_ptr<GameObject>(shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), DOWN,0))};
	//list <shared_ptr<GameObject>> heatManList = { shared_ptr<ExtraLife> (new ExtraLife(misc, Vector2f(0,0)), new HeatMan(Vector2f(0,0)), new SniperArmour(enemyT, Vector2f(0,0)), new BreakWall(enemyT, Vector2f(0,0)), new Springer(enemyT, Vector2f(0,0)), new TellySpawner(enemyT, Vector2f(0,0)), new FlyGuySpawner(enemyT, Vector2f(0,0)), new DisappearingTile(enemyT, Vector2f(0,0), 0), new DisappearingTile(enemyT, Vector2f(0,0), 1), new DisappearingTile(enemyT, Vector2f(0,0), 2), new DisappearingTile(enemyT, Vector2f(0,0), 3), shared_ptr<Door> (new Door(bossName, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), UP,0),  shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), DOWN,0) };
	//list <shared_ptr<GameObject>> bubbleList = {new BubbleMan(Vector2f(0,0)), new Anko(enemyT, Vector2f(0,0)), new Shrink(enemyT, Vector2f(0,0)), shared_ptr<SpawnPoint> (new SpawnPoint(string("snapper"))), new Croaker(enemyT, Vector2f(0,0)), new Crabbot(enemyT, Vector2f(0,0)), shared_ptr<GameObject>(new Background(Colour::Colour(0, 112, 236))), new FallPlatform(enemyT, Vector2f(0,0)), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), UP,0),  shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), DOWN,0) };
	//list <shared_ptr<GameObject>> metalList = {new MetalMan(Vector2f(0,0)),shared_ptr<ExtraLife> (new ExtraLife(misc, Vector2f(0,0)), new ETank(misc, Vector2f(0,0)), new PieRobot(enemyT, Vector2f(0,0)), new Blocky(enemyT, Vector2f(0,0)), shared_ptr<SpawnPoint> (new SpawnPoint(string("drill"))),  shared_ptr<Door> (new Door(bossName, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), UP,0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), DOWN,0), new Press(enemyT, Vector2f(0,0))};
	//list <shared_ptr<GameObject>> quickList = {new ETank(misc, Vector2f(0,0)), shared_ptr<BigHealth> (new BigHealth(misc, Vector2f(0,0)), shared_ptr<BigAmmo> (new BigAmmo(misc, Vector2f(0,0)), shared_ptr<ExtraLife> (new ExtraLife(misc, Vector2f(0,0)), new QuickMan(Vector2f(0,0)), new ScwormSpawn(enemyT, Vector2f(0,0)), shared_ptr<TorchGuy> (new TorchGuy (enemyT, Vector2f(0,0))), new BeamRight(beamT, Vector2f(0,0)), new BeamLeft(beamT, Vector2f(0,0)), new SniperArmour(enemyT, Vector2f(0,0)), shared_ptr<Door> (new Door(bossName, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), UP,0),  shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), DOWN,0) };
	//list <shared_ptr<GameObject>> flashList = { shared_ptr <FlashMan>(new FlashMan(Vector2f(0,0))), shared_ptr<Blocky>(new Blocky(enemyT, Vector2f(0,0))), shared_ptr<ScwormSpawn>(new ScwormSpawn(enemyT, Vector2f(0,0))), shared_ptr<SniperArmour>(new SniperArmour(enemyT, Vector2f(0,0))), shared_ptr<CannonRight>(new CannonRight(enemyT, Vector2f(0,0))), shared_ptr <CrazyCannon>(new CrazyCannon(enemyT, Vector2f(0,0))), shared_ptr <ETank>(new ETank(misc, Vector2f(0,0))), shared_ptr<BigHealth>(new BigHealth(misc, Vector2f(0,0))), shared_ptr<BigAmmo>(new BigAmmo(misc, Vector2f(0,0))), shared_ptr<ExtraLife>(new ExtraLife(misc, Vector2f(0,0))), shared_ptr<FlashDoor>(new FlashDoor(bossName, Vector2f(0,0), 0)), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), 0)), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), UP,0)),  shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), DOWN,0)), shared_ptr <BreakWall>(new BreakWall(enemyT, Vector2f(0,0)))};
	list <shared_ptr<GameObject>> crashList = { shared_ptr<CrashMan>(new CrashMan(Vector2f(0,0))), shared_ptr<CheckLeft>(new CheckLeft(Vector2f(0,0))), shared_ptr<CheckRight>(new CheckRight(Vector2f(0,0))), shared_ptr<CheckUp>(new CheckUp(Vector2f(0,0))), shared_ptr<CheckDown>(new CheckDown(Vector2f(0,0))), shared_ptr<RailPlatform>(new RailPlatform(enemyT, Vector2f(0,0))), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), 0)), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), UP,0)), shared_ptr<Door>(new Door(bossName, Vector2f(0,0), 0)), shared_ptr<FlyGuySpawner>(new FlyGuySpawner(enemyT, Vector2f(0,0))), shared_ptr<TellySpawner>(new TellySpawner(enemyT, Vector2f(0,0))) };

	for (shared_ptr<GameObject> o : crashList) {
		o->initial();
	}
	ObjectPlacer* o = new ObjectPlacer(wT, bossName, crashList);
	


	//Test animation setup
	//////////////////////
	list<IntRect> testAnim = list<IntRect>{ IntRect(0, 29, 31, 24), IntRect(108, 29, 32, 24), IntRect(141, 28, 18, 25), IntRect(160, 29, 24, 24), IntRect(141, 28, 18, 25) };
	list<Vector2f> testOffset =  list<Vector2f>{ Vector2f(0,0), Vector2f(0 * 4, 0 * 4), Vector2f(8 * 4, -1 * 4), Vector2f(6 * 4, 0 * 4), Vector2f(8 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0* 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4),Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4) };

	shared_ptr<Texture> testT = shared_ptr<Texture> (new Texture());
	testT->loadFromFile("Assets\\crash man.png");
	bool facingRight = false;
	AnimationTest* test = new AnimationTest(testAnim, testOffset, testT, facingRight);

	////////////////////////////////
	//Uncomment this if you want to use the animation tester
	//test->run(instance, targetFPS);


	//Un-comment this if you want to use the level editor
	//l->loop(instance, targetFPS);
	//
	// Un-comment this if you want to use the object placer
	// There must be a flag in the section that comes before the start
	//
	//
	//o->loop(instance, targetFPS);



	delete l;
	delete o;

	bg = shared_ptr<Texture> (new Texture());
	bg->loadFromFile("Assets\\NES - Mega Man 2 - Stage Select.png");
	bool hold;
	unique_ptr<LevelSelect> levelMenu;
	
	bool restart = false;



	while (run) {
		levelMenu = unique_ptr<LevelSelect>(new LevelSelect(bg, col->checkLead(), col->checkAtomicFire(), col->checkBlade(), col->checkShield(), col->checkTornado(), col->checkBoomerang(), col->checkStopper(), col->checkBomb()));
		
		if (!restart) {
			bossName = levelMenu->loop(instance, targetFPS, bg);

			hold = levelMenu->checkA();

			//StageIntro* intro = new StageIntro(bossName, hold, bg, bossT);
			//intro->loop(instance, targetFPS);
			//delete intro;
		}

		restart = false;
		

		col->setGrounded(true);
		col->HPReset();
		col->ammoReset();
		
		if (col->getLives() < 2) {
			col->setLives(2);
		}
		//col->heal(-27);
	


		unique_ptr<abstractStage> stage = unique_ptr<abstractStage>(new abstractStage(bossName, soundCol));

		//stage->reload(string(bossName), string("0"));
			
		unique_ptr<scene> area = unique_ptr<scene>(new scene(col, move(stage), enemyT));

		beforeLevelCheck(bossName);

		if (area->loop(instance, targetFPS, soundCol, beforeTileList)) {

		//if (true){
		
			shared_ptr<Weapon> newW = updatePlayer(col, bossName);

			if (newW != NULL) {
				unique_ptr<EquipAnim> equip = unique_ptr<EquipAnim>(new EquipAnim(newW, true));
				equip->loop(instance, targetFPS);

				shared_ptr<Weapon> newI = checkItem(col, bossName);
				unique_ptr<EquipAnim> equip1 = NULL;
				if (newI != NULL) {

					DLMessage* mess = new DLMessage(equip->getTexture(), equip->getSprites(), newI);
					mess->loop(instance, targetFPS);


					equip1 = unique_ptr<EquipAnim>(new EquipAnim(newI, false));
					equip1->loop(instance, targetFPS);

					delete mess;
				}
				unique_ptr<EquipMenu> eMenu;
				if (equip1 == NULL) {
					eMenu = unique_ptr<EquipMenu>(new EquipMenu(equip->getTexture(), equip->getSprites(), equip->getText(), Vector2f(500, 650)));
				}
				else {
					eMenu = unique_ptr<EquipMenu>(new EquipMenu(equip->getTexture(), equip1->getSprites(), equip1->getText(), Vector2f(500, 650)));
				}
				bool eLoop = true;
				while (eLoop) {
					eLoop = eMenu->loop(instance, targetFPS, equip->getMusic());
					if (eLoop) {
						//This is where the password screen will be run
					}
				}
				equip.reset();
				eMenu.reset();
				equip1.reset();
				newI.reset();
				newW.reset();
			}
		}
		else {
			bool gLoop = true;
			while (gLoop) {
				unique_ptr<GameOver> gO = unique_ptr<GameOver>(new GameOver());
				gO->loop(instance, targetFPS);
				unique_ptr<GameOverMenu> gMenu = unique_ptr<GameOverMenu>(new GameOverMenu());
				
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
				gO.reset();
				gMenu.reset();
			}
		}
		area.reset();
		stage.reset();
		levelMenu.reset();

		run = instance->getWindow()->isOpen();

		//soundCol.reset();
		
	}

	//mainMenu* menu = new mainMenu();
	//menu->menu(instance, targetFPS, col);
	
	
	
	//cout << "hi";

}