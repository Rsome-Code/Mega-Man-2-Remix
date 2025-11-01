#include "spawn on top.cpp"
#include "jelly fish.cpp"
#pragma once

class JellyFishSpawner : public SpawnOnTop {

	using SpawnOnTop::SpawnOnTop;

	void initial() {
		en = new JellyFish(texture, Vector2f(0, 0));
	}


	void spawn(list<enemy*>* enemyList, camera* cam, SoundCollection* soundCol) {
		Vector2f startP = getSpawnPos(cam);

		JellyFish* temp = new JellyFish(en->getSprite()->getTexture(), startP);

		temp->setHitSound(soundCol->getHit());

		temp->initial(en->getSprite()->getTexture(), startP, soundCol);

		enemyList->push_back(temp);
	}


};