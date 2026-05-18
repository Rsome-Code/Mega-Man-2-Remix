#include "gobject.cpp"
#include "enemy.cpp"
#include "gremlin.cpp"
#include "spawner.cpp"
#include "head spike.cpp"
#pragma once

class GiantHead : public Spawner {

	shared_ptr<animation> appearAnim;
	shared_ptr<animTimer> appearTimer;

	float flashTime = 0.16666666666666666666666666666667;
	float flashTime_left = flashTime;

	shared_ptr<objectHitbox> hit;

	float spawnTimer = 2;
	float spawnTimer_left = 0.2;

public:

	GiantHead(shared_ptr<Texture> tex, Vector2f pos) {
		
		sprite = shared_ptr<objectSprite>(new objectSprite(tex, IntRect(404, 227, 80, 80), pos, Vector2f(4, 4)));
		code = "giant head";

		appearAnim = shared_ptr<animation> (new animation(list<IntRect>{IntRect(404, 227, 80, 80), IntRect(485, 227, 80, 80), IntRect(566, 227, 80, 80), IntRect(647, 227, 80, 80)}, sprite));
		appearTimer = shared_ptr<animTimer> (new animTimer(appearAnim, 4, false));

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 80, 80), sprite));

		code = "giant head";

		display = true;
	}


	bool firstFrame = true;
	void eachFrame(float* deltaT, shared_ptr<player> player, list<shared_ptr<enemy>>* eList, shared_ptr<camera> cam, shared_ptr<SoundCollection> soundCol) {
		
		appearTimer->run(deltaT);

		flash(deltaT);

		tileCollision(player, deltaT, eList, soundCol);

		if (firstFrame) {
			spawnSpikes(eList);
			firstFrame = false;
		}
	}

	void spawnSpikes(list<shared_ptr<enemy>>* eList) {
		shared_ptr<HeadSpike> s1 = shared_ptr<HeadSpike>(new HeadSpike(sprite->getTexture(), Vector2f(sprite->getPosition().x + (4 * 4), sprite->getPosition().y)));

		shared_ptr<HeadSpike> s2 = shared_ptr<HeadSpike>(new HeadSpike(sprite->getTexture(), Vector2f(sprite->getPosition().x + sprite->getSize().x - ((16 * 4)), sprite->getPosition().y)));
		
		s1->initial();
		s2->initial();

		eList->push_back(s1);
		eList->push_back(s2);
	}

	void tileCollision(shared_ptr<player> p, float* deltaT, list<shared_ptr<enemy>>* eList, shared_ptr<SoundCollection> soundCol) {

		if (hitboxDetect::hitboxDetection(p->getHitbox(), hit)) {
			if (p->getSprite()->getHVelocity() > 0) {
				p->getSprite()->setPosition(Vector2f(hit->getPosition().x - p->getHitbox()->getSize().x - (5 * 4), p->getSprite()->getPosition().y));
			}
			else if (p->getSprite()->getHVelocity() < 0) {
				p->getSprite()->setPosition(Vector2f(hit->getPosition().x + hit->getSize().x - (4 * 4), p->getSprite()->getPosition().y));
			}
		}

		


		float currentX = p->getSprite()->getPosition().x;

		if (!p->getTempGround()) {
			if (hitboxDetect::hitboxDetection(p->getFoot(), hit)) {

				spawnTimer_left -= *deltaT;
				if (spawnTimer_left <= 0) {
					spawnTimer_left = spawnTimer;
					spawn(eList, soundCol);
				}

				if (p->getSprite()->getAcceleration().y < 0 || p->getGrounded()) {
					p->getSprite()->setPosition(Vector2f(currentX, hit->getPosition().y - (p->getHitbox()->getSize().y + 12)));

					p->setTempGround(true);
					if (!p->getGrounded()) {
						p->setGrounded(true);
						p->setGroundedOverride(true);
					}


						
				}
				else {
					p->setGroundedOverride(false);
				}
			}
			else {
				p->setGroundedOverride(false);
			}
		}

	}
		

	

	bool spawnRight = true;
	void spawn(list<shared_ptr<enemy>>* eList, shared_ptr<SoundCollection> soundCol) {
		if (spawnRight) {
			shared_ptr<Gremlin> g = shared_ptr<Gremlin>(new Gremlin(sprite->getTexture(), Vector2f(sprite->getPosition().x + (sprite->getSize().x - (sprite->getSize().x / 4)), sprite->getPosition().y + (39 * 4))));
			spawnRight = false;

			g->setRight(true);

			g->setHitSound(soundCol->getHit());

			eList->push_back(g);
		}
		else {
			shared_ptr<Gremlin> g = shared_ptr<Gremlin>(new Gremlin(sprite->getTexture(), Vector2f(sprite->getPosition().x + (sprite->getSize().x / 4) - (16*4), sprite->getPosition().y + (39 * 4))));
			spawnRight = true;

			g->setRight(false);
			g->setHitSound(soundCol->getHit());

			eList->push_back(g);
		}
	
	}

	bool lightOn = true;

	void flash(float* deltaT) {
		flashTime_left -= *deltaT;

		if (flashTime_left <= 0) {
			flashTime_left = flashTime;

			if (lightOn) {
				sprite->setRect(IntRect(sprite->getRect().getPosition().x, 308, sprite->getRect().getSize().x, sprite->getRect().getSize().y));
				lightOn = false;
			}
			else {
				sprite->setRect(IntRect(sprite->getRect().getPosition().x, 227, sprite->getRect().getSize().x, sprite->getRect().getSize().y));
				lightOn = true;
			}
		}
	}

	
};