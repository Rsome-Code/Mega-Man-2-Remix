#include "object sprite.cpp"
#include "3d circle.cpp"
#include "file.cpp"
#include "time.cpp"
#include "render logic.cpp"
#include "player.cpp"
#include "load.cpp"
#pragma once

class FileMenu {

	shared_ptr<MenuCircle> menuCircle;

	shared_ptr<File> file1;

	shared_ptr<File> file2;
	shared_ptr<File> file3;

	shared_ptr<File> file4;

	shared_ptr<File> file5;
	shared_ptr<File> file6;


	vector<shared_ptr<File>> files;
	vector<shared_ptr<File>> constFiles;


	shared_ptr<UISprite> background;

	shared_ptr<UISprite> optionMenu;
	shared_ptr<UISprite> optionCursor;

	shared_ptr<text> startText;
	shared_ptr<text> copyText;
	shared_ptr<text> deleteText;

	shared_ptr<text> copyCheck;

	bool copying = false;

	shared_ptr<Load> load;

	shared_ptr<RectangleShape> flashRect;

	Colour flashColour1;
	Colour flashColour2;

	vector<Colour> colourList;

	float flashTime = 0.1;
	float flashTime_left = flashTime;


public:

	FileMenu(shared_ptr<Load> load) {

		this->load = load;

		//levelTexture = shared_ptr<Texture>(new Texture());

		//levelTexture->loadFromFile("assets/NES - Mega Man 2 - Stage Select.png");

		shared_ptr<Texture> tex = shared_ptr<Texture>(new Texture());
		tex->loadFromFile("assets/load menu.png");

		optionMenu = shared_ptr<UISprite>(new UISprite(tex, IntRect(1, 274, 199, 54), Vector2f(0, 0), Vector2f(2, 2)));


		background = shared_ptr<UISprite>(new UISprite(tex, IntRect(0, 345, 1920, 1080), Vector2f(0,0), Vector2f(1,1)));
		

		menuCircle = shared_ptr<MenuCircle>(new MenuCircle());

		optionMenu->setCameraPosition(Vector2f(menuCircle->getCentrePoint().x - (optionMenu->getCameraSize().x / 2), 850));

		fileSetup(tex);

		optionCursor = shared_ptr<UISprite>(new UISprite(tex, IntRect(296, 155, 8, 8), Vector2f(0,0), Vector2f(2,2)));

		copyCheck = shared_ptr<text>(new text());
		copyCheck->setString("Copying!");
		copyCheck->setFont(file1->getFont());
		copyCheck->setSize(36);
		copyCheck->setPosition(Vector2f(optionMenu->getCameraPosition().x + (12 * 2), optionMenu->getCameraPosition().y + (18 * 2)));

		startText = shared_ptr<text>(new text());
		startText->setString("Start");
		startText->setFont(file1->getFont());
		startText->setSize(18);
		startText->setPosition(Vector2f(optionMenu->getCameraPosition().x + (14 * 2), optionMenu->getCameraPosition().y + (18 * 2)));

		copyText = shared_ptr<text>(new text());
		copyText->setString("Copy");
		copyText->setFont(file1->getFont());
		copyText->setSize(18);
		copyText->setPosition(Vector2f(optionMenu->getCameraPosition().x + (75 * 2), optionMenu->getCameraPosition().y + (18 * 2)));

		deleteText = shared_ptr<text>(new text());
		deleteText->setString("Delete");
		deleteText->setFont(file1->getFont());
		deleteText->setSize(18);
		deleteText->setPosition(Vector2f(optionMenu->getCameraPosition().x + (136 * 2), optionMenu->getCameraPosition().y + (18 * 2)));


		flashColour1 = flashColour1.Red();
		flashColour2 = flashColour2.Transparent();

		colourList.push_back(flashColour1);
		colourList.push_back(flashColour2);

		flashRect = shared_ptr<RectangleShape>(new RectangleShape());
		flashRect->setOutlineColor(flashColour1.getColour());
		flashRect->setFillColor(Color::Transparent);

		int rectGap = 4;

		flashRect->setSize(Vector2f((202 * 2) + (rectGap*2), (266 * 2) + (rectGap * 2)));
		flashRect->setPosition(Vector2f(menuCircle->getCentrePoint().x - (flashRect->getSize().x / 2), menuCircle->getCentrePoint().y - rectGap));
		flashRect->setOutlineThickness(4);

	}

	void fileSetup(shared_ptr<Texture> tex) {



		files.clear();
		constFiles.clear();

		file1 = shared_ptr<File>(new File(tex));

		file1->setPosition(Vector2f(file1->getPosition().x, menuCircle->getCentrePoint().y));
		//file1->setAngle(0 + ((360/3) * currentFile));

		file2 = shared_ptr<File>(new File(tex));
		file2->setPosition(Vector2f(file1->getPosition().x, menuCircle->getCentrePoint().y));
		//file2->setAngle(120 - ((360 / 3) * currentFile));

		file3 = shared_ptr<File>(new File(tex));
		file3->setPosition(Vector2f(file1->getPosition().x, menuCircle->getCentrePoint().y));
		//file3->setAngle(240 - ((360 / 3) * currentFile));

		//file4 = shared_ptr<File>(new File(tex));

		//file4->setPosition(Vector2f(file1->getPosition().x, menuCircle->getCentrePoint().y));
		//file1->setAngle(0 + ((360/3) * currentFile));

		//file5 = shared_ptr<File>(new File(tex));
		//file5->setPosition(Vector2f(file1->getPosition().x, menuCircle->getCentrePoint().y));
		//file2->setAngle(120 - ((360 / 3) * currentFile));

		//file6 = shared_ptr<File>(new File(tex));
		//file6->setPosition(Vector2f(file1->getPosition().x, menuCircle->getCentrePoint().y));
		//file3->setAngle(240 - ((360 / 3) * currentFile));



		files.push_back(file1);
		files.push_back(file2);
		files.push_back(file3);

		constFiles.push_back(file1);
		constFiles.push_back(file2);
		constFiles.push_back(file3);

		//files.push_back(file4);
		//files.push_back(file5);
		//files.push_back(file6);

		//constFiles.push_back(file4);
		//constFiles.push_back(file5);
		//constFiles.push_back(file6);


		

		int i = 0;
		for (shared_ptr<File> f : files) {
			string s = "save file-" + to_string(i) + ".txt";


			struct stat sb;
			if (stat(s.data(), &sb) == 0) {
				f->setupIcons(load->getBubble(s), load->getHeat(s), load->getMetal(s), load->getWood(s), load->getAir(s), load->getQuick(s), load->getFlash(s), load->getCrash(s));
				f->setName(load->getSaveName(s));
			}
			else {
				f->setNewFile();
			}

			i++;
		}

		positionConsistency();
	}

	
	int fastMoveSpeed = 540;
	int normalSpeed = 180;
	int moveSpeed = 0;
	bool selected = false;

	enum State {
		selectingFile, selectingOption
	};

	enum Option {
		startFile, copyFile, deleteFile
	};

	Option option = startFile;

	State state = selectingFile;

	bool loop(shared_ptr<renderer> instance, float targetRate, shared_ptr<pControls> controller) {
		shared_ptr<timer> time = shared_ptr<timer>(new timer());


		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;

		

		shared_ptr<camera> cam = shared_ptr<camera>(new camera(Vector2f(0, 0)));

		state = selectingFile;
		option = startFile;
		copying = false;

		while (instance->getWindow()->isOpen() && run) {
			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();
			}
			time->frameLimiter(targetRate, startP);
			deltaT = time->checkTimer(startP);
			start = time->timerStart();
			startP = &start;

			instance->UIDisplay(background);

			

			if (state == selectingOption) {
				optionLoop(controller);
			}
			if (state == selectingFile) {
				fileLoop(deltaT, controller, cam, instance);
			}
			reorder();

			

			for (shared_ptr<File> f : files) {

				instance->bObjectDisplayNoCalc(f->getSprites(), cam);
				instance->textDisplay(f->getText());

			}

			rectFlash(deltaT);

			instance->rectDisplay(flashRect);

			shared_ptr<RectangleShape> rect = shared_ptr<RectangleShape>(new RectangleShape());

			rect->setPosition(Vector2f(menuCircle->getCentrePoint().x, menuCircle->getCentrePoint().y));

			rect->setFillColor(Color::Red);
			rect->setSize(Vector2f(4, 4));

			//instance->rectDisplay(rect);

			if (state == selectingOption) {
				instance->UIDisplay(optionMenu);
				instance->UIDisplay(optionCursor);
				instance->textDisplay(startText);
				instance->textDisplay(copyText);
				instance->textDisplay(deleteText);
			}

			if (state == selectingFile) {
				option = startFile;
			}

			if (copying) {
				instance->textDisplay(copyCheck);
			}

			instance->getWindow()->display();
			instance->getWindow()->clear();
		}

		return selected;
	}

	int colourIt = 0;
	void rectFlash(float deltaT){

		flashTime_left -= deltaT;

		if (flashTime_left <= 0) {
			colourIt = (colourIt + 1) % colourList.size();
			flashRect->setOutlineColor(colourList[colourIt].getColour());
			flashTime_left = flashTime;
		}

	}

	void optionLoop(shared_ptr<pControls> controller) {
		controllerCheck (controller);

		if (option == startFile) {
			updateOption(startText);
		}
		else if (option == copyFile) {
			updateOption(copyText);
		}
		else if (option == deleteFile) {
			updateOption(deleteText);
		}
	}

	void updateOption(shared_ptr<text> sel) {
		optionCursor->setCameraPosition(Vector2f(sel->getPosition().x - optionCursor->getCameraSize().x - (2*2), sel->getPosition().y - (0*2)));
	}

	void fileLoop(float deltaT, shared_ptr<pControls> controller, shared_ptr<camera> cam, shared_ptr<renderer> instance) {
		controllerCheck(controller);


		

		int in = 0;

		for (shared_ptr<File> f : constFiles) {
			fileLoop(deltaT, f, in, instance, cam);

			in++;
		}
	}

	string getSaveFile() {
		return "save file-" + to_string(currentFile) + ".txt";
	}

	Option getOption() {
		return option;
	}

	int getSelected() {
		return currentFile;
	}

	bool leftPressed = true;
	bool rightPressed = true;
	bool aPressed = true;
	bool bPressed = true;
	bool startPressed = true;

	bool run = true;

	void controllerCheck(shared_ptr<pControls> con){

		shared_ptr<pController> c = con->getController();

		if (c->checkLEFT()) {
			
			if (!leftPressed) {
				if (state == selectingFile) {
					leftSpin();
				}

				else if (state == selectingOption) {
					leftSelect();
				}

				leftPressed = true;
			}
			else {
				if (state == selectingFile && moveSpeed == 0) {
					leftSpin();
				}
				
			}

			

		}
		else {
			leftPressed = false;
		}

		if (c->checkRIGHT()) {

			if (!rightPressed) {
				if (state == selectingFile) {
					rightSpin();
				}

				else if (state == selectingOption) {
					rightSelect();
				}
					
				
				rightPressed = true;
			}
			else {
				if (state == selectingFile && moveSpeed == 0) {
					rightSpin();
				}

			}

		}
		else {
			rightPressed = false;
		}

		if (c->checkA()) {
			if (!aPressed) {
				aPressed = true;

				accept();
			}
		}
		else {
			aPressed = false;
		}

		if (c->checkB()) {
			if (!bPressed) {
				if (state == selectingFile) {
					if (!copying) {
						run = false;
						selected = false;
					}
					else {
						copying = false;
					}
				}
				else {
					state = selectingFile;
				}
				bPressed = true;
			}
		}
		else {
			bPressed = false;
		}

		if (c->checkSTART()) {
			if (!startPressed) {
				startPressed = true;
				accept();
			}
		}
		else {
			startPressed = false;
		}



	}

	void rightSelect() {
		option = Option((option + 1) % 3);
	}

	void leftSelect() {
		option = Option(((option - 1) + 3) % 3);
	}

	void rightSpin() {
		if (moveSpeed <= -normalSpeed) {
			moveSpeed = -fastMoveSpeed;

			currentFile = (currentFile + 1) % files.size();
		}
		else if (moveSpeed > 0) {
			currentFile = passedFile;
			moveSpeed = -normalSpeed;
		}
		else {


			currentFile = (currentFile + 1) % files.size();


			moveSpeed = -normalSpeed;
		}
	}

	void leftSpin() {
		if (moveSpeed >= normalSpeed) {

			moveSpeed = fastMoveSpeed;

			currentFile = (currentFile - 1);
			if (currentFile < 0) {
				currentFile = files.size() - 1;
			}
		}
		else if (moveSpeed < 0) {
			currentFile = passedFile;
			moveSpeed = normalSpeed;
		}
		else {

			currentFile = (currentFile - 1);
			if (currentFile < 0) {
				currentFile = files.size() - 1;
			}


			moveSpeed = normalSpeed;

		}
	}

	int toCopy;

	void accept() {
		if (state == selectingOption) {

			if (option == startFile) {
				run = false;
				selected = true;
			}
			
			else if (option == copyFile) {
				state = selectingFile;
				copying = true;
				toCopy = currentFile;
			}

			else if (option == deleteFile) {
				deleteOption();
				state = selectingFile;
				option = startFile;
			}
			
		}

		else if (state == selectingFile) {

			if (moveSpeed == 0) {

				if (!copying) {
					if (!constFiles[currentFile]->isEmpty()) {
						state = selectingOption;
					}
					else {
						run = false;
						selected = true;
					}
				}
				else {
					copySelectedFile();
					copying = false;
				}
			}
		}
	}

	void deleteOption() {


		string fileName = "save file-" + to_string(currentFile) + ".txt";

		remove(fileName.data());
		

		fileSetup(background->getTexture());
	}

	void copySelectedFile() {

		shared_ptr<ofstream> myfile;
		myfile = shared_ptr<ofstream>(new ofstream());

		string fileName = "save file-" + to_string(currentFile) + ".txt";

		remove(fileName.data());

		myfile->open(fileName);

		shared_ptr<File> toCopyFile = constFiles[toCopy];

		*myfile << "name," + toCopyFile->getName() + "\n";
		*myfile << "lives, " + to_string(toCopyFile->lives) + "\n";
		*myfile << "E - tanks, " + to_string(toCopyFile->eTanks) + "\n";

		if (toCopyFile->heat) {
			*myfile << "Heat Man,y\n";
		}
		else {
			*myfile << "Heat Man,n\n";
		}
		if (toCopyFile->wood) {
			*myfile << "Wood Man,y\n";
		}
		else {
			*myfile << "Wood Man,n\n";
		}
		if (toCopyFile->bubble) {
			*myfile << "Bubble Man,y\n";
		}
		else {
			*myfile << "Bubble Man,n\n";
		}
		if (toCopyFile->metal) {
			*myfile << "Metal Man,y\n";
		}
		else {
			*myfile << "Metal Man,n\n";
		}
		if (toCopyFile->quick) {
			*myfile << "Quick Man,y\n";
		}
		else {
			*myfile << "Quick Man,n\n";
		}
		if (toCopyFile->flash) {
			*myfile << "Flash Man,y\n";
		}
		else {
			*myfile << "Flash Man,n\n";
		}
		if (toCopyFile->crash) {
			*myfile << "Crash Man,y\n";
		}
		else {
			*myfile << "Crash Man,n\n";
		}
		if (toCopyFile->air) {
			*myfile << "Air Man,y\n";
		}
		else {
			*myfile << "Air Man,n\n";
		}

		myfile->close();

		fileSetup(background->getTexture());
		
	}

	void reorder() {
		vector<shared_ptr<File>> temp;

		int i = 0;

		float highestZ = 0;

		int winner = 0;

		for (int j = 0; files.size(); j++) {
			for (shared_ptr<File> f : files) {

				if (f->getZ() > highestZ) {
					highestZ = f->getZ();
					winner = i;
				}

				i++;
			}
			i = 0;
			highestZ = 0;

			temp.push_back(files[winner]);
			files.erase(files.begin() + winner);
		}

		files = temp;
	}


	int currentFile = 0;
	int passedFile = 0;

	void fileLoop(float deltaT, shared_ptr<File> file, int index, shared_ptr<renderer> instance, shared_ptr<camera> cam) {
		file->setZ(menuCircle->getZDistance(file->getAngle()), instance, cam);

		file->setPosition(Vector2f(menuCircle->getXDistance(file->getAngle(), file->getSize()), file->getPosition().y));
		//file1->setPosition(Vector2f(file1->getPosition().x, menuCircle->getYDistance(file1->getAngle())));

		file->setAngle(file->getAngle() + (moveSpeed * deltaT));

		if (file->getAngle() > 360) {
			file->setAngle(0);
			if (index == currentFile) {
				if (moveSpeed > 0) {
					moveSpeed = 0;
					
					positionConsistency();
				}
			}
			passedFile = index;
		}
		else if (file->getAngle() < 0) {
			file->setAngle(360);
			if (index == currentFile) {
				if (moveSpeed < 0) {
					moveSpeed = 0;

					positionConsistency();
				}
			}
			passedFile = index;
		}

	}


	//Should not need to be altered if more or less files are added
	void positionConsistency() {
		int i = 0;
		int fileNum = files.size();
		float posDif = 360 / fileNum;

		for (shared_ptr<File> f : constFiles) {

			if (i == currentFile) {
				f->setAngle(0);
			}
			else if (i > currentFile) {
				int mult = i - currentFile;

				f->setAngle(mult * posDif);
			}
			else if (i < currentFile) {
				int dif = currentFile - i;
				int num = fileNum - dif;
				f->setAngle(num * posDif);
			}

			i++;
		}
	}
};