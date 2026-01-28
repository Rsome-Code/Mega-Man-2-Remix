#include "scworm.cpp"
#pragma once

class ScwormSpawn : public enemy {
	using enemy::enemy;
	animation* spawnAnim;
	animTimer* timer;

	float untilSpawn = 3;
	float untilSpawn_left = 0;

	int maxSpawn = 10;

public:
	void initial() {
		mov->setRect(IntRect(231, 546, 16, 8));
		spawnAnim = new animation(list<IntRect>{IntRect(231, 546, 16, 8), IntRect(248, 543, 16, 11), IntRect(265, 538, 16, 16), IntRect(282, 538, 16, 16), IntRect(299, 530, 16, 24), IntRect(318, 530, 16, 24)}, sprite);
		timer = new animTimer(spawnAnim, 8, false);
		hp = 3;
		damage = 2;
		hit = new objectHitbox(IntRect(0, 0, 16, 8), sprite);
		hurt = hit;

		mov->setPosition(initialPos);

		code = "scworm spawn";

		spawnAnim->setOffsetList(list<Vector2f>{ Vector2f(0, 0), Vector2f(0 * 4, -3 * 4), Vector2f(0 * 4, -8 * 4), Vector2f(0 * 4, -8 * 4), Vector2f(0 * 4, -16 * 4), Vector2f(0 * 4, -16 * 4)});
	}

	enum State {
		idle, spawning
	};
	State state = idle;

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		if (state == idle) {
			untilSpawn_left -= *deltaT;
			if (untilSpawn_left <= 0) {
				untilSpawn_left = untilSpawn;

				if (checkAmmount(objectList)) {
					state = spawning;
				}
			}
		}
		else {
			timer->run(deltaT);
			if (timer->isFinished()) {
				spawn(objectList);
				state = idle;
				timer->reset();
				spawnAnim->reset();
				spawnAnim->thisFrame();
			}
		}
		
	}

	bool checkAmmount(list<enemy*>* objectList) {
		int ammount = 0;
		for (enemy* e : *objectList) {
			if (e->getCode() == "scworm") {
				ammount++;
			}
		}
		if (ammount >= maxSpawn) {
			return false;
		}
		return true;
	}

	void spawn(list<enemy*>* enemyList) {
		Scworm* temp = new Scworm(sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y - 4*4));
		temp->setHitSound(hitSound);
		temp->initial();
		enemyList->push_back(temp);
	}
};