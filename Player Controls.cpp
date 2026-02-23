#include "xControl.cpp"
#include "physics Object.cpp"
#include "Player Animations.cpp"
#include "bullet.cpp"
#include "Mega Buster.cpp"
#include "weapon.cpp"
#include "atomic fire.cpp"
#include <SFML\Audio.hpp>

#pragma once

class pControls {
	shared_ptr<pController> p1;
	shared_ptr<physicsObject> sprite;
	shared_ptr<playerAnimation> pAnim;
	float maxSpeed = 450;
	
	float standardAccel = 10000;
	float groundAccel = standardAccel;
	float jumpForce = 1100;

	bool grounded = true;
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


	//list<shared_ptr<bullet>> bullets;
	shared_ptr<Weapon> weapon;
	shared_ptr<Texture> bT;
	//int weapon = 0;

	bool holding = false;

	shared_ptr<Sound> landSound;
	shared_ptr<SoundBuffer> landB;

	
public:
	pControls(shared_ptr<pController> p, shared_ptr<physicsObject> s, shared_ptr<playerAnimation> a) {
		p1 = p;
		sprite = s;
		pAnim = a;
		bT = shared_ptr<Texture> (new Texture());
		bT->loadFromFile("Assets\\Weapons.png");

		landB = shared_ptr<SoundBuffer> (new SoundBuffer());
		landB->loadFromFile("assets\\sound\\land.wav");
		landSound = shared_ptr<Sound>(new Sound());
		landSound->setBuffer(*landB);
		
	}

	float getStandardAccel() {
		return standardAccel;
	}

	void setGroundAccel(float ac) {
		groundAccel = ac;
	}

	shared_ptr<Weapon> getWeapon() {
		return weapon;
	}

	bool checkTeleport() {
		return teleport;
	}

	bool getOnLadder() {
		return onLadder;
	}

	shared_ptr<pController> getController() {
		return p1;
	}

	void setGrounded(bool b) {
		if (b == true) {
			if (grounded == false) {
				pAnim->landed(p1->checkLEFT() || p1->checkRIGHT());
				landSound->play();
			}
		}
		grounded = b;

	}

	void setThrow(bool t) {
		metalBlade = t;
	}

	list<shared_ptr<objectSprite>> getBullets() {
		list<shared_ptr<objectSprite>> bul;
		for (shared_ptr<bullet> b : weapon->getBullets()) {
			bul.push_back(b->getSprite());
		}
		return bul;
	}

	list<shared_ptr<bullet>> getRealBullets() {
		return weapon->getBullets();
	}

	list<shared_ptr<UISprite>> getUIBullets() {
		list<shared_ptr<UISprite>> bul;
		for (shared_ptr<bullet> b : weapon->getBullets()) {
			bul = b->getUISprites();
		}
		return bul;
	}

	void swapDirection() {
		pAnim->swapDirection(true);
	}

	void runWithoutControl(float* deltaT) {

		teleport = false;

		shooting = pAnim->getShooting();

		if (onLadder) {
			sprite->enableGravity(false);
			sprite->setVVelocity(0);
			sprite->setHVelocity(0);
			jumping = false;
		}
		else if (grounded) {
			sprite->enableGravity(false);
			sprite->setFriction(4500);
			sprite->setVVelocity(0);
			jumping = false;
		}
		else {
			sprite->enableGravity(true);
			pAnim->runJump();
			sprite->setFriction(4500);
			jumping = true;
		}




		//if (!onLadder) {
			//jumpConditions(deltaT);
		//}
		//shoot(deltaT);

		//ladderMove(deltaT);




	//	idle(deltaT);
		pAnim->idleAnim(deltaT);

		/*else if (p1->checkA() && !APressed) {
			onLadder = false;
			//grounded = false;
			setGrounded(false);
			APressed = true;
		}*/
		//else if (!p1->checkA()) {
			//APressed = false;
		//}



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

	void checkControls(float* deltaT, list<shared_ptr<ItemBullet>>* IBullets, float frictD) {
		teleport = false;

		shooting = pAnim->getShooting();

		if (onLadder) {
			sprite->enableGravity(false);
			sprite->setVVelocity(0);
			sprite->setHVelocity(0);
			jumping = false;
		}
		else if (grounded) {
			sprite->enableGravity(false);
			sprite->setFriction(4500 - frictD);
			sprite->setVVelocity(0);
			jumping = false;
		}
		else {
			sprite->enableGravity(true);
			pAnim->runJump();
			sprite->setFriction(4500);
			jumping = true;
		}


		

		if (!onLadder) {
			jumpConditions(deltaT);
		}
		shoot(deltaT, IBullets);

		ladderMove(deltaT);
		


		if (!onLadder) {
			if ((!shooting || !metalBlade || !grounded)) {
				move(deltaT, frictD);
			}
			else {
				idle(deltaT, frictD);
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



		weapon->checkDirection(p1->checkUP(), p1->checkRIGHT(), p1->checkDOWN(), p1->checkLEFT());

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

	void move(float* deltaT, float frictD) {
		if (!(metalBlade && shooting && grounded)) {
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
				idle(deltaT, frictD);
			}
		}
		else {
			idle(deltaT, frictD);
		}
	}
	
	bool getHold() {
		return holding;
	}
	void setHold(bool b) {
		holding = b;
	}

	void idle(float* deltaT, float frictD) {
		pAnim->resetRun();
		pAnim->stopped();
		sprite->setFriction(10000 - (frictD * 1.35));
		if (grounded) {
			if (standing == false) {
				pAnim->resetIdle();
				standing = true;
			}
			pAnim->idleAnim(deltaT);
		}
	}

	void setWeapon(shared_ptr<Weapon> weap) {
		weapon = weap;
	}

	void resetHold() {
		setHold(false);
		weapon->resetHold();
		BPressed = false;
	}


	void shoot(float* deltaT, list<shared_ptr<ItemBullet>>* IBullets) {
		if (p1->checkB() && !BPressed) {
			BPressed = true;
			if (weapon->fire(pAnim->getFacingRight()) || weapon->fire(pAnim->getFacingRight(), IBullets)) {
				pAnim->shootStart();
			}

		}

		else if (p1->checkB()) {
			if (weapon->hold(deltaT) != NULL) {
				holding = true;
			}
			
		}
		else if (!p1->checkB() && BPressed) {
			BPressed = false;
			holding = false;
			if (weapon->release(pAnim->getFacingRight())) {
				pAnim->shootStart();
			}
		}

	}


	list<shared_ptr<bullet>> getBulletObjects() {
		return weapon->getBullets();
	}

	shared_ptr<bullet> getBulletObject() {
		return *weapon->getBullets().begin();
	}

	void shootEachFrame(float* deltaT, list<shared_ptr<tile>> tileList, list<shared_ptr<ItemBullet>> ibuls) {
		weapon->tileColl(tileList);
		weapon->eachFrame(deltaT);
		weapon->checkCount(ibuls);
	}


	void jumpCancel() {
		fastFall = true;
		//sprite->setGravity(fastFallSpeed);
		if (sprite->getAcceleration().y > 0) {
			sprite->setVVelocity(0);
		}
	}

	int holdin() {
		float holdTime = weapon->getHoldTime();
		if (holding) {
			if (holdTime < weapon->getMaxHoldTime()) {
				int animFrame = ((holdTime * 1000000000) / 90000000);
				animFrame = animFrame % 3;
				animFrame = animFrame + 1;

				return animFrame * 380;

			}
			else {
				int animFrame = ((holdTime * 1000000000) / 7500000);
				animFrame = animFrame % 3;
				animFrame = animFrame + 1;

				return animFrame * 380;
			}
		}
		return 0;
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

	bool isJumping() {
		return jumping;
	}

	list<shared_ptr<objectHitbox>> getBulletHitboxes() {
		list<shared_ptr<objectHitbox>> hits;
		for (shared_ptr<bullet> b : weapon->getBullets()) {
			hits.push_back(b->getHitbox());
		}
		return hits;
	}

};