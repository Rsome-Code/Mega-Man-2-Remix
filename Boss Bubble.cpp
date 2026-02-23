#include "enemy bullet.cpp"
#include "physics object.cpp"
#include "animation timer.cpp"
#pragma once

class BossBubble : public EnemyBullet {
	shared_ptr<physicsObject> phys;

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	int hSpeed = 300;
	int jumpForce = 600;

	int angle;

public:
	BossBubble(shared_ptr<Texture> t, Vector2f pos, bool right) {
		if (right) {
			angle = 0;
		}
		else {
			angle = 180;
		}
		phys = shared_ptr<physicsObject> (new physicsObject(t, IntRect(1, 74, 16, 16), pos, Vector2f(4, 4)));
		sprite = phys;
		phys->setGravity(-900);
		phys->enableGravity(true);
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(1, 74, 16, 16), IntRect(19, 75, 14, 14)}, sprite));
		anim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(1 * 4, 1 * 4)});
		timer = shared_ptr<animTimer> (new animTimer(anim, 8, true));

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), sprite));

		damage = 3;
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		timer->run(deltaT);
		phys->eachFrame(deltaT);
		hit->updatePos();

		phys->move(angle, deltaT, hSpeed);

		if (groundCheck(tileList)) {
			bounce();
		}
		
	}

	void bounce() {
		phys->setVVelocity(jumpForce);
	}

	bool groundCheck(list<shared_ptr<tile>>* tileList) {
		for (shared_ptr<tile> t : *tileList) {
			if (t->getGround() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
					return true;
				}
			}
			if (t->getLeft() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getLeft(), hit)) {
					phys->setPosition(Vector2f(0, 0));
				}
			}
			if (t->getRight() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getRight(), hit)) {
					phys->setPosition(Vector2f(0, 0));
				}
			}
		}
		return false;
	}
};