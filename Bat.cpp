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
	
public:
	void initial() {
		Texture* t = sprite->getTexture();
		delete sprite;
		
		mover = new movable("enemy", t, Vector2i(3, 117), Vector2i(16, 23), initialPos, Vector2f(4, 4), 1);
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



	void alive(objectSprite* player, float* deltaT) {
		cout<<act;
		cout << ", ";
		if (!awake) {
			if (flyFromRight) {
				if (player->getPosition().x > sprite->getPosition().x - 500 && player->getPosition().x < sprite->getPosition().x) {
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
	}


};