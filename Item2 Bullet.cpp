#include "item bullets.cpp"
#include "animation timer.cpp"
#include "hitbox detector.cpp"
#pragma once

class Item2Bullet : public ItemBullet {

	shared_ptr<animation> flyAnim;
	shared_ptr<animTimer> flyTimer;


	float moveSpeed = 0;

	float accel = 200;
	int maxSpeed = 400;
	

	float flashTime = 0.1;
	float flashTime_left = 0;

	bool right = false;
public:

	Item2Bullet(shared_ptr<Texture> t, Vector2f pos, bool right) {

		//Change all this:
		phys = shared_ptr<physicsObject>(new physicsObject(t, IntRect(449, 287, 32, 8), pos, Vector2f(4, 4)));

		this->right = right;

		if (!right) {
			phys->setRect(IntRect(449, 296, 32, 8));
			flyAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(449, 296, 32, 8), IntRect(485, 296, 29, 8)}, phys));
			hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 23, 2), phys));
			
		}
		else {
			flyAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(449, 287, 32, 8), IntRect(485, 287, 29, 8)}, phys));
			flyAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(3 * 4, 0)});
			hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(10, 0, 23, 2), phys));
		}

		
		flyTimer = shared_ptr<animTimer>(new animTimer(flyAnim, 8, true));

		
		code = "item 2";

		thisFrameMove = Vector2f(0, 0);

		exploSetup();
		

	}

	bool eachFrame(float* deltaT, list<shared_ptr<tile>> tiles) {
		
		if (moveSpeed < maxSpeed) {
			moveSpeed += accel * *deltaT;
		}
		else {
			moveSpeed = maxSpeed;
		}
		
		if (right) {
			thisFrameMove = phys->move(0, deltaT, moveSpeed);
		}
		else {
			thisFrameMove = phys->move(180, deltaT, moveSpeed);
		}
		hitbox->updatePos();


		flyTimer->run(deltaT);


		return tileCol(tiles);

	}

	bool tileCol(list<shared_ptr<tile>> tiles) {
		if (right) {
			for (shared_ptr<tile> t : tiles) {
				if (t->getLeft() != NULL) {
					if (hitboxDetect::hitboxDetection(t->getLeft(), hitbox)) {
						return true;
					}
				}
			}
		}
		else{
			for (shared_ptr<tile> t : tiles) {
				if (t->getRight() != NULL) {
					if (hitboxDetect::hitboxDetection(t->getRight(), hitbox)) {
						return true;
					}
				}
			}
		}
		return false;
	}

	Vector2f thisFrameMove;
	Vector2f getThisframeDistance() {
		return thisFrameMove;
	}


};