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

	void initial() {
		mov->setRect(IntRect(0, 795, 112, 80));
		mov->setPosition(initialPos);
		hit = new objectHitbox(IntRect(18*4, 14*4, 112, (80 - 14)), sprite);
		hurt = new objectHitbox(IntRect(25*4, 0, 30, 14), sprite);
		hp = 15;
		damage = 5;

		setCode("anko");
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
		return 2;
	}
};