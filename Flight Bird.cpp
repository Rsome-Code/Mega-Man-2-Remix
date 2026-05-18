#include "bird.cpp"
#pragma once

class FlightBird : public Bird {
	using Bird::Bird;

	

	void initial() {


		mov = shared_ptr<movable>(new movable("enemy", sprite->getTexture(), IntRect(539, 420, 18, 16), Vector2f(1200, 1200), Vector2f(4, 4), 1));
		sprite = mov;

		mov->setPosition(initialPos);

		offSetList();

		//act = false;
		//display = false;

		egg = shared_ptr<Egg>(new Egg(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y + sprite->getSize().y)));
		state = NotDropped;

		flap = shared_ptr<animation>(new animation(list<IntRect>{IntRect(539, 420, 18, 16), IntRect(520, 420, 18, 16)}, sprite));
		flapTimer = shared_ptr<animTimer>(new animTimer(flap, 15, true));
		setCode();
		deathAnim->setSprite(sprite);
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 18, 16), sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 18, 16), sprite));
		hp = 1;
		damage = 3;

		spawned = false;
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		egg->setPosition(Vector2f(-99999, -99999));

		flapTimer->run(deltaT);

		mov->move(Angle::left, deltaT, speed);
		spawned = true;

	}
	void setCode() {
		code = "flight bird";
	}
};