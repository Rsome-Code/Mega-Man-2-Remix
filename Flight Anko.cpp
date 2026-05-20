#include "anko.cpp"
#pragma once

class FlightAnko : public Anko {
	using Anko::Anko;

	shared_ptr<objectSprite> platform;
	shared_ptr<animation> platAnim;
	shared_ptr<animTimer> platTimer;

	

	void initial() {

		platform = shared_ptr<objectSprite>(new objectSprite(sprite->getTexture(), IntRect(187, 335, 18, 11), Vector2f(mov->getPosition().x + 14 * 4, mov->getPosition().y + 24 * 4), Vector2f(4, 4)));
		platAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(187, 335, 18, 11), IntRect(206, 335, 18, 11)}, platform));
		platTimer = shared_ptr<animTimer>(new animTimer(platAnim, 8, true));

		mov->setRect(IntRect(0, 795, 112, 80));
		mov->setPosition(initialPos);
		mov->setVisualOffset(Vector2f(0, 0));
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(18 * 4, 16 * 4, 112, (80 - 16)), sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(25 * 4, 0, 30, 14), sprite));
		hp = 15;
		damage = 5;

		setCode();
		blinkTime_left = blinkTime;
		blinkOn = false;

		deathAnimFin = false;

		if (explos.empty()) {
			deathAnim->reset();
			for (int i = 0; i < 7; i++) {
				Vector2i posi = Vector2i(rand() % int(mov->getSize().x), rand() % int(mov->getSize().y));
				posi = Vector2i(posi.x + sprite->getPosition().x, posi.y + sprite->getPosition().y);
				shared_ptr<objectSprite> exp = shared_ptr<objectSprite>(new objectSprite(mov->getTexture(), deathAnim->getCurrentRect(), Vector2f(posi), Vector2f(4, 4)));
				explos.push_back(exp);
			}
		}


	}

	void platformLoop(float* deltaT) {
		
		platTimer->run(deltaT);
		platform->setPosition(Vector2f(mov->getPosition().x + (44 * 4), mov->getPosition().y + (79 * 4)));
		
	}


	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<GameObject>>* obList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {
		
		if (checkShrinks(objectList)) {
			spawnDelay_left -= *deltaT;
			if (spawnDelay_left <= 0) {
				spawn(objectList);
				spawnDelay_left = spawnDelay;
			}
		}


		blinkTime_left -= *deltaT;
		if (blinkTime_left <= 0) {
			blinkTime_left = blinkTime;
			blink();
		}


		moveLoop(p, deltaT);

		checkExit(obList, deltaT);

		platformLoop(deltaT);

		explosionUpdate();

	}

	void explosionUpdate() {
		for (shared_ptr<objectSprite> o : explos) {
			Vector2i posi = Vector2i(rand() % int(mov->getSize().x), rand() % int(mov->getSize().y));
			posi = Vector2i(posi.x + sprite->getPosition().x, posi.y + sprite->getPosition().y);
			o->setPosition(Vector2f(posi));
		}
	}

	int exitSpeed = 300;

	void checkExit(list<shared_ptr<GameObject>>* objectList, float* deltaT) {
		for (shared_ptr<GameObject> o : *objectList) {
			if (o->getCode() == "flight exit") {
				if (o->getSprite()->getCameraPosition().x < 1920) {
					mov->move(Angle::down, deltaT, exitSpeed);
				}
			}
		}
	}

	list<shared_ptr<objectSprite>> getExtraSprites() {
		return list<shared_ptr<objectSprite>>{platform};
	}

	void moveLoop(shared_ptr<player> p, float* deltaT) {
		
		if (mov->getCameraPosition().x < 1920-(112 * 4)) {

			mov->move(Angle::right, deltaT, p->getFlightSpeed());

		}

	}
	void uniqueDeathLoop(float* deltaT) {
		platform->setPosition(Vector2f(-9999999, -99999999));
	}

	void setCode() {
		code = "flight anko";
	}
};