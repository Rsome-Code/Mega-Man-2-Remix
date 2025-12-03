#include "physics enemy.cpp"
#include "gear saw.cpp"
#pragma once

class PieRobot : public PhysicsEnemy {
	using PhysicsEnemy::PhysicsEnemy;
	animation* anim;
	animTimer* timer;

	GearSaw* saw;

	bool firstFrame = true;

	enum State {
		starting, balencing, falling
	};
	State state;

	int bounces = 2;
	int bounceForce = 300;
	bool moved = false;

public:
	void initial() {
		phys->setRect(IntRect(338, 493, 25, 30));
		phys->setVVelocity(0);
		
		phys->setPosition(Vector2f(initialPos.x, initialPos.y));
		moved = true;
		

		sprite = phys;
		hit = new objectHitbox(IntRect(0, 0, 25, 30), sprite);
		hurt = hit;

		anim = new animation(list<IntRect>{IntRect(338, 493, 25, 30), IntRect(370, 494, 25, 29)}, sprite);
		timer = new animTimer(anim, 8, true);

		hp = 1;
		damage = 3;

		saw = new GearSaw(sprite->getTexture(), Vector2f(initialPos.x - 4, initialPos.y));
		saw->initial();
		saw->setHitSound(hitSound);
		firstFrame = true;
		state = starting;
		phys->enableGravity(true);
		bounces = 2;

		code = "pie robot";

		display = true;
		act = true;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		timer->run(deltaT);
		if (firstFrame) {
			objectList->push_back(saw);
			firstFrame = false;
		}

		if (state == starting) {
			phys->eachFrame(deltaT);
			if (hitboxDetect::hitboxDetection(hit, saw->getHitbox()) && phys->getVVelocity() < 0) {
				phys->setPosition(Vector2f(saw->getPosition().x, saw->getPosition().y - phys->getSize().y));
				
				if (bounces <= 0) {
					state = balencing;
					saw->moveNow();
				}
				else {
					phys->setVVelocity(bounceForce);
				}

				bounces--;
				
			}
		}

		else if (state == balencing){
			if (checkSaw(objectList)) {
				phys->setPosition(Vector2f(saw->getPosition().x, saw->getPosition().y - phys->getSize().y));
			}
			else {
				phys->setVVelocity(bounceForce);
				state = falling;
			}
		}

		else {
			phys->eachFrame(deltaT);
		}



		if (phys->getPosition().y > initialPos.y + 1080) {
			hp = 0;
		}
	}

	bool checkSaw(list<enemy*>* objectList) {
		for (enemy* en : *objectList) {
			if (en == saw) {
				return en->getAct();
			}
		}
	}


};