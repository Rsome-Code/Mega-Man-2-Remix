#include "bullet.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#include "Maths.cpp"
#include <SFML\audio.hpp>
#pragma once

class AtomicFireB : public bullet {
public:

	shared_ptr<animation> anim;
	shared_ptr<animTimer> aTimer;

	float holdTime = 0;
	float maxHold = 2;

	float minPower = 1;
	float maxPower = 5;

	float power = 1;

	float minSize = 1;
	float maxSize = 4;
	float size = 2;

	float yPosAdd = 0;
	float maxYPos = 32;

	shared_ptr<SoundBuffer> startHoldB;
	shared_ptr<SoundBuffer> midHoldB;
	shared_ptr<SoundBuffer> fullHoldB;
	shared_ptr<Sound> holdSound;

	shared_ptr<SoundBuffer> shootB;
	shared_ptr<Sound> shootSound;

	
	AtomicFireB(shared_ptr<objectSprite> o, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {
		shootTime = 1;
		shootTemp = shootTime;

		origin = o;
		sprite = shared_ptr<movable>(new movable("buster", t, Vector2i(347, 42), Vector2i(24, 24), Vector2f(0, 0), Vector2f(2, 2), 1));

		hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(24, 24)), sprite));

		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(347, 42, 24, 24), IntRect(374, 44, 20, 20)}, sprite));
		aTimer = shared_ptr<animTimer> (new animTimer(anim, 10, true));
		list<Vector2f> testOffset = list<Vector2f>{ Vector2f(0, 0), Vector2f(8, 8) };
		anim->setOffsetList(testOffset);

		holdSound = shared_ptr<Sound>(new Sound());
		startHoldB = shared_ptr<SoundBuffer> (new SoundBuffer());
		startHoldB->loadFromFile("assets\\sound\\atomic_fire_charge.wav");

		midHoldB = shared_ptr<SoundBuffer> (new SoundBuffer());
		midHoldB->loadFromFile("assets\\sound\\atomic_fire_charge_mid.wav");

		fullHoldB = shared_ptr<SoundBuffer> (new SoundBuffer());
		fullHoldB->loadFromFile("assets\\sound\\atomic_fire_full.wav");

		holdSound->setBuffer(*startHoldB);
		holdSound->setLoop(true);

		shootB = shared_ptr<SoundBuffer> (new SoundBuffer());
		shootB->loadFromFile("assets\\sound\\atomic_fire.wav");

		shootSound = shared_ptr<Sound> (new Sound());
		shootSound->setBuffer(*shootB);

		dinkSetup(soundCol);
		
	}

	bool eachFrame(float* deltaT) {

		shootTemp = shootTemp - *deltaT;
		if (shootTemp <= 0) {
			shootReset();
			return true;
		}

		if (shooting) {
			aTimer->run(deltaT);

			sprite->move(direction, deltaT, speed);

			hitbox->updatePos();
			return false;
		}
		return false;
	}

	void start(bool r) {
		holdSound->setBuffer(*startHoldB);
		holdSound->play();
		
	}

	int checkDamage(shared_ptr<object> en) {
		return (en->atomicDam() * power);
	}

	void holdSoundTime(float* deltaT) {

		if (holdTime >= maxHold && holdSound->getBuffer() == midHoldB.get()) {
			holdSound->setBuffer(*fullHoldB);
			holdSound->play();
		}
		else if (holdTime >= (maxHold/ 2) && holdSound->getBuffer() == startHoldB.get()) {
			holdSound->setBuffer(*midHoldB);
			holdSound->play();
		}
		

		

	}

	void hold(float* deltaT) {
		holdTime = holdTime + *deltaT;

		holdSoundTime(deltaT);
		
		if (holdTime <= maxHold) {
			power = Maths::map(float(0), maxHold, minPower, maxPower, holdTime);
			size = Maths::map(float(0), maxHold, minSize, maxSize, holdTime);
			yPosAdd = Maths::map(float(0), maxHold, 0, maxYPos, holdTime);
		}
		else {
			power = maxPower;
			size = maxSize;
			yPosAdd = maxYPos;
		}
	}

	void forceStop() {
		holdSound->stop();
	}

	bool release(bool r) {

		holdSound->stop();


		holdTime = 0;
		if (!shooting) {
			sprite->setScale(Vector2f(size, size));

			shootSound->play();

			shooting = true;
			float offset;
			if (r) {
				offset = 80;
			}
			else {
				offset = -24;
			}
			sprite->setPosition(Vector2f(origin->getPosition().x + offset, (origin->getPosition().y + 38) - yPosAdd));
			right = r;
			if (right) {
				direction = 0;
			}
			else {
				direction = 180;
			}
			shootTemp = shootTime;
			return true;
		}
		return false;
	}

	void resetHoldTime() {
		holdTime = 0;
		holdSound->stop();
	}

	float getHoldTime() {
		return holdTime;
	}
	float getMaxHoldTime() {
		return maxHold;
	}

};