#include "gobject.cpp"
#include "hitbox detector.cpp"
#include "angles.cpp"

#pragma once

class RailPlatform : public GameObject {

	enum Direction {
		Left, Right, Up, Down
	};

	Direction dir = Right;

	int speed = 200;

	shared_ptr<objectHitbox> ground;

	shared_ptr<movable> mov;

	Vector2f turnPoint;

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

public:

	RailPlatform(shared_ptr<Texture> tex, Vector2f pos) {
		mov = shared_ptr<movable>(new movable(tex, IntRect(1, 402, 24, 16), pos, Vector2f(4, 4)));
		sprite = mov;
		ground = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 24, 16), mov));
		updateTurnPoint();
		code = "rail platform";

		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(1, 402, 24, 16), IntRect(26, 402, 24, 16)}, sprite));
		timer = shared_ptr<animTimer>(new animTimer(anim, 8, true));
	}

	void updateTurnPoint() {
		turnPoint = Vector2f(mov->getPosition().x + 12 * 4, mov->getPosition().y + 12 * 4);
	}

	void eachFrame(float* deltaT, shared_ptr<player> p, shared_ptr<camera> cam, list<shared_ptr<GameObject>> objects) {
		timer->run(deltaT);
		checkpoint(objects);

		movePlat(deltaT);

		collision(p, deltaT);

		ground->updatePos();
		updateTurnPoint();
	}

	void checkpoint(list<shared_ptr<GameObject>> objects) {
		for (shared_ptr<GameObject> o : objects) {
			if (o->getCode() == "rail checkpoint-up") {
				if (checkColl(o)) {
					dir = Up;
				}
			}
			if (o->getCode() == "rail checkpoint-down") {
				if (checkColl(o)) {
					dir = Down;
				}
			}
			if (o->getCode() == "rail checkpoint-left") {
				if (checkColl(o)) {
					dir = Left;
				}
			}
			if (o->getCode() == "rail checkpoint-right") {
				if (checkColl(o)) {
					dir = Right;
				}
			}
		}
	}

	bool checkColl(shared_ptr<GameObject> o) {
		return hitboxDetect::hitboxDetection(turnPoint, o->getHitbox());
	}

	void movePlat(float* deltaT) {
		if (dir == Left) {
			mov->move(Angle::left, deltaT, speed);
		}
		else if (dir == Right) {
			mov->move(Angle::right, deltaT, speed);
		}
		else if (dir == Up) {
			mov->move(Angle::up, deltaT, speed);
		}
		else if (dir == Down) {
			mov->move(Angle::down, deltaT, speed);
		}
	}

	void collision(shared_ptr<player> p, float* deltaT) {
		float currentX = p->getSprite()->getPosition().x;
		
		if (!p->getTempGround()) {
			if (hitboxDetect::hitboxDetection(p->getFoot(), ground)) {
				
				if (p->getSprite()->getAcceleration().y < 0 || p->getGrounded()) {
					p->getSprite()->setPosition(Vector2f(currentX, ground->getPosition().y - (p->getHitbox()->getSize().y + 12)));

					p->setTempGround(true);
					if (!p->getGrounded()) {
						p->setGrounded(true);
						p->setGroundedOverride(true);
					}
					

					movePlayer(p, deltaT);
				}
				else {
					p->setGroundedOverride(false);
				}
			}
			else {
				p->setGroundedOverride(false);
			}
		}
	}

	void movePlayer(shared_ptr<player> p, float* deltaT) {
		if (dir == Left) {
			p->getSprite()->setPosition(Vector2f(p->getSprite()->getPosition().x - (speed * *deltaT), p->getSprite()->getPosition().y));
		}
		else if (dir == Right) {
			p->getSprite()->setPosition(Vector2f(p->getSprite()->getPosition().x + (speed * *deltaT), p->getSprite()->getPosition().y));
		}
		else if (dir == Up) {
			p->getSprite()->setPosition(Vector2f(p->getSprite()->getPosition().x, p->getSprite()->getPosition().y - (speed * *deltaT)));
		}
		else if (dir == Down) {
			p->getSprite()->setPosition(Vector2f(p->getSprite()->getPosition().x, p->getSprite()->getPosition().y + (speed * *deltaT)));
		}
	}
};