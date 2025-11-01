#include "snapper.cpp"
#include "spawn on top.cpp"
#pragma once

class SnapperSpawn : public SpawnOnTop {
	using SpawnOnTop::SpawnOnTop;

	void initial() {
		en = new Snapper(texture, Vector2f(0, 0));
	}


	void spawn(list<enemy*>* enemyList, camera* cam, SoundCollection* soundCol) {
		Vector2f startP = getSpawnPos(cam);

		Snapper* temp = new Snapper(en->getSprite()->getTexture(), startP);

		temp->setHitSound(soundCol->getHit());

		temp->initial();

		enemyList->push_back(temp);
	}
};