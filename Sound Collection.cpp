#include <sfml/Audio.hpp>


using namespace std;
using namespace sf;
#pragma once

class SoundCollection {

	shared_ptr<SoundBuffer> landB;
	shared_ptr<Sound> landSound;

	shared_ptr<SoundBuffer> hitB;
	shared_ptr<Sound> hitSound;

	shared_ptr<SoundBuffer> yokuB;
	shared_ptr<Sound> yoku;

	shared_ptr<SoundBuffer> shootB;
	shared_ptr<Sound> shootSound;

	shared_ptr<SoundBuffer> pshootB;
	shared_ptr<Sound> pshootSound;

	shared_ptr<SoundBuffer> pressB;
	shared_ptr<Sound> pressSound;

	shared_ptr<SoundBuffer> lifeGetB;
	shared_ptr<Sound> lifeGet;

	shared_ptr<SoundBuffer> beamAttackB;
	shared_ptr<Sound> beamAttack;

	shared_ptr<SoundBuffer> dinkB;
	shared_ptr<Sound> dink;

	shared_ptr<SoundBuffer>flashB;
	shared_ptr<Sound> flash;

	shared_ptr<SoundBuffer>crashB;
	shared_ptr<Sound> crash;

	shared_ptr<SoundBuffer>airB;
	shared_ptr<Sound> air;

	shared_ptr<SoundBuffer> leafB;
	shared_ptr<Sound> leaf;

public:

	SoundCollection(shared_ptr<SoundCollection> sou) {
		throw "Cannot copy Sound Collection object.";
		delete this;
	}


	SoundCollection() {

		dinkB = shared_ptr<SoundBuffer> (new SoundBuffer());
		dinkB->loadFromFile("Assets\\sound\\dink.wav");
		dink = shared_ptr<Sound>(new Sound());
		dink->setBuffer(*dinkB);

		landB = shared_ptr<SoundBuffer> (new SoundBuffer());
		landB->loadFromFile("assets\\sound\\fly_boy.wav");
		landSound = shared_ptr<Sound>(new Sound());
		landSound->setBuffer(*landB);

		hitB = shared_ptr<SoundBuffer> (new SoundBuffer());
		hitB->loadFromFile("assets\\sound\\enemy_hit.wav");
		hitSound = shared_ptr<Sound>(new Sound());
		hitSound->setBuffer(*hitB);

		yokuB = shared_ptr<SoundBuffer> (new SoundBuffer());
		yokuB->loadFromFile("assets\\sound\\yoku.wav");
		yoku = shared_ptr<Sound>(new Sound());
		yoku->setBuffer(*yokuB);

		shootB = shared_ptr<SoundBuffer> (new SoundBuffer());
		shootB->loadFromFile("assets\\sound\\enemy_shoot.wav");
		shootSound = shared_ptr<Sound>(new Sound());
		shootSound->setBuffer(*shootB);


		pshootB = shared_ptr<SoundBuffer>(new SoundBuffer());
		pshootB->loadFromFile("assets\\sound\\buster.wav");
		pshootSound = shared_ptr<Sound>(new Sound());
		pshootSound->setBuffer(*pshootB);

		pressB = shared_ptr<SoundBuffer> (new SoundBuffer());
		pressB->loadFromFile("assets\\sound\\time_stopper.wav");
		pressSound = shared_ptr<Sound>(new Sound());
		pressSound->setBuffer(*pressB);

		lifeGetB = shared_ptr<SoundBuffer> (new SoundBuffer());
		lifeGetB->loadFromFile("assets\\sound\\gaming voice.mp3");
		lifeGet = shared_ptr<Sound>(new Sound());
		lifeGet->setBuffer(*lifeGetB);

		beamAttackB = shared_ptr<SoundBuffer> (new SoundBuffer());
		beamAttackB->loadFromFile("assets\\sound\\force_beam.wav");
		beamAttack = shared_ptr<Sound>(new Sound());
		beamAttack->setBuffer(*beamAttackB);


		flashB = shared_ptr<SoundBuffer>(new SoundBuffer());
		flashB->loadFromFile("assets\\sound\\time_stopper.wav");
		flash = shared_ptr<Sound>(new Sound());
		flash->setBuffer(*flashB);

		crashB = shared_ptr<SoundBuffer>(new SoundBuffer());
		crashB->loadFromFile("assets\\sound\\crash_bomber.wav");
		crash = shared_ptr<Sound>(new Sound());
		crash->setBuffer(*crashB);

		airB = shared_ptr<SoundBuffer>(new SoundBuffer());
		airB->loadFromFile("assets\\sound\\air_shooter.wav");
		air = shared_ptr<Sound>(new Sound());
		air->setBuffer(*airB);

		leafB = shared_ptr<SoundBuffer>(new SoundBuffer());
		leafB->loadFromFile("assets\\sound\\leaf_shield.wav");
		leaf = shared_ptr<Sound>(new Sound());
		leaf ->setBuffer(*leafB);
	}

	shared_ptr<Sound> getCrash() {
		return crash;
	}

	shared_ptr<Sound> getYoku(){
		return yoku;
	}
	shared_ptr<Sound> getHit() {
		return hitSound;
	}
	shared_ptr<Sound> getLand() {
		return landSound;
	}
	shared_ptr<Sound> getShoot() {
		return shootSound;
	}

	shared_ptr<Sound> getFlash() {
		return flash;
	}

	shared_ptr<Sound> getPress() {
		return pressSound;
	}
	shared_ptr<Sound> getLifeGet() {
		return lifeGet;
	}

	shared_ptr<Sound> getBeam() {
		return beamAttack;
	}

	shared_ptr<Sound> getDink() {
		return dink;
	}

	shared_ptr<Sound> getPlayerShoot() {
		return pshootSound;
	}

	shared_ptr<Sound> getAir() {
		return air;
	}

	shared_ptr<Sound> getLeaf() {
		return leaf;
	}
};