#include "physics enemy.cpp"
#include "physics object.cpp"
#include "carrot.cpp"

#pragma once

class Rabbit : public PhysicsEnemy {
	using PhysicsEnemy::PhysicsEnemy;
	animation* jumpAnim;
	animation* landAnim;
	animTimer* jumpTimer;
	animTimer* landTimer;
	bool faceRight = false;

	float untilJump = 3;
	float untilJump_left = untilJump;
	float untilShoot = 0.75;
	float untilShoot_left = untilShoot;


	bool gonnaShoot = false;

	int jumpForce = 1500;

	list<Carrot*> cList;

	int maxShoot = 3;
	int shootLeft = maxShoot;

	

	void initial() {
		Texture* t = sprite->getTexture();
		//delete sprite;
		//phys = new physicsObject("enemy", t, IntRect(124, 574, 32, 37), initialPos, Vector2f(4, 4), 1);
		phys->setRect(IntRect(124, 574, 32, 37));
		sprite = phys;

		jumpAnim = new animation(list<IntRect>{IntRect(124, 574, 32, 37), IntRect(157, 576, 32, 35), IntRect(190, 571, 29, 40)}, phys);
		landAnim = new animation(list<IntRect>{ IntRect(157, 576, 32, 35), IntRect(124, 574, 32, 37)}, phys);
		jumpTimer = new animTimer(jumpAnim, 20, false);
		landTimer = new animTimer(landAnim, 20, false);

		jumpAnim->setOffsetList(list<Vector2f>{ Vector2f(0, 0), Vector2f(0, 2 * 4), Vector2f(0, 0)});
		landAnim->setOffsetList(list<Vector2f>{  Vector2f(0, 2 * 4), Vector2f(0, 0)});
		deathAnim->setSprite(sprite);
		hit = new objectHitbox(IntRect(0, 0, 32, 37), phys);
		hurt = new objectHitbox(IntRect(0, 0, 32, 37), phys);

		hp = 4;
		damage = 4;

		setCode("rabbit");

		offSetList();
		
	}


	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {
		checkDirection(p->getSprite());

		if (grounded) {
			landTimer->run(deltaT);
			phys->enableGravity(false);
			if (gonnaShoot) {
				runShoot(p->getSprite(), bList, deltaT);
			}
			else {
				
				jumpTimeRun(deltaT);
			}
		}
		else {
			phys->enableGravity(true);
			runJumping(p, deltaT, tileList);
			landAnim->reset();
		}



	}

	void runShoot(objectSprite* player, list<EnemyBullet*>* bList, float* deltaT){
		untilShoot_left -= *deltaT;

		if (untilShoot_left <= 0) {
			shoot(player, bList);
			untilShoot_left = untilShoot;
			shootLeft -= 1;
			if (shootLeft == 0) {
				shootLeft = maxShoot;
				gonnaShoot = false;
			}
		}
	}

	void shoot(objectSprite* player, list<EnemyBullet*>* bList) {
		float angle = getShootAngle(player);
		Carrot* carrot = new Carrot(sprite->getTexture(), Vector2f(sprite->getPosition().x + (sprite->getSize().x/2), sprite->getPosition().y + (sprite->getSize().y / 2) ), angle);
		cList.push_back(carrot);
		//objectList->push_back(carrot);
		bList->push_back(carrot);
	}

	float getShootAngle(objectSprite* player) {
		Vector2f pos = sprite->getMiddlePos();
		Vector2f pos1 = player->getMiddlePos();
		return Maths::getAngle(pos, pos1);
	}


	void runJumping(player* p, float* deltaT, list<tile*>* tileList) {
		jumpTimer->run(deltaT);
		phys->eachFrame(deltaT);
		
		tileCollision(tileList);

		if (grounded) {
			checkShoot(p);
		}
	}

	void checkShoot(player* p) {
		gonnaShoot = true;
	}

	void jumpTimeRun(float* deltaT) {
		untilJump_left -= *deltaT;
		if (untilJump_left <= 0) {
			
			jump();
			untilJump_left = untilJump;
		}
	}

	void jump() {
		jumpAnim->reset();
		phys->setPosition(Vector2f(sprite->getPosition().x, sprite->getPosition().y - 5));
		phys->setVVelocity(jumpForce);
		grounded = false;
		
	}

	void checkDirection(objectSprite* player) {
		if (player->getPosition().x > sprite->getPosition().x) {
			setFacing(true);
		}
		else {
			setFacing(false);
		}
	}

	void setFacing(bool right) {
		if (right != faceRight) {
			faceRight = right;
			jumpAnim->swapAll();
			landAnim->swapAll();
		}
	}
};