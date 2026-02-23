#include "master.cpp"
#include "boss bubble.cpp"
#include "water bullet.cpp"
#include "hitbox detector.cpp"
#pragma once

class BubbleMan : public Master{
	using Master::Master;
	IntRect idle = IntRect(1, 23, 28, 28);

	shared_ptr<animation> swimAnim;
	shared_ptr<animTimer> swimTimer;

	shared_ptr<animation> swimShoot;
	shared_ptr<animTimer> shootTimer;
	float shootDelay = 0.6;
	float shootDelay_left = 0;

	shared_ptr<animation> bubbleShootAnim;
	shared_ptr<animTimer> bubbleTimer;
	float bubbleDelay = 1;
	float bubbleDelay_left = bubbleDelay;

	shared_ptr<animation> standShoot;
	shared_ptr<animTimer> standTimer;
	shared_ptr<animation> stand;

	int shootNumber;
	int swimAngle;
	int upLeft = 215;
	int upRight = 325;

	bool swimRight;

	int xTarget;

	void initial() {
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\bubble man.png");
		phys->setTexture(t);

		phys->setRect(IntRect(148, 22, 29, 33));
		sprite = phys;

		introAnim = shared_ptr<animation>(new animation(list<IntRect>{idle, IntRect(39, 21, 25, 30), IntRect(71, 24, 28, 27)}, sprite));
		introAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(3 * 4, -2 * 4), Vector2f(0 * 4, 1 * 4)});
		introTimer = shared_ptr<animTimer> (new animTimer(introAnim, 8, false));

		swimAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(148, 22, 29, 33), IntRect(182, 22, 28, 29)}, sprite));
		swimAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, -4 * 4), Vector2f(0 * 4, 0 * 4)});
		swimTimer = shared_ptr<animTimer> (new animTimer(swimAnim, 8, true));

		swimShoot = shared_ptr<animation>(new animation(list<IntRect>{IntRect(218, 23, 28, 33), IntRect(251, 23, 28, 30)}, sprite));
		swimShoot->setOffsetList(list<Vector2f>{Vector2f(-4 * 4, -4 * 4), Vector2f(-5 * 4, -1 * 4)});
		shootTimer = shared_ptr<animTimer> (new animTimer(swimShoot, 8, true));
		state = wait;
		
		standShoot = shared_ptr<animation>(new animation(list<IntRect>{IntRect(108, 26, 28, 25), idle}, sprite));
		standShoot->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 3 * 4), Vector2f(0, 0)});
		standTimer = shared_ptr<animTimer> (new animTimer(standShoot, 8, false));
		stand = shared_ptr<animation>(new animation(idle, sprite));
		masterInitial(string("bubble man"));
		setCode("bubble man");

		damage = 5;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 29, 28), sprite));
		hurt = hit;
	}


	enum State {
		wait, standing, swimming, floating
	};
	State state = standing;

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		checkDirection(p->getSprite());

		shootDelay_left -= *deltaT;

		if (state == standing) {
			standTimer->run(deltaT);
			bubbleDelay_left -= *deltaT;
			if (bubbleDelay_left <= 0) {
				bubbleDelay_left = bubbleDelay;
				if (shootNumber > 0) {
					shootBubble(bList);
					
				}
				else {
					state = swimming;
					grounded = false;
					swimRight = faceRight;
					if (faceRight) {
						swimAngle = upRight;
					}
					else {
						swimAngle = upLeft;
					}

					xTarget = p->getSprite()->getMiddlePos().x;
					
				}
				shootNumber--;
			}
		}
		else if (state == swimming) {
			if (swim(deltaT, tileList, p->getSprite())) {
				state = floating;
			}
			if (checkShoot(p, bList)) {
				shootBullet(bList);
			}
		}
		else if (state == floating) {
			floatLoop(deltaT, tileList);
			if (checkShoot(p, bList)) {
				shootBullet(bList);
			}
		}
		else if (state == wait) {
			stand->thisFrame();
			if (p->getGrounded()) {
				standShoot->reset();
				standTimer->reset();
				standShoot->thisFrame();
				state = standing;
				shootBullet(bList);
				shootNumber = (rand() % 3) +1;
			}
		}
	}

	void setFacing(bool r) {
		if (r != faceRight) {
			swimAnim->swapAll();
			standShoot->swapAll();
			swimShoot->swapAll();
			stand->swapAll();

			if (r) {
				swimShoot->setOffsetList(list<Vector2f>{Vector2f(5 * 4, -4 * 4), Vector2f(5 * 4, -1 * 4)});
				
			}
			else {
				swimShoot->setOffsetList(list<Vector2f>{Vector2f(-4 * 4, -4 * 4), Vector2f(-5 * 4, -1 * 4)});
			}
		}

		faceRight = r;
	}

	bool swim(float* deltaT, list<shared_ptr<tile>>* tileList, shared_ptr<objectSprite> p) {
		if (shootDelay_left <= 0) {
			swimTimer->run(deltaT);
		}
		else {
			shootTimer->run(deltaT);
		}

		//tileCollision(tileList);

		phys->move(swimAngle, deltaT, 300);
		
		if (swimRight) {
			
			return (phys->getMiddlePos().x > xTarget);
		}
		else {

			return (phys->getMiddlePos().x < xTarget);
		}

		
		
	}

	void floatLoop(float* deltaT, list<shared_ptr<tile>>* tileList) {
		if (shootDelay_left <= 0) {
			swimTimer->run(deltaT);
		}
		else {
			shootTimer->run(deltaT);
		}

		phys->move(90, deltaT, 200);

		for (shared_ptr<tile> t : *tileList) {
			if (t->getGround() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
					grounded = true;
					state = wait;
				}
			}
		}
		
	}

	bool checkShoot(shared_ptr<player> p, list<shared_ptr<EnemyBullet>>* bList) {
		if (hitboxDetect::hitboxYDetection(p->getSprite()->getMiddlePos(), hit)) {
			if (shootDelay_left <= 0) {
				
				shootDelay_left = shootDelay;
				return true;
			}
		}
		return false;
	}

	void shootBullet(list<shared_ptr<EnemyBullet>>* bList) {
		shared_ptr<WaterBullet> temp;
		if (!faceRight) {
			temp = shared_ptr<WaterBullet> (new WaterBullet(sprite->getTexture(), sprite->getMiddlePos(), 180));
		}
		else {
			temp = shared_ptr<WaterBullet> (new WaterBullet(sprite->getTexture(), sprite->getMiddlePos(), 0));
		}
		temp->spriteSetup(sprite->getTexture(), sprite->getMiddlePos());
		bList->push_back(temp);

		if (state == swimming || state == floating) {
			swimShoot->thisFrame();
		}
	}
	void shootBubble(list<shared_ptr<EnemyBullet>>* bList) {
		standShoot->reset();
		standTimer->reset();
		shared_ptr<BossBubble> temp;
		
		temp = shared_ptr<BossBubble> (new BossBubble(sprite->getTexture(), sprite->getMiddlePos(), faceRight));
		
		bList->push_back(temp);
	}

	int metalDam() {
		return 4;
	}
};