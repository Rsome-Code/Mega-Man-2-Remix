#include "bullet.cpp"
#pragma once

class LeafBullet : public bullet {

	float angleSpeed = 380;
	int startAngleSpeed = 400;
	float speed = 500;
	float angle;
	float moveAngle = 0;

	bool shot = false;
	bool right = false;

	float shotSpeed = 600;

	Vector2f initialPos;

	Vector2f relativeCPoint;

public:
	LeafBullet(shared_ptr<objectSprite> o, shared_ptr<Texture> t, Vector2f relCPoint) {

		sprite = shared_ptr<movable>(new movable(string("eBullet"), t, IntRect(30, 141, 8, 8), o->getMiddlePos(), Vector2f(4, 4)));
		Vector2f position = Vector2f(o->getMiddlePos().x - (sprite->getSize().x / 2), o->getMiddlePos().y - (sprite->getSize().y / 2));
		sprite->setPosition(position);
		origin = o;
		hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 8), sprite));

		angle = 0;
		initialPos = o->getPosition();

		relativeCPoint = relCPoint;
	}

	void setCorner(int i) {
		if (i == 2) {
			angle = 225;
		}
		else if (i == 0) {
			angle = 315;
		}
		else if (i == 3) {
			angle = 135;
		}
		else if (i == 1) {
			angle = 45;
		}
		//angle = 0;
	}

	bool moveOut = true;
	

	enum State {
		shield, moving
	};
	State state = shield;

	void start(bool r) {
		right = r;
		initialPos = origin->getPosition();
		shooting = true;

		

		Vector2f position = Vector2f(origin->getMiddlePos().x - (sprite->getSize().x / 2), origin->getMiddlePos().y - (sprite->getSize().y / 2));
		sprite->setPosition(position);
		firstFrame = true;
		state = shield;
		moveOut = true;
		deflected = false;
	}

	void secondFire(int angle) {
	
		if (state != moving) {
			state = moving;
			this->moveAngle = angle;
		}
	}


	void deflect() {
		angle = 90;

		dink->play();
		deflected = true;
	}

	float circleDist = 80;
	void checkOut() {
	//	if (!hitboxDetect::hitboxDetection(hitbox, origin)) {
		Vector2f cPoint = relativeCPoint + origin->getPosition();
		if (Maths::getDistance(sprite->getMiddlePos(), cPoint) >= circleDist) {
			moveOut = false;
			angle += 90;
		}
		//}
	}

	int checkDamage(shared_ptr<object> en) {
		return en->leafDam();
	};

	void circleLoop(float* deltaT) {
		if (moveOut) {
			//angle = angle + (startAngleSpeed * *deltaT);
			sprite->move(angle, deltaT, speed);

			if (!firstFrame) {
				checkOut();
			}

		}
		else {
			angle = angle + (angleSpeed * *deltaT);
			sprite->move(angle, deltaT, speed);
		}


		

		
	}

	void shieldLoop(float* deltaT) {
		Vector2f posDiff = origin->getPosition() - initialPos;
		
		sprite->setPosition(sprite->getPosition() + posDiff);

		initialPos = origin->getPosition();
	}


	bool firstFrame = true;
	bool eachFrame(float* deltaT){

		if (shooting) {
			if (!deflected) {
				circleLoop(deltaT);

				if (state == shield) {
					shieldLoop(deltaT);
				}

				else if (state == moving) {
					sprite->move(moveAngle, deltaT, shotSpeed);
				}
			}
			else {
				sprite->move(angle, deltaT, speed);
			}

			hitbox->updatePos();


			if (!firstFrame) {
				if (0 - sprite->getSize().x > sprite->getCameraPosition().x || sprite->getCameraPosition().x > 1920) {
					
					shootReset();
					return true;
				}
				if (0 - sprite->getSize().y > sprite->getCameraPosition().y || sprite->getCameraPosition().y > 1080) {
					
					shootReset();
					return true;

				}
			}
			else {
				firstFrame = false;
			}
			return false;
		}

		return false;
	}



	void setRight(bool b) {
		right = b;
	}
};