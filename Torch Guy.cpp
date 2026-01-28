#include "enemy.cpp"
#include "fire bullet.cpp"
#pragma once

class TorchGuy : public enemy {

	using enemy::enemy;

	animation* idleAnim;
	animTimer* idleTimer;

	animation* throwAnim;
	animTimer* throwTimer;

	float idleTime = 2;
	float idleTime_left = idleTime;

	FireBullet* bullet = NULL;

	enum State {
		idle, throwing
	};
	State state = idle;

	void initial() {
		mov->setRect(IntRect(638, 65, 40, 35));

		mov->setPosition(initialPos);

		idleAnim = new animation(list<IntRect>{IntRect(638, 65, 40, 35), IntRect(679, 68, 40, 32)}, sprite);
		idleTimer = new animTimer(idleAnim, 8, true);

		idleAnim->setOffsetList(list<Vector2f>{ Vector2f(0, 0), Vector2f(0 * 4, 3 * 4)});

		throwAnim = new animation(list<IntRect>{IntRect(721, 68, 39, 32), IntRect(765, 64, 44, 36)}, sprite);
		throwTimer = new animTimer(throwAnim, 8, false);
		throwAnim->setOffsetList(list<Vector2f>{ Vector2f(1 * 4, 3 * 4), Vector2f(-4 * 4, -1 * 4)});

		code = "torch guy";

		hit = new objectHitbox(IntRect(0, 0, 40, 35), sprite);
		hurt = hit;

		hp = 5;
		damage = 5;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {


		if (state == idle) {
			idleTimer->run(deltaT);
			idleTime_left -= *deltaT;
			if (idleTime_left <= 0) {
				idleTime_left = idleTime;
				state = throwing;
				
				throwAnim->thisFrame();
			}
		}
		else if (state == throwing) {
			
			if (throwTimer->isFinished()) {
				if (checkBulletGone(bList)) {
					state = idle;
					throwTimer->reset();
					throwAnim->reset();
				}
				else {
					bullet->updateDistance(p);
				}
			}
			else {
				if (throwTimer->run(deltaT)) {
					if (throwTimer->isFinished()) {
						throwFire(bList, p);
					}
				}
			}
			
		}

		
	}

	int airDam() {
		return 5;
	}

	void throwFire(list<EnemyBullet*>* bList, player* p) {
		bullet = new FireBullet(sprite->getTexture(), sprite->getPosition(), p->getPosition().x);
		bList->push_back(bullet);
	}
	bool checkBulletGone(list<EnemyBullet*>* bList) {
		for (EnemyBullet* b : *bList) {
			if (b == bullet) {
				return false;
			}
		}
		bullet = NULL;
		return true;
	}
};