#include "enemy.cpp"
#include "temp enemy.cpp"

#pragma once

class SpawnArea : object {
	
protected:
	float startX;
	float endX;

	TempEnemy* en;

	Texture* texture;

	float toSpawn = 4;
	float toSpawn_left = 0;

public:

	SpawnArea(float start) {
		startX = start;
		
		texture = new Texture();
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

	void eachFrame(player* p, float deltaT, list<enemy*>* enemyList, camera* cam) {
		if (p->getPosition().x > startX && p->getPosition().x < endX) {
			spawnTime(deltaT, enemyList, cam);
		}
		else {
			toSpawn_left = 0.5;
		}
	}

	void spawnTime(float deltaT, list<enemy*>* enemyList, camera* cam) {
		toSpawn_left -= deltaT;
		if (toSpawn_left <= 0) {
			toSpawn_left = toSpawn;
			spawn(enemyList, cam);
		}
	}

	void setEnd(float end) {
		endX = end;
		checkPoints();
	}

	virtual void spawn(list<enemy*>* enemyList, camera* cam) {

	}

	float getStartPos() {
		return startX;
	}
	float getEndPos() {
		return endX;
	}

	enemy* getEnemy() {
		return en;
	}

	virtual void initial() = 0;
};