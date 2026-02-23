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

	shared_ptr<animation> anim;
	shared_ptr<animTimer> aTime;

	shared_ptr<animation> aLoop;
	shared_ptr<animTimer> loopTimer;

	
public:

	virtual ~bat() {

	}

	void initial() {
		shared_ptr<Texture> t = sprite->getTexture();


		
		mov = shared_ptr<movable>(new movable("enemy", t, Vector2i(3, 117), Vector2i(16, 23), initialPos, Vector2f(4, 4), 1));
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(3, 117, 16, 23), IntRect(41, 117, 17, 24), IntRect(71, 123, 24, 28), IntRect(134, 117, 30, 20)}, mov));
		aTime = shared_ptr<animTimer> (new animTimer(anim, 20, false));

		aLoop = shared_ptr<animation>(new animation(list<IntRect>{IntRect(134, 117, 30, 20), IntRect(170, 117, 24, 28), IntRect(206, 117, 18, 19)}, mov));
		loopTimer = shared_ptr<animTimer> (new animTimer(aLoop, 20, true));
		anim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(-20, 0), Vector2f(-25, 0), Vector2f(-30, 0)});
		aLoop->setOffsetList(list<Vector2f>{ Vector2f(0, 5), Vector2f((12), 2), Vector2f(24, 0)});

		sprite = mov;
		deathAnim-> setSprite(sprite);
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 23)), true, sprite));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 23)), true, sprite));
		hp = 2;
		damage = 4;
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

		setCode("e1");
		dead = false;
	}



	int genericDam() {
		if (awake) {
			return 1;
		}
		else {
			return 0;
		}
	}

	

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		if (!awake) {

			if (p->getPosition().y > sprite->getPosition().y) {

				if (flyFromRight) {
					if (p->getPosition().x > sprite->getPosition().x - 600 && p->getPosition().x < sprite->getPosition().x) {
						awake = true;
					}
				}
				else {
					if (p->getPosition().x < sprite->getPosition().x + 500 && p->getPosition().x > sprite->getPosition().x) {
						awake = true;
					}
				}
			}
		}
		else if (!flyToPlayer) {
			flyDown(p->getSprite(), deltaT);
		}
		else {
			flyForwards(deltaT);
		}

	}

	void flyDown(shared_ptr<objectSprite> player, float* deltaT) {
		if (player->getPosition().y > sprite->getPosition().y) {
			mov->move(90, deltaT, 300);
			aTime->run(deltaT);
		}
		else {
			flyToPlayer = true;
		}

	}

	

	void flyForwards(float* deltaT) {
		if (flyFromRight) {
			mov->move(180, deltaT, 800);
		}
		else {
			mov->move(0, deltaT, 800);
		}
		loopTimer->run(deltaT);
	}


};