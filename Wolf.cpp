#include "enemy.cpp"
#include "wolf_fireball.cpp"
#pragma once

enum State {
	teleporting,
	shooting,
	standing

};

class Wolf :public enemy {
	using enemy::enemy;

	movable* mov;

	animation* teleportAnim;

	list<IntRect> idleAnim_down;
	list<IntRect> idleAnim_up;
	animation* idleAnim;

	list<IntRect> shootAnim_up;
	list<IntRect> shootAnim_down;
	animation* shootAnim;

	animTimer* teleportTimer;
	animTimer* idleTimer;

	State state;

	float standTime = 2;
	float standTime_left = standTime;

	float shootInterval = 0.1;
	float shootInterval_left = shootInterval;
	int shootAmount = 5;
	int shootAmount_left = shootAmount;

	float wagTime = 0.3;
	float wagTime_left = wagTime;

	bool tailUp = false;


public:
	void initial() {
		mov = new movable("enemy", sprite->getTexture(), IntRect(646, 727, 14, 80), Vector2f(initialPos.x, initialPos.y), Vector2f(4, 4));
		sprite = mov;
		hp = 15;
		damage = 4;

		mov->setSpeed(500);

		teleportAnim = new animation(list<IntRect>{IntRect(646, 727, 14, 80), IntRect(662, 760, 64, 14), IntRect(727, 751, 48, 32)}, sprite);
		teleportAnim->setOffsetList(list<Vector2f>{Vector2f(28 * 4, 0 * 4), Vector2f(4 * 4, 25 * 4), Vector2f(12 * 4, 20 * 4)});
		teleportAnim->thisFrame();
		idleAnim_down = list<IntRect>{IntRect(0, 728, 67, 59), IntRect(146, 728, 67, 59), IntRect(292, 728, 67, 59)};
		idleAnim_up = list < IntRect>{ IntRect(73, 728, 67, 59), IntRect(219, 728, 67, 59), IntRect(365, 728, 67, 59) };
		idleAnim = new animation(idleAnim_down, sprite);

		shootAnim_up = list<IntRect>{IntRect(511, 728, 67, 59)};
		shootAnim_down = list<IntRect>{IntRect(438, 728, 67, 59)};
		shootAnim = new animation(shootAnim_down, sprite);

		teleportTimer = new animTimer(teleportAnim, 15, false);
		idleTimer = new animTimer(idleAnim, 4, true);

		setCode("wolf");

		hit = new objectHitbox(IntRect(30, 45, 1, 48), sprite);
		hurt = new objectHitbox(IntRect(0, 0, 67, 59), sprite);
		state = teleporting;

		offSetList();
		dead = false;

		deathAnim->setSprite(sprite);
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		if (state == teleporting) {
			teleportLoop(deltaT, tileList);
		}
		else {
			tailAnim(deltaT);
			if (state == standing) {
				standLoop(deltaT);
			}
			else if (state == shooting) {
				shootLoop(deltaT, bList);
				//state = standing;
			}
		}
	}

	void shootLoop(float* deltaT, list<EnemyBullet*>* bList) {
		idleAnim->reset();
		shootAnim->thisFrame();
		shootInterval_left -= *deltaT;
		if (shootInterval_left <= 0) {
			shootInterval_left = shootInterval;
			shoot(bList);
			shootAmount_left -= 1;
		}

		if (shootAmount_left <= 0) {
			state = standing;
			shootAmount_left = shootAmount;
		}
	}

	void shoot(list<EnemyBullet*>* bList) {
		WolfFireBall* bullet = new WolfFireBall(this->sprite->getTexture(), Vector2f(sprite->getPosition().x, sprite->getPosition().y + 32 * 4));

		bList->push_back(bullet);
	}

	void standLoop(float* deltaT) {
		standTime_left -= *deltaT;
		
		if (standTime_left <= 0) {
			standTime_left = standTime;
			state = shooting;
		}
		if (standTime_left <= 0.25) {
			shootAnim->thisFrame();
		}
		else {
			idleTimer->run(deltaT);
		}
	}

	void tailAnim(float* deltaT) {
		wagTime_left -= *deltaT;
		if (wagTime_left <= 0) {
			wagTime_left = wagTime;

			tailUp = !tailUp;
			swapAnims(tailUp);
		}
	}

	void swapAnims(bool up) {
		if (up) {
			idleAnim->setAnim(idleAnim_up);
			shootAnim->setAnim(shootAnim_up);
		}
		else {
			idleAnim->setAnim(idleAnim_down);
		
			
			shootAnim->setAnim(shootAnim_down);
		}
	}

	void teleportLoop(float* deltaT, list<tile*>* tileList) {

		mov->move(90, deltaT);

		for (tile* t : *tileList) {
			if (t->getGround() != NULL) {
				if (groundCheck(t)) {
					state = standing;
					hit->reset(IntRect(0, 0, 67, 59));
					hit->updatePos();
				}
			}
		}
	}

	int atomicDam() {
		return 3;
	}
};