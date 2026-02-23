#include "enemy.cpp"
#include "GObject.cpp"
#include "temp enemy.cpp"
#include "spawner.cpp"

#pragma once

class SpawnArea : Spawner {
	
protected:
	float startX;
	float endX;

	shared_ptr<TempEnemy> en;

	shared_ptr<Texture> texture;

	float toSpawn = 4;
	float toSpawn_left = 0;

public:


	SpawnArea(float start) {
		startX = start;
		
		texture = shared_ptr<Texture> (new Texture());
		texture->loadFromFile("assets\\enemy.png");
		
	}
	void checkPoints() {
		if (startX > endX) {
			float temp = startX;
			startX = endX;
			endX = temp;
		}
		//en->initial();
	}

	void eachFrame(shared_ptr<player> p, float deltaT, list<shared_ptr<enemy>>* enemyList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {
		if (p->getPosition().x > startX && p->getPosition().x < endX) {
			spawnTime(deltaT, enemyList, cam, soundCol);
		}
		else {
			toSpawn_left = 0.5;
		}
	}

	void spawnTime(float deltaT, list<shared_ptr<enemy>>* enemyList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {
		toSpawn_left -= deltaT;
		if (toSpawn_left <= 0) {
			toSpawn_left = toSpawn;
			spawn(enemyList, cam, soundCol);
		}
	}

	void setEnd(float end) {
		endX = end;
		checkPoints();
	}

	virtual void spawn(list<shared_ptr<enemy>>* enemyList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {

	}

	float getStartPos() {
		return startX;
	}
	float getEndPos() {
		return endX;
	}

	shared_ptr<enemy> getEnemy() {
		return en;
	}

	virtual void initial() = 0;
};