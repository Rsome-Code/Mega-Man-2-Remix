#include "enemy.cpp"
#include "egg.cpp"
#include "temp enemy.cpp"
#pragma once


class Bird : public TempEnemy {
	using TempEnemy::TempEnemy;
	Egg* egg;

	movable* mov;
	enum State {
		NotDropped, Dropped
	};
	State state;
	float speed = 1000;
	bool first = true;

	float dropDist = 400;

	animation* flap;
	animTimer* flapTimer;

public:
	void initial() {

		mov = new movable("enemy", sprite->getTexture(), IntRect(539, 420, 18, 16), Vector2f(1200, 1200), Vector2f(4, 4), 1);
		sprite = mov;

		offSetList();

		act = false;
		display = false;

		egg = new Egg(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y + sprite->getSize().y));
		state = NotDropped;

		flap = new animation(list<IntRect>{IntRect(539, 420, 18, 16), IntRect(520, 420, 18, 16)}, sprite);
		flapTimer = new animTimer(flap, 15, true);
	}
	void initial(Vector2f pos) {

		initialPos = pos;

		mov = new movable("enemy", sprite->getTexture(), IntRect(539, 420, 18, 16), pos, Vector2f(4, 4), 1);
		sprite = mov;

		offSetList();

		act = true;
		display = true;

		egg = new Egg(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y + sprite->getSize().y));
		egg->initial();
		state = NotDropped;

		hit = new objectHitbox(IntRect(0,0, 18, 16), sprite);
		hurt = new objectHitbox(IntRect(0, 0, 18, 16), sprite);

		hp = 1;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		mov->move(180, deltaT, speed);
		if (state == NotDropped) {
			eggFollow();
			checkPDistance(p);
		}
		if (first) {
			objectList->push_back(egg);
			egg->initial();
			first = false;
		}

		flapTimer->run(deltaT);
	}

	void checkPDistance(player* p) {
		if (mov->getPosition().x <= p->getPosition().x + dropDist) {
			state = Dropped;
			egg->setDropped(true);
		}
	}

	void eggFollow() {
		egg->getSprite()->setPosition(Vector2f(sprite->getPosition().x, sprite->getPosition().y + sprite->getSize().y));
	}
};