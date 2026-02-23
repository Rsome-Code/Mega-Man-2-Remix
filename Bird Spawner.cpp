#include "spawn area.cpp"
#include "bird.cpp"
#pragma once

class BirdSpawner: public SpawnArea {
	
	using SpawnArea::SpawnArea;

public:
	void initial() {
		en = shared_ptr<Bird>(new Bird(texture, Vector2f(0,0)));
	}

	void spawn(list<shared_ptr<enemy>>* enemyList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {
		shared_ptr<Bird> temp = shared_ptr<Bird>(new Bird(texture, Vector2f(0, 0)));
		
		

		//temp->getSprite()->setPosition(Vector2f(cam->getPosition().x + 1930, cam->getPosition().y + 50));
		temp->initial(Vector2f(cam->getPosition().x + 1919, cam->getPosition().y + 200), soundCol);
		enemyList->push_back(temp);
	}
};