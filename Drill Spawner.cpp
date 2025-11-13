#include "spawn top and below.cpp"
#include "drill.cpp"
#pragma once

class DrillSpawner : public SpawnAboveBelow {

	using SpawnAboveBelow::SpawnAboveBelow;

	

	void initial() {
		en = new Drill(texture, Vector2f(0, 0));
		en->setCode("drill");
		toSpawn = 0.75;
		toSpawn_left = 0;
	}

	void spawn(list<enemy*>* enemyList, camera* cam, SoundCollection* soundCol) {
		Vector2f startP = getSpawnPos(cam);

		Drill* temp = new Drill(en->getSprite()->getTexture(), startP);
		temp->setUP(!up);

		temp->setHitSound(soundCol->getHit());

		temp->initial(en->getSprite()->getTexture(), startP, soundCol);

		enemyList->push_back(temp);
	}
};