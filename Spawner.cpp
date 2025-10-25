#include "enemy.cpp"
#pragma once

class Spawner : public GameObject{
protected:
	Texture* tex;
	enemy* toSpawn;
	Vector2f position;
	float spawnTime = 4;
	float spawnTime_left = 2.5;

	enemy* spawned = NULL;

	SoundBuffer* soundB;

public:

	Spawner(){}



	void eachFrame(float* deltaT, objectSprite* player, list<enemy*>* enemyList, camera* cam) {
		if (spawned == NULL) {
			spawnTime_left -= *deltaT;
			if (spawnTime_left <= 0) {
				spawnTime_left = spawnTime;
				spawn(enemyList);
			}
		}
		
		else {
			bool passed = false;
			for (enemy* en : *enemyList) {
				if (en == spawned) {
					passed = true;
				}
			}
			if (!passed) {
				spawned = NULL;
			}
		}

	}
	virtual void spawn(list<enemy*>* enemyList) {


	}

	void setPosition(Vector2f pos) {
		position = pos;
		sprite->setPosition(pos);
	}
};