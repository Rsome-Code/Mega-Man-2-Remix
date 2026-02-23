#include "spawn area.cpp"
#include "chicken.cpp"
#pragma once

class ChickenSpawner : public SpawnArea {

	using SpawnArea::SpawnArea;

public:
	void initial() {
		en = shared_ptr<Chicken>(new Chicken(texture, Vector2f(0, 0)));
		toSpawn = 2;
	}

	void spawn(list<shared_ptr<enemy>>* enemyList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {
		shared_ptr<Chicken> temp = shared_ptr<Chicken>(new Chicken(texture, Vector2f(0, 0)));

		//temp->getSprite()->setPosition(Vector2f(cam->getPosition().x + 1930, cam->getPosition().y + 50));
		temp->initial(Vector2f(cam->getPosition().x + 1919, cam->getPosition().y + 200));
		temp->setHitSound(soundCol->getHit());
		enemyList->push_back(temp);
	}
};