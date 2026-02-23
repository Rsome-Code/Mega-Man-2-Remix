#include "spawn on top.cpp"
#include "jelly fish.cpp"
#pragma once

class JellyFishSpawner : public SpawnOnTop {

	using SpawnOnTop::SpawnOnTop;

	void initial() {
		en = shared_ptr<JellyFish>(new JellyFish(texture, Vector2f(0, 0)));
	}


	void spawn(list<shared_ptr<enemy>>* enemyList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {
		Vector2f startP = getSpawnPos(cam);

		shared_ptr<JellyFish> temp = shared_ptr<JellyFish>(new JellyFish(en->getSprite()->getTexture(), startP));

		temp->setHitSound(soundCol->getHit());

		temp->initial(en->getSprite()->getTexture(), startP, soundCol);

		enemyList->push_back(temp);
	}


};