#include "spawner.cpp"
#include "flyguy.cpp"
#pragma once

class FlyGuySpawner:public Spawner {
	using Spawner::Spawner;

public:

	FlyGuySpawner(Texture* tex, Vector2f pos) {
		position = pos;
		toSpawn = new FlyGuy(tex, Vector2f(position));
		toSpawn->initial();
		sprite = toSpawn->getSprite();
		setCode(toSpawn->getCode() + "-spawn");
		sprite->setPosition(pos);

		soundB = new SoundBuffer();
		soundB->loadFromFile("Assets\\sound\\enemy_hit.wav");

		this->tex = tex;
	}

	void spawn(list<enemy*>* enemyList) {
		FlyGuy* temp = new FlyGuy(tex, position);
		temp->initial();

		spawned = temp;


		enemyList->push_back(temp);

	}
};