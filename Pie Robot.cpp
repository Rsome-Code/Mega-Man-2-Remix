#include "physics enemy.cpp"
#include "gear saw.cpp"
#pragma once

class PieRobot : public PhysicsEnemy {
	using PhysicsEnemy::PhysicsEnemy;
	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	shared_ptr<GearSaw> saw;

	bool firstFrame = true;

	enum State {
		starting, balencing, falling
	};
	State state;

	int bounces = 2;
	int bounceForce = 300;
	bool moved = false;

public:

	virtual ~PieRobot() {

	}

	void initial() {
		phys->setRect(IntRect(338, 493, 25, 30));
		phys->setVVelocity(0);
		
		phys->setPosition(Vector2f(initialPos.x, initialPos.y));
		moved = true;
		

		sprite = phys;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 25, 30), sprite));
		hurt = hit;

		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(338, 493, 25, 30), IntRect(370, 494, 25, 29)}, sprite));
		timer = shared_ptr<animTimer> (new animTimer(anim, 8, true));

		hp = 1;
		damage = 3;

		saw = shared_ptr<GearSaw> (new GearSaw(sprite->getTexture(), Vector2f(initialPos.x - 4, initialPos.y)));
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

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {
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

	bool checkSaw(list<shared_ptr<enemy>>* objectList) {
		for (shared_ptr<enemy> en : *objectList) {
			if (en == saw) {
				return en->getAct();
			}
		}
	}


};