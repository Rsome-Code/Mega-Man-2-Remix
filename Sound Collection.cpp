#include <sfml/Audio.hpp>


using namespace std;
using namespace sf;
#pragma once

class SoundCollection {

	SoundBuffer* landB;
	Sound* landSound;

	SoundBuffer* hitB;
	Sound* hitSound;

	SoundBuffer* yokuB;
	Sound* yoku;

	SoundBuffer* shootB;
	Sound* shootSound;

	SoundBuffer* pressB;
	Sound* pressSound;

	SoundBuffer* lifeGetB;
	Sound* lifeGet;

	SoundBuffer* beamAttackB;
	Sound* beamAttack;

public:

	SoundCollection() {
		landB = new SoundBuffer();
		landB->loadFromFile("assets\\sound\\fly_boy.wav");
		landSound = new Sound();
		landSound->setBuffer(*landB);

		hitB = new SoundBuffer();
		hitB->loadFromFile("assets\\sound\\enemy_hit.wav");
		hitSound = new Sound();
		hitSound->setBuffer(*hitB);

		yokuB = new SoundBuffer();
		yokuB->loadFromFile("assets\\sound\\yoku.wav");
		yoku = new Sound();
		yoku->setBuffer(*yokuB);

		shootB = new SoundBuffer();
		shootB->loadFromFile("assets\\sound\\enemy_shoot.wav");
		shootSound = new Sound();
		shootSound->setBuffer(*shootB);

		pressB = new SoundBuffer();
		pressB->loadFromFile("assets\\sound\\time_stopper.wav");
		pressSound = new Sound();
		pressSound->setBuffer(*pressB);

		lifeGetB = new SoundBuffer();
		lifeGetB->loadFromFile("assets\\sound\\gaming voice.mp3");
		lifeGet = new Sound();
		lifeGet->setBuffer(*lifeGetB);

		beamAttackB = new SoundBuffer();
		beamAttackB->loadFromFile("assets\\sound\\force_beam.wav");
		beamAttack = new Sound();
		beamAttack->setBuffer(*beamAttackB);
	}

	Sound* getYoku(){
		return yoku;
	}
	Sound* getHit() {
		return hitSound;
	}
	Sound* getLand() {
		return landSound;
	}
	Sound* getShoot() {
		return shootSound;
	}

	Sound* getPress() {
		return pressSound;
	}
	Sound* getLifeGet() {
		return lifeGet;
	}

	Sound* getBeam() {
		return beamAttack;
	}
};