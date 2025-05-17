#include "solid Tile.cpp"

#include "Abstract Stage.cpp"


#include "Bat.cpp"
#pragma once


class woodManStage :public abstractStage{


public:
	woodManStage() {

		tileTexture = new Texture();
		tileTexture->loadFromFile("Assets//Wood man-stage.png");
		setInitialPlayer(Vector2f((15 * 4) * 16, (13 * 4) * 16));
		setInitialCamera(Vector2f(12*4*16, (2 * 4) * 16));

		enemyTexture = new Texture();
		enemyTexture->loadFromFile("Assets\\enemy.png");



		addTransition(new transition(Vector2f(-10, 10), 0));


		addTransition(new transition(Vector2f(60, 10), 0));

		this->levelName = "myfile-objects.txt";

		load();
	}
	/*woodManStage(Texture* enemyT, Texture* miscT) {
		enemyTexture = enemyT;

		setInitialCamera(Vector2f(0, (7*4)*16));
		setInitialPlayer(Vector2f((1 * 4) * 16, (18 * 4) * 16));

		tileTexture = new Texture();
		tileTexture->loadFromFile("Assets//Woodman.png");

		addMultiTile(new multiWoodManSolidTile(IntRect(Vector2i(0, 20), Vector2i(80, 5)), tileTexture));
		addMultiTile(new multiWoodManSolidTile(IntRect(Vector2i(11, 15), Vector2i(10, 3)), tileTexture));
		//addMultiTile(new multiTile(new IntRect(Vector2i(11, 15), Vector2i(10, 3)), tileTexture, 3));
		addLadder(new ladder(Vector2f(10, 13), 5, tileTexture));

		addTransition(new transition(Vector2f(0, 10), 0));


		addTransition(new transition(Vector2f(60, 10), 0));

		bat* b = new bat(enemyT, Vector2f((30 * 4) * 16, (13 * 4) * 16));
		b->initial();
		addEnemy(b);

	}*/

	



};