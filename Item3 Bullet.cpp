#include "item bullets.cpp"
#include "animation timer.cpp"
#include "hitbox detector.cpp"
#pragma once

class Item3Bullet : public ItemBullet {

	shared_ptr<animation> flyAnim;
	shared_ptr<animTimer> flyTimer;


	int moveSpeed = 300;

	int climbSpeed = 100;

	int bounceForce = 800;


	float flashTime = 0.1;
	float flashTime_left = 0;

	bool right = false;

	shared_ptr<objectHitbox> groundHit;
	shared_ptr<objectHitbox> topHit;

	bool playerOn = false;

public:

	Item3Bullet(shared_ptr<Texture> t, Vector2f pos, bool right) {

		//Change all this:
		phys = shared_ptr<physicsObject>(new physicsObject(t, IntRect(520, 287, 22, 12), pos, Vector2f(4, 4)));

		this->right = right;

		

		flyAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(520, 287, 22, 12), IntRect(545, 287, 22, 12)}, phys));
		hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(4, 0, 14, 2), phys));

		groundHit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(4, 0, 14, 12), phys));

		topHit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(4, -25 * 4, 14, 12 + 24), phys));

		
		flyTimer = shared_ptr<animTimer>(new animTimer(flyAnim, 8, true));


		code = "item 3";

		thisFrameMove = Vector2f(0, 0);

		exploSetup();

		phys->enableGravity(true);
	}

	enum State {
		bouncing, climbing
	};

	State state = bouncing;

	float time = 8;

	void flash(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			display = !display;
			flashTime_left = flashTime;
		}
	}

	bool eachFrame(float* deltaT, list<shared_ptr<tile>> tiles) {
		flyTimer->run(deltaT);

		if (state == bouncing) {
			if (bounceLoop(deltaT, tiles)) {
				state = climbing;
				phys->enableGravity(false);
			}
		}

		else if (state == climbing) {
			time -= *deltaT;

			if (time <= 2) {
				flash(deltaT);
			}

			if (time <= 0) {
				display = true;
				return true;
			}

			hitbox->updatePos();
			return climbLoop(deltaT, tiles);
		}
		hitbox->updatePos();
		return false;

	}

	void bounce() {
		phys->setVVelocity(bounceForce);
	}

	bool firstPlayerOn = true;

	void setPlayerOn(bool b) {
		if (b) {
			firstPlayerOn = false;
		}
		playerOn = b;
	}

	bool climbLoop(float* deltaT, list<shared_ptr<tile>> tiles) {

		if (playerOn || firstPlayerOn) {
			if (ceilingCol(tiles)) {
				thisFrameMove = phys->move(90, deltaT, climbSpeed);
				firstPlayerOn = false;
			}
			else {
				thisFrameMove = phys->move(270, deltaT, climbSpeed);
			}
		}
		else {
			thisFrameMove = phys->move(90, deltaT, climbSpeed);
		}

		
		return floorCol(tiles);
	}

	
	bool ceilingCol(list<shared_ptr<tile>> tiles) {
		topHit->updatePos();
		for (shared_ptr<tile> t : tiles) {
			if (t->getCeiling() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getCeiling(), topHit)) {
					return true;
				}
			}
		}
		return false;

	}

	bool bounceLoop(float* deltaT, list<shared_ptr<tile>> tiles) {

		phys->eachFrame(deltaT);

		if (right) {
			thisFrameMove = phys->move(0, deltaT, moveSpeed);
		}
		else {
			thisFrameMove = phys->move(180, deltaT, moveSpeed);
		}
		

		
		if (floorCol(tiles)) {
			bounce();
		}

		if (wallCol(tiles)) {
			return true;
		}
		return false;
	}

	bool floorCol(list<shared_ptr<tile>> tiles) {
		groundHit->updatePos();
		for (shared_ptr<tile> t : tiles) {
			if (t->getGround() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getGround(), groundHit)) {
					return true;
				}
			}
		}
		return false;

	}

	bool wallCol(list<shared_ptr<tile>> tiles) {
		if (right) {
			for (shared_ptr<tile> t : tiles) {
				if (t->getLeft() != NULL) {
					if (hitboxDetect::hitboxDetection(t->getLeft(), hitbox)) {
						return true;
					}
				}
			}
		}
		else {
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