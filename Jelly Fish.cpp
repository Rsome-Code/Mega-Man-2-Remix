#include "spawn on top.cpp"
#pragma once

class JellyFish : public TempEnemy {
	using TempEnemy::TempEnemy;


	int speed = 200;

	animation* anim;
	animTimer* aTimer;

public:

	void initial() {
		mov->setRect(IntRect(345, 210, 24, 25));

		setCode("jelly fish");
		hp = 3;
		damage = 3;

		sprite->setScale(Vector2f(4, 4));

		anim = new animation(list<IntRect>{IntRect(345, 210, 24, 25), IntRect(370, 210, 24, 25)}, sprite);
		aTimer = new animTimer(anim, 8, true);

		hp = 3;
		damage = 3;

		setCode("jelly fish");
		hit = new objectHitbox(IntRect(0, 0, 24, 25), sprite);
		hurt = hit;

		deathAnim->setSprite(sprite);
		offSetList();
	}

	void initial(Texture* tex, Vector2f pos, SoundCollection* soundCol) {

		hitSound = soundCol->getHit();

		mov->setPosition(pos);
		mov->setTexture(tex);

		initial();
	
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		aTimer->run(deltaT);
		mov->move(90, deltaT, speed);

	}

	int metalDam() {
		return 3;
	}
};