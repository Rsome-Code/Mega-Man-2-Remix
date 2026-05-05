#include "master.cpp"
#pragma once

class CrashMan : public Master {

	list<IntRect> poseList = { IntRect(0, 29, 31, 24), IntRect(38, 23, 31, 30), IntRect(73, 29, 29, 24)};

	shared_ptr<animation> walkAnim;
	shared_ptr<animTimer> walkTimer;

	shared_ptr<animation> jumpAnim;

	shared_ptr<animation> shootAnim;
	shared_ptr<animTimer> shootTimer;

	void initial() {
		code = "crash man";
		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture());
		t->loadFromFile("assets\\crash man.png");

		phys->setTexture(t);
		phys->setRect(poseList.front());

		introAnim = shared_ptr<animation>(new animation(poseList, phys));
		introTimer = shared_ptr<animTimer>(new animTimer(introAnim, 8, false));
		introAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(1 * 4, -6 * 4), Vector2f(3 * 4, 0 * 4) });

		walkAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(108, 29, 32, 24), IntRect(141, 28, 18, 25), IntRect(160, 29, 24, 24), IntRect(141, 28, 18, 25)}, phys));
		walkTimer = shared_ptr<animTimer>(new animTimer(walkAnim, 8, true));
		walkAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(8 * 4, -1 * 4), Vector2f(6 * 4, 0 * 4), Vector2f(8 * 4, -1 * 4)});

		jumpAnim = shared_ptr<animation>(new animation(IntRect(191, 21, 29, 31), phys));
		jumpAnim->setOffsetList(list<Vector2f>{Vector2f(0, -7*8)});

		shootAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(228, 21, 24, 29), IntRect(256, 25, 26, 28)}, phys));
		shootTimer = shared_ptr<animTimer>(new animTimer(shootAnim, 8, false));

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0,0, 31, 24), phys));
		hurt = hit;

		masterInitial("crash bomb");

		grounded = false;
		damage = 5;
	}

	enum State {
		walk,
		jump,
	};

	State state = walk;

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		if (state == walk) {

		}


		hit->updatePos();

	}
};