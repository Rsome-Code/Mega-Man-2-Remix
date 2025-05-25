#include "object.cpp"
#include "movable object.cpp"
#include "animate.cpp"
#include "animation Timer.cpp"
#include "enemy.cpp"
#include "Object Hitbox.cpp"
#pragma once

class bat :public enemy {
	using enemy::enemy;
	bool awake;
	bool flyFromRight;
	bool flyToPlayer;
	movable* mover;
	animation* anim;
	animTimer* aTime;

	animation* aLoop;
	animTimer* loopTimer;

	
public:
	void initial() {
		Texture* t = sprite->getTexture();
		delete sprite;
		
		mover = new movable("enemy", t, Vector2i(3, 117), Vector2i(16, 23), initialPos, Vector2f(4, 4), 1);
		anim = new animation(list<IntRect>{IntRect(3, 117, 16, 23), IntRect(41, 117, 17, 24), IntRect(71, 123, 24, 28), IntRect(134, 117, 30, 20)}, mover);
		aTime = new animTimer(anim, 20, false);

		aLoop = new animation(list<IntRect>{IntRect(134, 117, 30, 20), IntRect(170, 117, 24, 28), IntRect(206, 117, 18, 19)}, mover);
		loopTimer = new animTimer(aLoop, 20, true);
		anim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(-20, 0), Vector2f(-25, 0), Vector2f(-30, 0)});
		aLoop->setOffsetList(list<Vector2f>{ Vector2f(0, 5), Vector2f((12), 2), Vector2f(24, 0)});

		sprite = mover;
		deathAnim-> setSprite(sprite);
		hit = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 23)), true, sprite);
		hurt = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 23)), true, sprite);
		hp = 1;
		damage = 2;
		awake = false;
		flyToPlayer = false;
		offSetList();

		int temp = rand();
		temp = temp % 2;
		if (temp == 0) {
			flyFromRight = true;
		}
		else {
			flyFromRight = false;
		}

		display = false;
		act = false;

	}

	void setCode() {
		code = "e1";
	}

	int genericDam() {
		if (awake) {
			return 1;
		}
		else {
			return 0;
		}
	}

	

	void alive(objectSprite* player, float* deltaT) {

		if (!awake) {

			if (flyFromRight) {
				if (player->getPosition().x > sprite->getPosition().x - 600 && player->getPosition().x < sprite->getPosition().x) {
					awake = true;
				}
			}
			else {
				if (player->getPosition().x < sprite->getPosition().x + 500 && player->getPosition().x > sprite->getPosition().x) {
					awake = true;
				}
			}
		}
		else if (!flyToPlayer) {
			flyDown(player, deltaT);
		}
		else {
			flyForwards(deltaT);
		}

	}

	void flyDown(objectSprite* player, float* deltaT) {
		if (player->getPosition().y > sprite->getPosition().y) {
			mover->move(90, deltaT, 300);
			aTime->run(deltaT);
		}
		else {
			flyToPlayer = true;
		}

	}

	

	void flyForwards(float* deltaT) {
		if (flyFromRight) {
			mover->move(180, deltaT, 800);
		}
		else {
			mover->move(0, deltaT, 800);
		}
		loopTimer->run(deltaT);
	}


};