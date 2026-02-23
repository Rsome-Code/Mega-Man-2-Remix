#include "spawn top and below.cpp"
#include "drill.cpp"
#pragma once

class DrillSpawner : public SpawnAboveBelow {

	using SpawnAboveBelow::SpawnAboveBelow;

	

	void initial() {
		en = shared_ptr<Drill>(new Drill(texture, Vector2f(0, 0)));
		en->setCode("drill");
		toSpawn = 0.75;
		toSpawn_left = 0;
	}

	void spawn(list<shared_ptr<enemy>>* enemyList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {
		Vector2f startP = getSpawnPos(cam);

		shared_ptr<Drill> temp = shared_ptr<Drill>(new Drill(en->getSprite()->getTexture(), startP));
		temp->setUP(!up);

		temp->setHitSound(soundCol->getHit());

		temp->initial(en->getSprite()->getTexture(), startP, soundCol);

		enemyList->push_back(temp);
	}
};