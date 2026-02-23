#include "snapper.cpp"
#include "spawn on top.cpp"
#pragma once

class SnapperSpawn : public SpawnOnTop {
	using SpawnOnTop::SpawnOnTop;

	void initial() {
		en = shared_ptr<Snapper>(new Snapper(texture, Vector2f(0, 0)));
	}


	void spawn(list<shared_ptr<enemy>>* enemyList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {
		Vector2f startP = getSpawnPos(cam);

		shared_ptr<Snapper> temp = shared_ptr<Snapper>(new Snapper(en->getSprite()->getTexture(), startP));

		temp->setHitSound(soundCol->getHit());

		temp->initial();

		enemyList->push_back(temp);
	}
};