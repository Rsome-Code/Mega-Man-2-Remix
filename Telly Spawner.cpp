#include "spawner.cpp"
#include "telly.cpp"
#pragma once

class TellySpawner : public Spawner {
	using Spawner::Spawner;
	Telly* toSpawn;

public:

	TellySpawner(Texture* tex, Vector2f pos) {
		position = pos;
		toSpawn = new Telly(tex, Vector2f(position));
		toSpawn->initial(Vector2f(0,0));
		sprite = toSpawn->getSprite();
		setCode(toSpawn->getCode() + "-spawn");
		sprite->setPosition(pos);

		soundB = new SoundBuffer();
		soundB->loadFromFile("Assets\\sound\\enemy_hit.wav");

		this->tex = tex;
	}

	void spawn(list<enemy*>* enemyList) {
		Telly* temp = new Telly(tex, position);
		temp->setHitB(soundB);
		temp->initial(position);

		spawned = temp;
		enemyList->push_back(temp);

	}
};