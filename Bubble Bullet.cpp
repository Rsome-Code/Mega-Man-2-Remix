#include "bullet.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#include "Maths.cpp"

#include <SFML\audio.hpp>
#pragma once

class BubbleBullet : public bullet {

	animation* anim;
	animTimer* timer;

	bool grounded = false;

	float forward;

	float angleSpeed = 400;

public:

	BubbleBullet(objectSprite* o, Texture* t) {
		
		shootTime = 2;
		shootTemp = shootTime;
		origin = o;

		sprite = new movable("buster", t, Vector2i(220, 233), Vector2i(16, 16), Vector2f(0, 0), Vector2f(4, 4));
		hitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 16)), sprite);

		anim = new animation(list<IntRect>{IntRect(220, 230, 16, 16), IntRect(238, 231, 14, 14), IntRect(256, 232, 12, 12)}, sprite);
		timer = new animTimer(anim, 8, true);

		speed = 700;

		dinkSetup();
		grounded = false;
	}

	void start(bool r) {
		deflected = false;
		
		right = r;
		if (right) {
			forward = 0;
			direction = 300;
		}
		else {
			forward = 180;
			direction = 240;
		}

		shooting = true;

		float offset;
		if (r) {
			offset = 80;
		}
		else {
			offset = -24;
		}
		sprite->setPosition(Vector2f(origin->getPosition().x + offset, origin->getPosition().y + 38));


		shootTemp = shootTime;
	}

	bool eachFrame(float* deltaT) {
		if (shooting) {
			if (!grounded) {
				if (right) {
					if (direction != 90) {
						direction = direction + (angleSpeed * *deltaT);
						if (direction >= 450) {
							direction = 450;
						}
					}
				}
				else {
					direction = direction - (angleSpeed * *deltaT);
					if (direction <= 90) {
						direction = 90;
					}
				}
			}

			shootTemp = shootTemp - *deltaT;
			if (shootTemp <= 0) {
				shootReset();
				shooting = false;
				return true;
			}

		
			timer->run(deltaT);

			sprite->move(direction, deltaT, speed);

			hitbox->updatePos();
			return false;
		}
		return false;
	}

	void tileCollision(list<tile*> tileList) {
		if (!deflected) {
			if (grounded) {
				grounded = false;
				direction = 90;
			}
			for (tile* t : tileList) {
				if (t->getGround() != NULL) {
					if (hitboxDetect::hitboxDetection(hitbox, t->getGround())) {
						grounded = true;

						direction = forward;
					}
				}
			}
		}
	}
	int checkDamage(object* en) {
		return en->bubbleDam();
	}

};