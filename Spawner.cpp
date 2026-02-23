#include "enemy.cpp"
#include "sound collection.cpp"
#pragma once

class Spawner : public GameObject{
protected:
	shared_ptr<Texture> tex;
	shared_ptr<enemy> toSpawn;
	Vector2f position;
	float spawnTime = 4;
	float spawnTime_left = 2.5;

	shared_ptr<enemy> spawned = NULL;

	string levelName;

public:


	Spawner() {}



	void eachFrame(float* deltaT, shared_ptr<objectSprite> player, list<shared_ptr<enemy>>* enemyList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {
		if (spawned == NULL) {
			spawnTime_left -= *deltaT;
			if (spawnTime_left <= 0) {
				spawnTime_left = spawnTime;
				spawn(enemyList, soundCol);
			}
		}
		
		else {
			bool passed = false;
			for (shared_ptr<enemy> en : *enemyList) {
				if (en == spawned) {
					passed = true;
				}
			}
			if (!passed) {
				spawned = NULL;
			}
		}

	}
	virtual void spawn(list<shared_ptr<enemy>>* enemyList, shared_ptr<SoundCollection> soundCol) {


	}

	void setPosition(Vector2f pos) {
		position = pos;
		sprite->setPosition(pos);
	}

	void setPallete(string levelName) {
		this->levelName = levelName;
	}

};