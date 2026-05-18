#include "enemy.cpp"
#include "egg.cpp"
#include "temp enemy.cpp"
#pragma once


class Bird : public TempEnemy {
	using TempEnemy::TempEnemy;
protected:
	shared_ptr<Egg> egg;

	shared_ptr<movable> mov;
	enum State {
		NotDropped, Dropped
	};
	State state;
	float speed = 1000;
	bool first = true;

	float dropDist = 400;

	shared_ptr<animation> flap;
	shared_ptr<animTimer> flapTimer;

public:

	virtual ~Bird() {

		//delete egg;
	}

	void initial() {


		mov = shared_ptr<movable>(new movable("enemy", sprite->getTexture(), IntRect(539, 420, 18, 16), Vector2f(1200, 1200), Vector2f(4, 4), 1));
		sprite = mov;

		offSetList();

		act = false;
		display = false;

		egg = shared_ptr<Egg>(new Egg(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y + sprite->getSize().y)));
		state = NotDropped;

		flap = shared_ptr<animation>(new animation(list<IntRect>{IntRect(539, 420, 18, 16), IntRect(520, 420, 18, 16)}, sprite));
		flapTimer = shared_ptr<animTimer> (new animTimer(flap, 15, true));
		setCode();
		deathAnim->setSprite(sprite);
	}
	void initial(Vector2f pos, shared_ptr<SoundCollection> soundCol) {

		hitSound = soundCol->getHit();

		initialPos = pos;

		mov = shared_ptr<movable>(new movable("enemy", sprite->getTexture(), IntRect(539, 420, 18, 16), pos, Vector2f(4, 4), 1));
		sprite = mov;

		offSetList();

		act = true;
		display = true;

		egg = shared_ptr<Egg>(new Egg(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y + sprite->getSize().y)));
		egg->initial();
		egg->setHitSound(soundCol->getHit());
		state = NotDropped;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0,0, 18, 16), sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 18, 16), sprite));

		flap = shared_ptr<animation>(new animation(list<IntRect>{IntRect(539, 420, 18, 16), IntRect(520, 420, 18, 16)}, sprite));
		flapTimer = shared_ptr<animTimer> (new animTimer(flap, 15, true));

		hp = 1;
		deathAnim->setSprite(sprite);
	}

	virtual void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
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

	void checkPDistance(shared_ptr<player> p) {
		if (mov->getPosition().x <= p->getPosition().x + dropDist) {
			state = Dropped;
			egg->setDropped(true);
		}
	}

	void eggFollow() {
		egg->getSprite()->setPosition(Vector2f(sprite->getPosition().x, sprite->getPosition().y + sprite->getSize().y));
	}

	void lowerHP(int h) {
		if (h > 0) {
			hitSound->play();
			egg->killedNoSound();
		}
		hp = hp - h;
		
	}

	void setCode() {
		code = "bird";
	}
};