#include "spawn on top.cpp"
#pragma once

class JellyFish : public TempEnemy {
	using TempEnemy::TempEnemy;


	int speed = 200;

	shared_ptr<animation> anim;
	shared_ptr<animTimer> aTimer;

public:

	virtual ~JellyFish() {

	}

	void initial() {
		mov->setRect(IntRect(345, 210, 24, 25));

		setCode("jelly fish");
		hp = 3;
		damage = 3;

		sprite->setScale(Vector2f(4, 4));

		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(345, 210, 24, 25), IntRect(370, 210, 24, 25)}, sprite));
		aTimer = shared_ptr<animTimer> (new animTimer(anim, 8, true));

		hp = 3;
		damage = 3;

		setCode("jelly fish");
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 24, 25), sprite));
		hurt = hit;

		deathAnim->setSprite(sprite);
		offSetList();
	}

	void initial(shared_ptr<Texture> tex, Vector2f pos, shared_ptr<SoundCollection> soundCol) {

		hitSound = soundCol->getHit();

		mov->setPosition(pos);
		mov->setTexture(tex);

		initial();
	
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
		aTimer->run(deltaT);
		mov->move(90, deltaT, speed);

	}

	int metalDam() {
		return 3;
	}
};