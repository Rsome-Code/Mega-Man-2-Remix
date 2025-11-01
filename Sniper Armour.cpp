#include "physics enemy.cpp"
#include "Sniper Joe.cpp"
#include "sniper bullet.cpp"
#pragma once

class SniperArmour : public PhysicsEnemy {
	using PhysicsEnemy::PhysicsEnemy;
	animation* jumpAnim;
	animTimer* jumpTimer;

	list<IntRect> frames = { IntRect(682, 476, 42, 56), IntRect(726, 480, 46, 52), IntRect(778, 468, 39, 64)};
	SoundBuffer* shootB;
	Sound* shootSound;

	SoundBuffer* landB;
	Sound* landSound;

	enum State {
		jumping, shooting, idle
	};
	State state;

	float shootDelay = 1;
	float shootDelay_left = shootDelay;
	int shootNum = 3;
	int shootNum_left = shootNum;

	float jumpForce = 1000;
	float forwardSpeed = 300;
	int angle = 0;

	float idleTime = 1;
	float idleTime_left = idleTime;


	void loadSound(SoundCollection* soundCol) {

		sound = soundCol->getLand();

		shootSound = soundCol->getShoot();
	}

	void initial() {
		phys->setPosition(initialPos);
		phys->setRect(IntRect(682, 476, 42, 56));
		phys->setScale(Vector2f(4, 4));

		hp = 10;
		damage = 5;
		shootNum_left = shootNum;
		shootDelay_left = shootDelay;
		state = idle;
		offSetList();
		jumpAnim = new animation(frames, sprite);
		jumpTimer = new animTimer(jumpAnim, 6, false);
		setCode("sniper armour");

		hit = new objectHitbox(IntRect(0, 0, 42, 56), sprite);
		hurt = hit;
		phys->enableGravity(true);

		jumpAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(-4 * 4, 4 * 4), Vector2f(3 * 4, -8 * 4)});



	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		
		checkDirection(p->getSprite());
		tileCollision(tileList);
		if (state == jumping) {
			
			if (grounded) {
				sound->play();
				jumpAnim->reset();
				jumpAnim->thisFrame();
				jumpTimer->reset();
				if (p->getGrounded()) {
					state = shooting;
					
				}
				else {
					state = idle;
				}
			}
			else {
				jumpLoop(deltaT);
			}
		}
		else if (state == shooting) {
			shootDelay_left -= *deltaT;
			if (shootDelay_left <= 0) {
				shootDelay_left = shootDelay;
				shoot(bList, p);
				shootNum_left -= 1;
				if (shootNum_left <= 0) {
					shootNum_left = shootNum;
					state = jumping;
					jump();
				}
			}
		}

		else if (state == idle) {
			phys->eachFrame(deltaT);
			idleTime_left -= *deltaT;
			if (idleTime_left <= 0) {
				idleTime_left = idleTime;
				state = jumping;
				jump();
			}


		}
		
	}
	void jump() {
		grounded = false;
		phys->setVVelocity(jumpForce);
		if (faceRight) {
			angle = 0;
		}
		else {
			angle = 180;
		}
	}
	void jumpLoop(float* deltaT) {
		jumpTimer->run(deltaT);

		if (jumpTimer->isFinished()) {
			phys->eachFrame(deltaT);
			phys->move(angle, deltaT, forwardSpeed);
		}

	}

	void spawnEnemy(list<enemy*>* enemies, SoundCollection* soundCol) {

		SniperJoe* joe = new SniperJoe(sprite->getTexture(), sprite->getPosition());
		joe->setHitB(hitB);
		joe->initial(false);
		joe->setSound(soundCol);
		enemies->push_back(joe);
	}

	void shoot(list<EnemyBullet*>* bList, player* p) {
		shootSound->play();
		Vector2f sPos;
		
		if (faceRight) {
			sPos = Vector2f(sprite->getPosition().x + sprite->getSize().x, sprite->getMiddlePos().y);
			//angle = 0;
		}
		else {
			sPos = Vector2f(sprite->getPosition().x, sprite->getMiddlePos().y);
			//angle = 180;
		}

		int angle = Maths::getAngle(sPos, p->getSprite()->getMiddlePos());

		SniperBullet* newB = new SniperBullet(sprite->getTexture(), sPos, angle);
		bList->push_back(newB);
	}


	void setFacing(bool right) {
		if (right != faceRight) {
			jumpAnim->swapAll();
			jumpAnim->thisFrame();
			if (right) {
				jumpAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(-4 * 4, 4 * 4), Vector2f(3 * 4, -8 * 4)});
			}
			else {
				jumpAnim->setOffsetList(list<Vector2f>{ Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 4 * 4), Vector2f(0 * 4, -8 * 4)});
			}
		}
		faceRight = right;
		
	}
};