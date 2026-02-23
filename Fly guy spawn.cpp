#include "spawner.cpp"
#include "flyguy.cpp"
#pragma once

class FlyGuySpawner:public Spawner {
	using Spawner::Spawner;


public:

	FlyGuySpawner(shared_ptr<Texture> tex, Vector2f pos) {
		position = pos;
		toSpawn = shared_ptr<FlyGuy> (new FlyGuy(tex, Vector2f(position)));
		toSpawn->initial();
		sprite = toSpawn->getSprite();
		setCode(toSpawn->getCode() + "-spawn");
		sprite->setPosition(pos);

		this->tex = tex;

		soundB = shared_ptr<SoundBuffer> (new SoundBuffer());
		soundB->loadFromFile("assets\\sound\\fly_boy.wav");
		sound = shared_ptr<Sound>(new Sound());
		sound->setBuffer(*soundB);

	}

	void spawn(list<shared_ptr<enemy>>* enemyList, shared_ptr<SoundCollection> soundCol) {
		shared_ptr<FlyGuy> temp = shared_ptr<FlyGuy>(new FlyGuy(tex, position));
		temp->initial();
		temp->setSoundPoint(&sound);

		temp->setHitSound(soundCol->getHit());

		spawned = temp;

		enemyList->push_back(temp);

	}
};