#include "bullet.cpp"
#include "animation timer.cpp"
#include "Maths.cpp"
#pragma once

class BoomerangBullet : public bullet {
	movable* mov;
	float flyTime = 0.2;
	float flyTime_left = flyTime;

	float turnTime = 0.2;
	float turnTime_left = turnTime;
	
	float slowTurnSpeed = 3;
	float fastTurnSpeed = 30;
	animation* anim;
	animTimer* timer;

	int angle;

public:
	BoomerangBullet(Texture* tex, objectSprite* pl, SoundCollection* soundCol){
		origin = pl;
		mov = new movable(tex, IntRect(219, 284, 8, 6), Vector2f(0,0), Vector2f(4, 4));
		sprite = mov;
		hitbox = new objectHitbox(IntRect(0, 0, 8, 8), sprite);

		anim = new animation(list<IntRect>{IntRect(219, 284, 8, 6), IntRect(229, 283, 6, 8)}, sprite);
		timer = new animTimer(anim, 16, true);
		

		shootTime = 3;
		speed = 1200;
		dinkSetup(soundCol);
	}

	void start(bool r) {
		deflected = false;

		right = r;
		if (right) {
			angle = -30;
		}
		else {
			angle = 180 + 30;
		}

		shooting = true;

		float offset;
		if (r) {
			offset = 80;
		}
		else {
			offset = -36;
		}
		sprite->setPosition(Vector2f(origin->getPosition().x + offset, origin->getPosition().y + 38));


		shootTemp = shootTime;
		state = forward;
		flyTime_left = flyTime;
		turnTime_left = turnTime;
	}

	enum State {
		forward, turn, backward
	};

	State state;

	bool eachFrame(float* deltaT) {
		hitbox->updatePos();
		if (shooting) {
			timer->run(deltaT);
			if (state == forward) {
				if (right) {
					angle = angle + slowTurnSpeed;
				}
				else {
					angle = angle - slowTurnSpeed;
				}

				flyTime_left -= *deltaT;
				if (flyTime_left <= 0) {
					flyTime_left = flyTime;
					state = turn;
				}


				
			}
			else if (state == turn) {
				if (right) {
					angle = angle + fastTurnSpeed;
					if (angle >= 135) {
						state = backward;
					}
				}
				else {
					angle = angle - fastTurnSpeed;
					if (angle <= 45){
						state = backward;
					}
				}

				
			}
			else if (state == backward) {
				if (right) {
					angle = angle + slowTurnSpeed;
				}
				else {
					angle = angle - slowTurnSpeed;
				}

				flyTime_left -= *deltaT;
				if (flyTime_left <= 0) {
					sprite->setPosition(Vector2f(-9999, -9999));
					shooting = false;
				}
			}
			mov->move(angle, deltaT, speed);
			return true;
		}
		
		return false;
	}

	int checkDamage(object* en) {
		return en->quickDam();
	}
};