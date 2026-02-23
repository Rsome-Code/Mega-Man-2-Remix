#include "enemy.cpp"
#pragma once

class GroundedEnemy : public enemy {
	using enemy::enemy;
protected:

	int angle = 180;

	shared_ptr<animation> moveAnim;
	shared_ptr<animTimer> moveTimer;



	void checkDirection(list<shared_ptr<tile>>* tileList) {
		bool onGround = false;


		shared_ptr<objectHitbox> temp = shared_ptr<objectHitbox> (new objectHitbox(*hit));

	
		//temp->setSprite(sprite);

		temp->setSize(Vector2i(1, temp->getSize().y));
		if (faceRight) {
			temp->setRelativePosition(Vector2i(temp->getRelativePosition().x + (64), temp->getRelativePosition().y));
			temp->updatePos();
		}

		for (shared_ptr<tile> t : *tileList) {
			if (t->getGround() != NULL) {

				if (faceRight) {
					if (hitboxDetect::hitboxDetection(temp, t->getGround())) {
						onGround = true;
					}
				}
				else {
					if (hitboxDetect::hitboxDetection(temp, t->getGround())) {
						onGround = true;
					}
				}

			}
		}

		if (!onGround) {
			setFacing(!faceRight);
		}

	}


	virtual void setFacing(bool right) {

		if (right != faceRight) {
			//springAnim->swapAll();
			//springAnim->thisFrame();

			angle = angle + 180;
			angle = angle % 360;


			moveAnim->swapAll();
			moveAnim->thisFrame();

		}

		faceRight = right;
	}
};