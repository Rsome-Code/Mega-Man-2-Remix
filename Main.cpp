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
#include "neo mettool.cpp"
#include "lightning lord.cpp"
#include "lightning lord anticlock.cpp"
#include "flight exit.cpp"
#include "flight bird.cpp"
#include "giant head.cpp"
#include "flight anko.cpp"
#include "air target.cpp"
#include "air target 1.cpp"
#include "air target 2.cpp"
#include "air man.cpp"
#include "z test.cpp"
#include "file menu.cpp"
#include "new save.cpp"
#include "stationary fly platform.cpp"
#include "dragon.cpp"
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
		p->setLeaf(true);
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
	else if (levelName == "crash man") {
		p->setCrashBomb(true);
		return p->getCrashBomb();
	}
	else if (levelName == "air man") {
		p->setAir(true);
		return p->getAirShooter();
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
	if (levelName == "air man") {
		return p->getItem2();
	}
	if (levelName == "flash man") {
		return p->getItem3();
	}
	return NULL;
}


bool newSaveMenu(shared_ptr<renderer> instance, float targetFPS, shared_ptr<pController> control, string fileName, int fileNum) {

	shared_ptr<NewSaveMenu> saveMenu = shared_ptr<NewSaveMenu>(new NewSaveMenu());
	if (!saveMenu->loop(instance, targetFPS, control)) {
		return false;
	}

	string name = saveMenu->getInput();

	if (name == string("")) {
		name = "File " + to_string(fileNum);
	}

	shared_ptr<ofstream> myfile;
	myfile = shared_ptr<ofstream>(new ofstream());

	myfile->open(fileName);

	*myfile << "name," + name + "\n";
	*myfile << "lives, 2\n";
	*myfile << "E - tanks, 0\n";
	*myfile << "Heat Man,n\n";
	*myfile << "Wood Man,n\n";
	*myfile << "Bubble Man,n\n";
	*myfile << "Metal Man,n\n";
	*myfile << "Quick Man,n\n";
	*myfile << "Flash Man,n\n";
	*myfile << "Crash Man,n\n";
	* myfile << "Air Man,n\n";

	myfile->close();

}

void save(shared_ptr<player> p, string saveFile, shared_ptr<Load> l) {
	shared_ptr<ofstream> myfile;
	myfile = shared_ptr<ofstream>(new ofstream());

	

	string fileName = l->getSaveName(saveFile);

	remove(saveFile.data());

	myfile->open(saveFile);

	

	*myfile << "name," + fileName + "\n";
	*myfile << "lives," + to_string(p->getLives()) + "\n";
	*myfile << "E - tanks," + to_string(p->getETanks()) + "\n";

	if (p->checkAtomicFire()) {
		*myfile << "Heat Man,y\n";
	}
	else {
		*myfile << "Heat Man,n\n";
	}

	if (p->checkShield()) {
		*myfile << "Wood Man,y\n";
	}
	else {
		*myfile << "Wood Man,n\n";
	}

	if (p->checkLead()) {
		*myfile << "Bubble Man,y\n";
	}
	else {
		*myfile << "Bubble Man,n\n";
	}

	if (p->checkBlade()) {
		*myfile << "Metal Man,y\n";
	}
	else {
		*myfile << "Metal Man,n\n";
	}

	if (p->checkBoomerang()) {
		*myfile << "Quick Man,y\n";
	}
	else {
		*myfile << "Quick Man,n\n";
	}

	if (p->checkStopper()) {
		*myfile << "Flash Man,y\n";
	}

	else {
		*myfile << "Flash Man,n\n";
	}

	if (p->checkBomb()) {
		*myfile << "Crash Man,y\n";
	}
	else {
		*myfile << "Crash Man,n\n";
	}

	if (p->checkAir()) {
		*myfile << "Air Man,y\n";
	}
	else {
		*myfile << "Air Man,n\n";
	}

	myfile->close();
}

void developerStuff(shared_ptr<renderer> instance, float targetFPS, shared_ptr<Texture> enemyT, shared_ptr<Font> font) {

	unique_ptr<ZTest> ztest = unique_ptr<ZTest>(new ZTest());
	//ztest->loop(instance, targetFPS);
	ztest.reset();


	string bossName = "dragon";



	shared_ptr<Texture> bossT;
	bossT = shared_ptr<Texture>(new Texture());
	bossT->loadFromFile("assets\\" + bossName + ".png");


	shared_ptr<Texture> wT = shared_ptr<Texture>(new Texture());
	wT->loadFromFile("assets\\stage\\" + bossName + ".png");
	levelEditor* l = new levelEditor(wT, bossName, font);

	shared_ptr<Texture> misc = shared_ptr<Texture>(new Texture());
	misc->loadFromFile("assets\\misc\\mega buster.png");

	shared_ptr<Texture> woodBossT = shared_ptr<Texture>(new Texture());
	woodBossT->loadFromFile("assets\\wood man.png");

	shared_ptr<Texture> heatBossT = shared_ptr<Texture>(new Texture());
	heatBossT->loadFromFile("assets\\heat man.png");;

	shared_ptr<Texture> bubbleBossT = shared_ptr<Texture>(new Texture());
	bubbleBossT->loadFromFile("assets\\bubble man.png");

	shared_ptr<Texture> flashBossT = shared_ptr<Texture>(new Texture());
	flashBossT->loadFromFile("assets\\bubble man.png");

	shared_ptr<Texture> beamT = shared_ptr<Texture>(new Texture());
	beamT->loadFromFile("assets\\beam.png");

	//Object Placer setup
	//list<shared_ptr<GameObject>> woodManObList = { shared_ptr<NeoMettool>(new NeoMettool(enemyT, Vector2f(0,0))), shared_ptr<GameObject>(new Background((Colour::Colour(0, 232, 216)))), shared_ptr<GameObject>(new WoodMan(Vector2f(0,0))), shared_ptr<GameObject>(new SpawnPoint(string("chicken"))),shared_ptr<GameObject>(new SpawnPoint(string("bird"))), shared_ptr<GameObject>(new Wolf(enemyT, Vector2f(0,0))), shared_ptr<GameObject>(new Gorilla(enemyT, Vector2f(0,0))), shared_ptr<GameObject>(new Rabbit(enemyT, Vector2f(0,0))), shared_ptr<Door>(shared_ptr<Door>(new Door(bossName, Vector2f(0,0), 0))), shared_ptr<ExtraLife>(new ExtraLife(misc, Vector2f(0,0))), shared_ptr<GameObject>(new ETank(misc, Vector2f(0,0))), shared_ptr<SmallAmmo>(new SmallAmmo(misc, Vector2f(0,0))), shared_ptr<BigAmmo>(new BigAmmo(misc, Vector2f(0,0))), shared_ptr<SmallHealth>(new SmallHealth(misc, Vector2f(0,0))) , shared_ptr<BigHealth>(new BigHealth(misc, Vector2f(0,0))),  shared_ptr<bat>(new bat(enemyT, Vector2f(600, 600))), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), 0)), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), UP,0)), (shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), DOWN,0))) };
	//list <shared_ptr<GameObject>> heatManList = { shared_ptr<ExtraLife> (new ExtraLife(misc, Vector2f(0,0)), new HeatMan(Vector2f(0,0)), new SniperArmour(enemyT, Vector2f(0,0)), new BreakWall(enemyT, Vector2f(0,0)), new Springer(enemyT, Vector2f(0,0)), new TellySpawner(enemyT, Vector2f(0,0)), new FlyGuySpawner(enemyT, Vector2f(0,0)), new DisappearingTile(enemyT, Vector2f(0,0), 0), new DisappearingTile(enemyT, Vector2f(0,0), 1), new DisappearingTile(enemyT, Vector2f(0,0), 2), new DisappearingTile(enemyT, Vector2f(0,0), 3), shared_ptr<Door> (new Door(bossName, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), UP,0),  shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), DOWN,0) };
	//list <shared_ptr<GameObject>> bubbleList = {new BubbleMan(Vector2f(0,0)), new Anko(enemyT, Vector2f(0,0)), new Shrink(enemyT, Vector2f(0,0)), shared_ptr<SpawnPoint> (new SpawnPoint(string("snapper"))), new Croaker(enemyT, Vector2f(0,0)), new Crabbot(enemyT, Vector2f(0,0)), shared_ptr<GameObject>(new Background(Colour::Colour(0, 112, 236))), new FallPlatform(enemyT, Vector2f(0,0)), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), UP,0),  shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), DOWN,0) };
	//list <shared_ptr<GameObject>> metalList = {new MetalMan(Vector2f(0,0)),shared_ptr<ExtraLife> (new ExtraLife(misc, Vector2f(0,0)), new ETank(misc, Vector2f(0,0)), new PieRobot(enemyT, Vector2f(0,0)), new Blocky(enemyT, Vector2f(0,0)), shared_ptr<SpawnPoint> (new SpawnPoint(string("drill"))),  shared_ptr<Door> (new Door(bossName, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), UP,0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), DOWN,0), new Press(enemyT, Vector2f(0,0))};
	//list <shared_ptr<GameObject>> quickList = {new ETank(misc, Vector2f(0,0)), shared_ptr<BigHealth> (new BigHealth(misc, Vector2f(0,0)), shared_ptr<BigAmmo> (new BigAmmo(misc, Vector2f(0,0)), shared_ptr<ExtraLife> (new ExtraLife(misc, Vector2f(0,0)), new QuickMan(Vector2f(0,0)), new ScwormSpawn(enemyT, Vector2f(0,0)), shared_ptr<TorchGuy> (new TorchGuy (enemyT, Vector2f(0,0))), new BeamRight(beamT, Vector2f(0,0)), new BeamLeft(beamT, Vector2f(0,0)), new SniperArmour(enemyT, Vector2f(0,0)), shared_ptr<Door> (new Door(bossName, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), 0), shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), UP,0),  shared_ptr<EndFlag> (new EndFlag(enemyT, Vector2f(0,0), DOWN,0) };
	//list <shared_ptr<GameObject>> flashList = { shared_ptr <FlashMan>(new FlashMan(Vector2f(0,0))), shared_ptr<Blocky>(new Blocky(enemyT, Vector2f(0,0))), shared_ptr<ScwormSpawn>(new ScwormSpawn(enemyT, Vector2f(0,0))), shared_ptr<SniperArmour>(new SniperArmour(enemyT, Vector2f(0,0))), shared_ptr<CannonRight>(new CannonRight(enemyT, Vector2f(0,0))), shared_ptr <CrazyCannon>(new CrazyCannon(enemyT, Vector2f(0,0))), shared_ptr <ETank>(new ETank(misc, Vector2f(0,0))), shared_ptr<BigHealth>(new BigHealth(misc, Vector2f(0,0))), shared_ptr<BigAmmo>(new BigAmmo(misc, Vector2f(0,0))), shared_ptr<ExtraLife>(new ExtraLife(misc, Vector2f(0,0))), shared_ptr<FlashDoor>(new FlashDoor(bossName, Vector2f(0,0), 0)), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), 0)), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), UP,0)),  shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), DOWN,0)), shared_ptr <BreakWall>(new BreakWall(enemyT, Vector2f(0,0)))};
	//list <shared_ptr<GameObject>> crashList = { shared_ptr <FlashMan>(new FlashMan(Vector2f(0,0))), shared_ptr<Blocky>(new Blocky(enemyT, Vector2f(0,0))), shared_ptr<NeoMettool>(new NeoMettool(enemyT, Vector2f(0,0))),  shared_ptr<CannonRight>(new CannonRight(enemyT, Vector2f(0,0))), shared_ptr <CrazyCannon>(new CrazyCannon(enemyT, Vector2f(0,0))),  shared_ptr<SpawnPoint>(new SpawnPoint(string("bird"))), shared_ptr<ExtraLife>(new ExtraLife(misc, Vector2f(0,0))), shared_ptr<BigHealth>(new BigHealth(misc, Vector2f(0,0))), shared_ptr <ETank>(new ETank(misc, Vector2f(0,0))), shared_ptr<GameObject>(new Background((Colour::Colour(36, 24, 140)))), shared_ptr<GameObject>(new Background((Colour::Colour(32, 56, 236)))), shared_ptr<CrashMan>(new CrashMan(Vector2f(0,0))), shared_ptr<CheckLeft>(new CheckLeft(Vector2f(0,0))), shared_ptr<CheckRight>(new CheckRight(Vector2f(0,0))), shared_ptr<CheckUp>(new CheckUp(Vector2f(0,0))), shared_ptr<CheckDown>(new CheckDown(Vector2f(0,0))), shared_ptr<RailPlatform>(new RailPlatform(enemyT, Vector2f(0,0))), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), 0)), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), UP,0)), shared_ptr<Door>(new Door(bossName, Vector2f(0,0), 0)), shared_ptr<FlyGuySpawner>(new FlyGuySpawner(enemyT, Vector2f(0,0))), shared_ptr<TellySpawner>(new TellySpawner(enemyT, Vector2f(0,0))) };
	//list <shared_ptr<GameObject>> airList = { shared_ptr<ExtraLife>(new ExtraLife(misc, Vector2f(0,0))), shared_ptr<ETank>(new ETank(misc, Vector2f(0,0))), shared_ptr<BigHealth>(new BigHealth(misc, Vector2f(0,0))), shared_ptr<BigAmmo>(new BigAmmo(misc, Vector2f(0,0))), shared_ptr<AirMan>(new AirMan(Vector2f(0,0))), shared_ptr<AirTarget2>(new AirTarget2(Vector2f(0,0))), shared_ptr<AirTarget1>(new AirTarget1(Vector2f(0,0))), shared_ptr<AirTarget>(new AirTarget(Vector2f(0,0))), shared_ptr<FlightAnko>(new FlightAnko(enemyT, Vector2f(0,0))), shared_ptr<GiantHead>(new GiantHead(enemyT, Vector2f(0,0))), shared_ptr<FlightBird>(new FlightBird(enemyT, Vector2f(0,0))), shared_ptr<LightningLordAnticlock>(new LightningLordAnticlock(enemyT, Vector2f(0,0))), shared_ptr<FlightExit>(new FlightExit(Vector2f(0,0))), shared_ptr<Background>(new Background(Colour::Colour(60, 188, 252))), shared_ptr <LightningLord>(new LightningLord(enemyT, Vector2f(0,0))), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), RIGHT,0)), shared_ptr<Door>(new Door(bossName, Vector2f(0,0), 0)) };
	list <shared_ptr<GameObject>> dragonList = {shared_ptr<Dragon>(shared_ptr<Dragon>(new Dragon(Vector2f(0,0)))), shared_ptr<StationaryFlyPlatform>(new StationaryFlyPlatform(enemyT, Vector2f(0,0))), shared_ptr<Background>(new Background(Colour(24, 60, 92))), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), DOWN,0)), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), RIGHT,0)), shared_ptr<EndFlag>(new EndFlag(enemyT, Vector2f(0,0), UP,0)), shared_ptr<GameObject>(new SpawnPoint(string("bird"))) };

	for (shared_ptr<GameObject> o : dragonList) {
		o->initial();
	}
	ObjectPlacer* o = new ObjectPlacer(wT, bossName, dragonList);



	//Test animation setup
	//////////////////////
	list<IntRect> testAnim = list<IntRect>{ list<IntRect>{IntRect(198, 360, 28, 28), IntRect(164, 369, 32, 21)} };
	list<Vector2f> testOffset = list<Vector2f>{ Vector2f(0 * 4, 0 * 4), Vector2f(2 * 4, 16 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(3 * 4, -3 * 4), Vector2f(6 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(3 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4),Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4) };
	bool facingRight = false;

	shared_ptr<Texture> testT = shared_ptr<Texture>(new Texture());
	testT->loadFromFile("Assets\\wily bosses.png");

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
}

// Returns true when a Wily stage is selected
bool stageTypeCheck(string bossName) {

	if (bossName == "dragon" || bossName == "guts tank" || bossName == "pikopiko-kun" || bossName == "wily machine 2" || bossName == "alien") {
		return true;
	}
	return false;
}

void wilyAnim() {

}

bool levelPlayLoop(shared_ptr<renderer> instance, float targetFPS, string bossName, shared_ptr<SoundCollection> soundCol, shared_ptr<Texture> enemyT,  shared_ptr<player> col, shared_ptr<LevelSelect> levelMenu, string saveFile, shared_ptr<Load> load) {

	bool restart = true;

	while (restart) {
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

		//if (true) {

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
			while (gLoop && instance->getWindow()->isOpen()) {
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
						restart = false;
					}
				}
				gO.reset();
				gMenu.reset();
			}
		}


		area.reset();
		stage.reset();
		levelMenu.reset();

		save(col, saveFile, load);
	}

	return true;
}

bool levelSelectLoop(shared_ptr<renderer> instance, shared_ptr<LevelSelect> levelMenu, shared_ptr<Texture> bg, shared_ptr<player> col, float targetFPS, string bossName, shared_ptr<Texture> enemyT, shared_ptr<SoundCollection> soundCol, string saveFile, shared_ptr<Load> load) {
	
	bool hold = false;

	bool run = true;

	while (run) {

		run = instance->getWindow()->isOpen();

		levelMenu = shared_ptr<LevelSelect>(new LevelSelect(bg, col->checkLead(), col->checkAtomicFire(), col->checkBlade(), col->checkShield(), col->checkTornado(), col->checkBoomerang(), col->checkStopper(), col->checkBomb()));
		
		if (levelMenu->loop(instance, targetFPS, bg)) {
			bossName = levelMenu->getSelection();
		}
		else {
			return false;
		}

		hold = levelMenu->checkA();

		if (!stageTypeCheck(bossName)) {
			StageIntro* intro = new StageIntro(bossName, hold, bg);
			//Comment this out to skip the intro scene
			intro->loop(instance, targetFPS);
			delete intro;
		}
		else {
			wilyAnim();
		}
		



		levelPlayLoop(instance, targetFPS, bossName, soundCol, enemyT, col, levelMenu, saveFile, load);

		
	}
}

bool fileSelectLoop(shared_ptr<renderer> instance, shared_ptr<Load> load, string saveFile, float targetFPS, shared_ptr<player> col, shared_ptr<Music> menuMusic) {
	menuMusic->play();

	shared_ptr<FileMenu> fileMenu = shared_ptr<FileMenu>(new FileMenu(load));

	if (fileMenu->loop(instance, targetFPS, col->getControls())) {
		saveFile = fileMenu->getSaveFile();

		struct stat sb;
		if (stat(saveFile.data(), &sb) == 0) {
			load->loadSaveFile(col, saveFile);
		}
		else {
			if (!newSaveMenu(instance, targetFPS, col->getController(), saveFile, fileMenu->getSelected() + 1)) {
				return false;
				menuMusic->stop();
			}

			load->loadSaveFile(col, saveFile);
		}
	}
	else {
		//run = instance->getWindow()->isOpen();
		menuMusic->stop();
		return false;

	}

	menuMusic->stop();
	return true;
}

int main() {


	shared_ptr<SoundCollection> soundCol(new SoundCollection());
	bool run = true;

	//Set the framerate here
	double targetFPS = 120;

	//Set the resolution here
	int verticalRes = 1080;


	int horiRes = verticalRes * (float(1920) / float(1080));

	shared_ptr <RenderWindow> window = shared_ptr <RenderWindow>(new RenderWindow(VideoMode(horiRes, verticalRes), "Executable", Style::Default));
	shared_ptr<pController> p1 = shared_ptr<pController>(new pController(window));

	shared_ptr<Texture> enemyT = shared_ptr<Texture>(new Texture());
	enemyT->loadFromFile("Assets\\enemy.png");

	shared_ptr<Texture> miscT = shared_ptr<Texture>(new Texture());
	miscT->loadFromFile("Assets\\misc\\mega buster.png");

	shared_ptr<player> col = shared_ptr<player>(new player(p1, soundCol));
	shared_ptr<Load> load = shared_ptr<Load>(new Load());




	//woodManStage* wood = new woodManStage(enemyT, miscT);



	shared_ptr<Font> font = shared_ptr<Font>(new Font());
	font->loadFromFile("assets//font.otf");


	shared_ptr<RenderWindow> w = move(window);
	shared_ptr<renderer> instance = shared_ptr<renderer>(new renderer(w, false));






	developerStuff(instance, targetFPS, enemyT, font);






	shared_ptr<Texture> bg;
	bg = shared_ptr<Texture>(new Texture());
	bg->loadFromFile("Assets\\NES - Mega Man 2 - Stage Select.png");


	string bossName;


	bg = shared_ptr<Texture>(new Texture());
	bg->loadFromFile("Assets\\NES - Mega Man 2 - Stage Select.png");
	bool hold;
	shared_ptr<LevelSelect> levelMenu;

	bool restart = false;

	shared_ptr<Music> menuMusic = shared_ptr<Music>(new Music());
	menuMusic->openFromFile("assets//sound//music//Password.wav");
	menuMusic->setLoop(true);

	menuMusic->setVolume(20);
	string saveFile;


	while (run && instance->getWindow()->isOpen()) {


		if (!fileSelectLoop(instance, load, saveFile, targetFPS, col, menuMusic)) {
			continue;
		}



		if (!levelSelectLoop(instance, levelMenu, bg, col, targetFPS, bossName, enemyT, soundCol, saveFile, load)) {
			continue;
		}



		run = instance->getWindow()->isOpen();

		//soundCol.reset();

	}

	//mainMenu* menu = new mainMenu();
	//menu->menu(instance, targetFPS, col);



	cout << "hi";
}