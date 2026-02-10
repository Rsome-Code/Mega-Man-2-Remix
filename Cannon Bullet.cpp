#include "enemy bullet.cpp"
#include "angles.cpp"
#pragma once

class CannonBullet : public EnemyBullet {

	physicsObject* phys;

	int angle;

	int speed;

	int upForce = 700;

	int forwardForce = 400;

public:

	CannonBullet(Texture* t, Vector2f pos, bool right) {
		phys = new physicsObject(t, IntRect(400, 604, 8, 8), pos, Vector2f(4, 4));
		sprite = phys;
		mov = phys;

		if (right) {
			angle = Angle::right;
		}
		else {
			angle = Angle::left;
		}

		

		//Standard gravity is -3000
		phys->setGravity(-1500);
		phys->enableGravity(true);

		speed = 200;

		hit = new objectHitbox(IntRect(0, 0, 8, 8), phys);
	}

	void setUp(bool u) {
		if (u) {
			phys->setVVelocity(upForce);

			sprite->setPosition(Vector2f(sprite->getPosition().x, sprite->getPosition().y - (10*4)));
		}
		else {
			phys->setVVelocity(forwardForce);

			speed = 800;
		}
	}

	void eachFrame(float* deltaT, list<tile*>* tileList) {
		hit->updatePos();
		phys->eachFrame(deltaT);

		phys->move(angle, deltaT, speed);

		
	}
};