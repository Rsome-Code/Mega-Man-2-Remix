#include "xControl.cpp"
#include "physics Object.cpp"
#include "Player Animations.cpp"
#include "bullet.cpp"
#include "Mega Buster.cpp"

#pragma once

class pControls {
	pController* p1;
	physicsObject* sprite;
	playerAnimation* pAnim;
	float maxSpeed = 700;
	float groundAccel = 10000;
	float jumpForce = 1000;

	bool grounded = false;
	bool APressed = false;
	bool BPressed = false;
	bool LPressed = false;

	bool teleport = false;


	bool A = false;
	float ATime = 0.15;
	float tempATime = 0.1;


	bool fastFall = false;
	bool standing = false;
	float fastFallSpeed = -10000;
	float fallSpeed = -3000;

	bool jumping = false;
	bool metalBlade = false;
	bool shooting = false;
	
	bool infrontOfLadder = true;
	bool onLadder = false;
	bool ladderBelow = false;
	bool ladderAbove = true;

	list<bullet*> bullets;
	Texture* bT;
	int weapon = 0;

	
public:
	pControls(pController* p, physicsObject* s, playerAnimation* a) {
		p1 = p;
		sprite = s;
		pAnim = a;
		bT = new Texture();
		bT->loadFromFile("Assets\\Weapons.png");
		bullets = { new megaBuster(sprite, bT), new megaBuster(sprite, bT), new megaBuster(sprite, bT) };
	}

	bool checkTeleport() {
		return teleport;
	}

	bool getOnLadder() {
		return onLadder;
	}

	void setGrounded(bool b) {
		if (b == true) {
			if (grounded == false) {
				pAnim->landed(p1->checkLEFT() || p1->checkRIGHT());
				PlaySound(TEXT("Assets//Sound//land.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}
		grounded = b;

	}

	list<objectSprite*> getBullets() {
		list<objectSprite*> bul;
		for (bullet* b : bullets) {
			bul.push_back(b->getSprite());
		}
		return bul;
	}

	void checkControls(float* deltaT) {
		teleport = false;

		shooting = pAnim->getShooting();

		if (onLadder) {
			sprite->enableGravity(false);
			sprite->setVVelocity(0);
			sprite->setHVelocity(0);
		}
		else if (grounded) {
			sprite->enableGravity(false);
			sprite->setFriction(4500);
			sprite->setVVelocity(0);
		}
		else {
			sprite->enableGravity(true);
			pAnim->runJump();
			sprite->setFriction(4500);
		}


		

		if (!onLadder) {
			jumpConditions(deltaT);
		}
		shoot(deltaT);

		ladderMove(deltaT);
		


		if (!onLadder) {
			if ((!shooting || !metalBlade || !grounded)) {
				move(deltaT);
			}
			else {
				idle(deltaT);
			}
		}
		else if (p1->checkA() && !APressed) {
			onLadder = false;
			//grounded = false;
			setGrounded(false);
			APressed = true;
		}
		else if (!p1->checkA()) {
			APressed = false;
		}

		////////////////////////////////
		//Testing
		if (p1->checkL() && LPressed == false) {
			teleport = true;
			LPressed = true;
		}
		else if (!p1->checkL()) {
			LPressed = false;
		}
		////////////////////////////////////

		if (sprite->getAcceleration().x >= maxSpeed) {
			sprite->setHVelocity(maxSpeed);
		}
		else if (sprite->getAcceleration().x <= -maxSpeed) {
			sprite->setHVelocity(-maxSpeed);
		}
		/*else {
			if (sprite->getAcceleration().x > 0) {
				sprite->addHorizontalForce(-(groundAccel / 2), deltaT);
			}
			else if (sprite->getAcceleration().x < 0) {
				sprite->addHorizontalForce((groundAccel / 2), deltaT);
			}
		}*/
		if (onLadder) {
			sprite->enableGravity(false);
			
		}
	}

	void setLadder(bool b) {
		onLadder = b;
	}
	void setInfrontOfLadder(bool b) {
		infrontOfLadder = b;
	}
	void setLadderBelow(bool b) {
		ladderBelow = b;
		if (b == false) {
			onLadder = false;
		}
	}
	void setLadderAbove(bool b) {
		ladderAbove = b;
		if (b == false) {
			onLadder = false;
		}
	}

	void ladderMove(float* deltaT) {
		
		if (shooting && onLadder) {
			pAnim->climbShoot();
		}

		else if (infrontOfLadder) {
			if (p1->checkUP()) {
				if (ladderAbove) {
					if (!onLadder) {
						pAnim->ladderStart();
					}
					sprite->enableGravity(false);
					sprite->move(270, deltaT, 200);
					onLadder = true;

					pAnim->ladderAnim(deltaT);
					
					//grounded = false;
					setGrounded(false);
				}
			}
			else if (p1->checkDOWN()) {
				if (ladderBelow) {
					if (!onLadder) {
						pAnim->ladderStart();
					}
					sprite->enableGravity(false);
					sprite->move(90, deltaT, 200);
					onLadder = true;
					pAnim->ladderAnim(deltaT);
					//grounded = false;
					setGrounded(false);
				}
			}
			else if (onLadder) {
				sprite->setVVelocity(0);
				//grounded = false;
				setGrounded(false);
			}
		}
		if (p1->checkLEFT()) {
			pAnim->swapDirection(false);
		}
		else if (p1->checkRIGHT()) {
			pAnim->swapDirection(true);
		}
	}

	void move(float* deltaT) {
		if (p1->checkLEFT()) {
			sprite->addHorizontalForce(-(groundAccel), deltaT);
			//pAnim->resetIdle();
			if (grounded) {
				pAnim->toeAnim(deltaT, false);
			}
			pAnim->swapDirection(false);
			standing = false;


		}
		else if (p1->checkRIGHT()) {
			sprite->addHorizontalForce(groundAccel, deltaT);
			//pAnim->resetIdle();
			if (grounded) {
				pAnim->toeAnim(deltaT, true);
			}
			pAnim->swapDirection(true);
			standing = false;

		}
		else {
			idle(deltaT);
		}
	}

	void idle(float* deltaT) {
		pAnim->resetRun();
		pAnim->stopped();
		sprite->setFriction(10000);
		if (grounded) {
			if (standing == false) {
				pAnim->resetIdle();
				standing = true;
			}
			pAnim->idleAnim(deltaT);
		}
	}


	void shoot(float* deltaT) {
		if (p1->checkB() && !BPressed) {
			bool shot = false;
			

			for (bullet* b : bullets) {
				if (!b->getShooting()) {
					b->start(pAnim->getFacingRight());
					shot = true;
					break;
				}
			}
			if (shot) {
				pAnim->shootStart();
			}

		}

		if (p1->checkB()) {
			BPressed = true;
		}
		else {
			BPressed = false;
		}

	}

	list<bullet*> getBulletObjects() {
		return bullets;
	}

	bullet* getBulletObject() {
		return *bullets.begin();
	}

	void shootEachFrame(float* deltaT) {
		for (bullet* b : bullets) {
			if (b->getShooting()) {
				b->eachFrame(deltaT);
			}
		}
	}


	void jumpCancel() {
		fastFall = true;
		//sprite->setGravity(fastFallSpeed);
		if (sprite->getAcceleration().y > 0) {
			sprite->setVVelocity(0);
		}
	}


	void jumpConditions(float* deltaT) {
		if (grounded) {
			//sprite->setGravity(fallSpeed);
			fastFall = false;
		}

		if (p1->checkA() && !grounded && APressed != true) {
			A = true;
		}
		
		if (A) {
			tempATime = tempATime - *deltaT;
			if (tempATime <= 0|| !p1->checkA()) {
				A = false;
				tempATime = ATime;
			}
		}

		if ((p1->checkA() && grounded && APressed == false) || A && grounded) {
			jump();
			A = false;
			tempATime = ATime;
		}

		if (!p1->checkA() && !grounded && fastFall == false) {
			jumpCancel();
		}

		


		if (p1->checkA()) {
			APressed = true;
		}
		else {
			APressed = false;
			
		}
	}

	void jump() {
		sprite->setPosition(Vector2f(sprite->getPosition().x, sprite->getPosition().y - 5));
		sprite->setVVelocity(jumpForce);
		setGrounded(false);
	}

	list<objectHitbox*> getBulletHitboxes() {
		list<objectHitbox*> hits;
		for (bullet* b : bullets) {
			hits.push_back(b->getHitbox());
		}
		return hits;
	}

};