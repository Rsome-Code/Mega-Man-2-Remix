#include "enemy.cpp"
#include "shrink.cpp"

#pragma once

class Anko : public enemy {
	using enemy::enemy;

	float spawnDelay = 2;
	float spawnDelay_left = 0;

	float blinkTime = 0.4;
	float blinkTime_left = blinkTime;
	bool blinkOn = false;

	list<objectSprite*> explos;

	bool deathAnimFin = false;
	objectSprite* currentEx = NULL;

	void initial() {
		mov->setRect(IntRect(0, 795, 112, 80));
		mov->setPosition(initialPos);
		hit = new objectHitbox(IntRect(18*4, 16*4, 112, (80 - 16)), sprite);
		hurt = new objectHitbox(IntRect(25*4, 0, 30, 14), sprite);
		hp = 15;
		damage = 5;

		setCode("anko");
		blinkTime_left = blinkTime;
		blinkOn = false;

		if (explos.empty()) {
			for (int i = 0; i < 7; i++) {
				Vector2i posi = Vector2i(rand() % int(mov->getSize().x), rand() % int(mov->getSize().y));
				posi = Vector2i(posi.x + sprite->getPosition().x, posi.y + sprite->getPosition().y);
				objectSprite* exp = new objectSprite(mov->getTexture(), deathAnim->getCurrentRect(), Vector2f(posi), Vector2f(4, 4));
				explos.push_back(exp);
			}
		}

		
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
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

	}

	void spawn(list<enemy*>* objectList) {
		
		Shrink* temp = new Shrink(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getMiddlePos().y));
		temp->initial();
			
		temp->setHitSound(hitSound);
			
		objectList->push_back(temp);
		
	}

	bool checkShrinks(list<enemy*>* enemies) {
		int num = 0;
		for (enemy* e : *enemies) {
			if (e->getCode() == "shrink") {
				num++;
			}
		}
		return num < 3;
	}

	void blink() {
		if (!blinkOn) {
			sprite->setRect(IntRect(Vector2i(sprite->getRect().getPosition().x + 113, sprite->getRect().getPosition().y), sprite->getRect().getSize()));
		}
		else {
			sprite->setRect(IntRect(Vector2i(sprite->getRect().getPosition().x - 113, sprite->getRect().getPosition().y), sprite->getRect().getSize()));
		}
		blinkOn = !blinkOn;
	}

	int metalDam() {
		return 1;
	}

	int busterDam() {
		return 2;
	}




	bool death(float* deltaT, list<enemy*>* tempEList) {
		hit->setPosition(Vector2f(-1000, 0));
		hurt->setPosition(Vector2f(-1000, 0));
		if (deathAnimFin) {
			if (!dead) {
				sprite->setPosition(Vector2f(sprite->getMiddlePos().x - (12 * 4), sprite->getMiddlePos().y - (12 * 4)));
				dead = true;
				deathAnim->reset();
			}
			

			deathAnim->thisFrame();

			if (!deathTimer->isFinished()) {
				deathTimer->run(deltaT);
			}
			else {
				return isDead(tempEList);

			}
			return false;
		}

		else {
			sprite->setRect(IntRect(456, 795, 112, 80));
			explosion(deltaT);
			return false;
		}
	}

	void explosion(float* deltaT) {
		if (currentEx == NULL) {
			if (explos.empty()) {
				deathAnimFin = true;
				deathAnim->setSprite(mov);
				deathAnim->reset();
				deathTimer->reset();
			}
			else {
				currentEx = *explos.begin();
				deathAnim->reset();
				deathTimer->reset();
				deathAnim->setSprite(currentEx);
			}
		}

		else {
			deathTimer->run(deltaT);
			if (deathTimer->isFinished()) {
				hitSound->play();
				currentEx = NULL;
				explos.pop_front();
			}
			

			
		}
	}

	objectSprite* getDamSprite() {
		if (currentEx != NULL) {
			return sprite;
		}
		return NULL;
		
	}

	objectSprite* getSprite() {
		if (currentEx != NULL) {
			return currentEx;
		}
		else {
			return sprite;
		}
	}
};