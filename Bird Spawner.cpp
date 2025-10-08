#include "spawn area.cpp"
#include "bird.cpp"
#pragma once

class BirdSpawner: public SpawnArea {
	
	using SpawnArea::SpawnArea;

public:
	void initial() {
		en = new Bird(texture, Vector2f(0,0));
	}

	void spawn(list<enemy*>* enemyList, camera* cam) {
		Bird* temp = new Bird(texture, Vector2f(0, 0));
		
		//temp->getSprite()->setPosition(Vector2f(cam->getPosition().x + 1930, cam->getPosition().y + 50));
		temp->initial(Vector2f(cam->getPosition().x + 1919, cam->getPosition().y + 200));
		enemyList->push_back(temp);
	}
};