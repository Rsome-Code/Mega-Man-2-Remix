#include "spawner.cpp"
#include "telly.cpp"
#pragma once

class TellySpawner : public Spawner {
	using Spawner::Spawner;
	shared_ptr<Telly> toSpawn;

public:

	TellySpawner(shared_ptr<Texture> tex, Vector2f pos) {
		position = pos;
		toSpawn = shared_ptr<Telly>(new Telly(tex, Vector2f(position)));
		toSpawn->initial(Vector2f(0,0));
		sprite = toSpawn->getSprite();
		setCode(toSpawn->getCode() + "-spawn");
		sprite->setPosition(pos);

		soundB = shared_ptr<SoundBuffer> (new SoundBuffer());
		soundB->loadFromFile("Assets\\sound\\enemy_hit.wav");

		this->tex = tex;


	}

	void spawn(list<shared_ptr<enemy>>* enemyList, shared_ptr<SoundCollection> soundCol) {
		shared_ptr<Telly> temp = shared_ptr<Telly> (new Telly(tex, position));
		temp->initial(position);
		temp->setHitSound(soundCol->getHit());
		spawned = temp;
		enemyList->push_back(temp);

	}
};