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

		this->tex = tex;

		soundB = new SoundBuffer();
		soundB->loadFromFile("assets\\sound\\fly_boy.wav");
		sound = new Sound();
		sound->setBuffer(*soundB);

	}

	void spawn(list<enemy*>* enemyList, SoundCollection* soundCol) {
		FlyGuy* temp = new FlyGuy(tex, position);
		temp->initial();
		temp->setSoundPoint(&sound);

		temp->setHitSound(soundCol->getHit());

		spawned = temp;

		enemyList->push_back(temp);

	}
};