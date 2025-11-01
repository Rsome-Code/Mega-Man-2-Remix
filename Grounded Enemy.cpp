#include "enemy.cpp"
#pragma once

class GroundedEnemy : public enemy {
	
	using enemy::enemy;
protected:

	int angle = 180;

	animation* moveAnim;
	animTimer* moveTimer;



	void checkDirection(list<tile*>* tileList) {
		bool onGround = false;

		objectHitbox tempH = *hit;
		objectHitbox* temp = &tempH;
		//temp->setSprite(sprite);

		temp->setSize(Vector2i(1, temp->getSize().y));
		if (faceRight) {
			temp->setRelativePosition(Vector2i(temp->getRelativePosition().x + (64), temp->getRelativePosition().y));
			temp->updatePos();
		}

		for (tile* t : *tileList) {
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