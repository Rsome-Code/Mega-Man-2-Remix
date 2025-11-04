#include "enemy bullet.cpp"
#include "physics object.cpp"
#include "animation timer.cpp"
#pragma once

class BossBubble : public EnemyBullet {
	physicsObject* phys;

	animation* anim;
	animTimer* timer;

	int hSpeed = 300;
	int jumpForce = 600;

	int angle;

public:
	BossBubble(Texture* t, Vector2f pos, bool right) {
		if (right) {
			angle = 0;
		}
		else {
			angle = 180;
		}
		phys = new physicsObject(t, IntRect(1, 74, 16, 16), pos, Vector2f(4, 4));
		sprite = phys;
		phys->setGravity(-900);
		phys->enableGravity(true);
		anim = new animation(list<IntRect>{IntRect(1, 74, 16, 16), IntRect(19, 75, 14, 14)}, sprite);
		anim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(1 * 4, 1 * 4)});
		timer = new animTimer(anim, 8, true);

		hit = new objectHitbox(IntRect(0, 0, 16, 16), sprite);

		damage = 3;
	}

	void eachFrame(float* deltaT, list<tile*>* tileList) {
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

	bool groundCheck(list<tile*>* tileList) {
		for (tile* t : *tileList) {
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