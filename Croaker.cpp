#include "enemy.cpp"
#include "petit croaker.cpp"
#pragma once

class Croaker : public enemy {
	using enemy::enemy;
	float idleTime = 1;
	float idleTime_left = idleTime;

	list<TempEnemy*> petits = {};

	int startAmmount;
	bool first;

	animation* idleAnim;
	animTimer* idleTimer;

	animation* spawnAnim;
	animTimer* spawnTimer;
	bool playSpawn = false;

	IntRect blink = IntRect(67, 354, 32, 32);
	int loopsTilBlink = 10;
	int loopsLeft = loopsTilBlink;

public:

	void initial() {
		sprite->setRect(IntRect(1, 355, 32, 32));
		sprite->setScale(Vector2f(4, 4));
		sprite->setPosition(initialPos);

		offSetList();
		setCode("croaker");

		hit = new objectHitbox(IntRect(0, 0, 32, 32), sprite);
		hurt = hit;
		hp = 8;
		damage = 3;

		first = true;

		idleAnim = new animation(list<IntRect>{IntRect(1, 355, 32, 32), IntRect(34, 354, 32, 32)}, sprite);
		idleTimer = new animTimer(idleAnim, 8, true);

		spawnAnim = new animation(list<IntRect>{IntRect(100, 354, 32, 32), IntRect(133, 355, 32, 32), IntRect(133, 355, 32, 32), IntRect(100, 354, 32, 32)}, sprite);
		spawnTimer = new animTimer(spawnAnim, 8, false);
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {

		if (first) {
			startAmmount = objectList->size();
			first = false;
		}
		if (!playSpawn) {
			if (checkList(objectList)) {
				idleTime_left -= *deltaT;
				if (idleTime_left <= 0) {
					idleTime_left = idleTime;
					spawn(objectList);
					playSpawn = true;
				}

				else {
					blinkL(deltaT);
				}
			}

			else {
				blinkL(deltaT);
			}
		}
		else {
			spawnTimer->run(deltaT);
			if (spawnTimer->isFinished()) {
				playSpawn = false;
				spawnAnim->reset();
				spawnTimer->reset();
			}
		}
	}

	bool checkList(list<enemy*>* objectList) {
		for (enemy* e : *objectList) {
			if (e->getCode() == "petit croaker") {
				return false;
			}
		}

		return true;
	}

	void blinkL(float* deltaT) {
		
		if (idleTimer->run(deltaT)) {
			loopsLeft -= 1;
			if (loopsLeft <= 0) {
				sprite->setRect(blink);
				loopsLeft = loopsTilBlink;
			}
		}
	}

	void spawn(list<enemy*>* objectList) {
		for (int i = 1; i <= 3; i++) {
			PCroaker* temp = new PCroaker(sprite->getTexture(), sprite->getMiddlePos());
			temp->initial();
			temp->setNextSpeed(temp->getDSpeed() * i);
			temp->setHitSound(hitSound);
			petits.push_back(temp);
			objectList->push_back(temp);
		}
	}
};