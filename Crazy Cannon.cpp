#include "enemy.cpp"
#include "cannon bullet.cpp"
#pragma once

class CrazyCannon : public enemy {
protected:
	using enemy::enemy;

	shared_ptr<animation> downAnim;
	shared_ptr<animation> upAnim;

	shared_ptr<animTimer> timer;

	bool up = false;

	int shootAmount = 3;
	int shootLeft = shootAmount;
	float shootTime = 1;
	float longTimer = 2;

	float shootTime_left = longTimer;

	shared_ptr<Sound> shootSound;

	

public:

	void deleteInt() {



		
	}

	virtual void initial() {
		sprite->setPosition(initialPos);

		sprite->setRect(IntRect(374, 581, 31, 22));

		downAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(374, 581, 31, 22), IntRect(407, 580, 31, 23), IntRect(440, 581, 31, 22), IntRect(473, 580, 31, 23), IntRect(508, 577, 29, 26), IntRect(541, 577, 29, 26)}, sprite));
		
		upAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(541, 577, 29, 26), IntRect(508, 577, 29, 26), IntRect(473, 580, 31, 23), IntRect(440, 581, 31, 22), IntRect(407, 580, 31, 23), IntRect(374, 581, 31, 22)}, sprite));

		downAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(2 * 4, -4 * 4), Vector2f(2 * 4, -4 * 4)});
		upAnim->setOffsetList(list<Vector2f>{Vector2f(2 * 4, -4 * 4), Vector2f(2 * 4, -4 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0, 0)});

		if (faceRight) {
			upAnim->swapAll();
			downAnim->swapAll();

			downAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, -4 * 4), Vector2f(0 * 4, -4 * 4)});
			upAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, -4 * 4), Vector2f(0 * 4, -4 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0, 0)});
		}

		if (up) {
			timer = shared_ptr<animTimer> (new animTimer(downAnim, 8, false));

			while (downAnim->getCurrentIndex() != downAnim->getSize()) {
				downAnim->nextFrame(false);
				downAnim->thisFrame();
			}
			
		}
		else {
			timer = shared_ptr<animTimer> (new animTimer(upAnim, 8, false));
			while (upAnim->getCurrentIndex() != upAnim->getSize()) {
				
				upAnim->nextFrame(false);
				upAnim->thisFrame();
			}
		}

		if (faceRight) {
			code = "crazy cannon-y";
		}
		else {
			code = "crazy cannon-n";
		}
		hp = 5;
		damage = 3;

		offSetList();

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 31, 22), sprite));
		hurt = hit;
	}


	void loadSound(shared_ptr<SoundCollection> soundCol) {

		sound = soundCol->getLand();

		shootSound = soundCol->getShoot();
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {

		shootTime_left -= *deltaT;

		timer->run(deltaT);

		if (shootTime_left <= 0) {
			shootTime_left = shootTime;
			shoot(bList);
			shootLeft -= 1;
			
			if (shootLeft <= 0) {
				shootLeft = shootAmount;
				
				shootTime_left = longTimer;

				if (up) {
					timer->setAnim(upAnim);
					timer->reset();
					upAnim->reset();
				}
				else {
					timer->setAnim(downAnim);
					timer->reset();
					downAnim->reset();
				}
				up = !up;
			}
		}

	}

	void shoot(list<shared_ptr<EnemyBullet>>* bList) {

		shootSound->play();
		Vector2f startPos;
		if (faceRight) {
			startPos = Vector2f(sprite->getEndPosition().x, sprite->getMiddlePos().y - (4 * 4));
		}
		else {
			startPos = Vector2f(sprite->getPosition().x , sprite->getMiddlePos().y - (4 *4));
		}
		shared_ptr<CannonBullet> temp = shared_ptr<CannonBullet>(new CannonBullet(sprite->getTexture(), startPos, faceRight));

		
		temp->setUp(up);
		

		bList->push_back(temp);
	}

public:

	void setRight(bool r) {
		faceRight = r;
		if (r) {
			code = "crazy cannon-y";
		}
		else {
			code = "crazy cannon-n";
		}


	}
};