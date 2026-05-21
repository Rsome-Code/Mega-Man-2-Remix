#include "master.cpp"
#include "boss crash bomb.cpp"
#pragma once

class CrashMan : public Master {
	using Master::Master;

	list<IntRect> poseList = { IntRect(1, 29, 31, 24), IntRect(38, 23, 31, 30), IntRect(73, 29, 29, 24)};

	shared_ptr<animation> walkAnim;
	shared_ptr<animTimer> walkTimer;

	shared_ptr<animation> jumpAnim;

	shared_ptr<animation> shootAnim;
	//shared_ptr<animTimer> shootTimer;

	int walkSpeed = 300;

	int jumpForce = 1500;
	int jumpSpeed = 300;

	int fireSpeed = 700;

	shared_ptr<Sound> bulletLandSound;
	shared_ptr<Sound> bulletExplodeSound;

	void loadSound(shared_ptr<SoundCollection> soundCol) {
		bulletLandSound = soundCol->getCrash();
		bulletExplodeSound = soundCol->getHit();
	}



	void initial() {
		code = "crash man";
		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture());
		t->loadFromFile("assets\\crash man.png");

		phys->setTexture(t);
		phys->setRect(poseList.front());

		introAnim = shared_ptr<animation>(new animation(poseList, phys));
		introTimer = shared_ptr<animTimer>(new animTimer(introAnim, 8, false));
		introAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(1 * 4, -6 * 4), Vector2f(3 * 4, 0 * 4) });

		walkAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(108, 29, 32, 24), IntRect(141, 28, 18, 25), IntRect(160, 29, 24, 24), IntRect(141, 28, 18, 25)}, phys));
		walkTimer = shared_ptr<animTimer>(new animTimer(walkAnim, 8, true));
		walkAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(8 * 4, -1 * 4), Vector2f(6 * 4, 0 * 4), Vector2f(8 * 4, -1 * 4)});

		jumpAnim = shared_ptr<animation>(new animation(IntRect(191, 21, 29, 31), phys));
		jumpAnim->setOffsetList(list<Vector2f>{Vector2f(0, -7*8)});

		shootAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(228, 21, 24, 29), IntRect(256, 25, 26, 28)}, phys));
		//shootTimer = shared_ptr<animTimer>(new animTimer(shootAnim, 8, false));

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0,0, 31, 24), phys));
		hurt = hit;

		masterInitial("crash bomb");

		grounded = false;
		damage = 5;
		state = walk;
	}

	enum State {
		walk,
		jumping,
	};

	State state;

	void jump(shared_ptr<player> p) {

		checkDirection(p->getSprite());

		phys->setVVelocity(jumpForce);
	}

	void walkLoop(shared_ptr<player> p, float* deltaT) {
		walkTimer->run(deltaT);
		if (faceRight) {
			phys->move(0, deltaT, walkSpeed);
		}
		else {
			phys->move(180, deltaT, walkSpeed);
		}

		if (p->getController()->checkB()) {
			jump(p);
			state = jumping;
			jumpAnim->thisFrame();
		}
	}

	bool shot = false;

	void jumpLoop(list<shared_ptr<EnemyBullet>>* bList, float* deltaT, list<shared_ptr<tile>>* tileList, shared_ptr<player> p){
		if (phys->getVVelocity() < 0) {
			shootAnim->thisFrame();
		}
		if (phys->getVVelocity() < -200 && !shot) {
			shoot(bList, p);
			shot = true;
		}

		phys->eachFrame(deltaT);

		if (faceRight) {
			phys->move(0, deltaT, jumpSpeed);
		}
		else {
			phys->move(180, deltaT, jumpSpeed);
		}
		
	}

	void hitRight(){
		//faceRight = true;
		setFacing(true);
	}
	void hitLeft() {

		//faceRight = false;
		setFacing(false);
	}

	void setFacing(bool right) {
		if (right) {
			if (!faceRight) {
				faceRight = true;
				walkAnim->swapAll();
				walkAnim->setOffsetList(list<Vector2f>{Vector2f(-1 * 4, 0 * 4), Vector2f(5 * 4, -1 * 4), Vector2f(1 * 4, 0 * 4), Vector2f(5 * 4, -1 * 4)});
				if (state == walk) {
					walkAnim->thisFrame();
				}
				jumpAnim->swapAll();
				shootAnim->swapAll();
			}
		}
		else {
			if (faceRight) {
				faceRight = false;
				walkAnim->swapAll();
				walkAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(8 * 4, -1 * 4), Vector2f(6 * 4, 0 * 4), Vector2f(8 * 4, -1 * 4)});
				if (state == walk) {
					walkAnim->thisFrame();
				}
				jumpAnim->swapAll();
				shootAnim->swapAll();
			}
		}
	}

	

	void shoot(list<shared_ptr<EnemyBullet>>* bList, shared_ptr<player> p) {
		shootAnim->nextFrame(false);
		shootAnim->thisFrame();
		shootAnim->reset();
		

		bList->push_back(shared_ptr<EnemyBullet>(new BossCrashBomb(phys->getMiddlePos(), Maths::getAngle(phys->getMiddlePos(), p->getMiddlePos()), fireSpeed, sprite->getTexture(), bulletLandSound, bulletExplodeSound)));

	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		if (state == walk) {
			shot = false;
			walkLoop(p, deltaT);

		}
		else if (state == jumping) {

			jumpLoop(bList, deltaT, tileList, p);
			if (grounded) {
				state = walk;
				walkAnim->thisFrame();
			}
		}

		tileCollision(tileList);
		
		hit->updatePos();

	}

	int airDam() {
		return 8;
	}

	int genericDam() {
		return 100;
	}
};