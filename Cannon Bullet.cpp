#include "enemy bullet.cpp"
#include "angles.cpp"
#pragma once

class CannonBullet : public EnemyBullet {


	shared_ptr<physicsObject> phys;

	int angle;

	int speed;

	int upForce = 700;

	int forwardForce = 400;

public:



	CannonBullet(shared_ptr<Texture> t, Vector2f pos, bool right) {
		phys = shared_ptr<physicsObject> (new physicsObject(t, IntRect(400, 604, 8, 8), pos, Vector2f(4, 4)));
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

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 8), phys));
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

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		hit->updatePos();
		phys->eachFrame(deltaT);

		phys->move(angle, deltaT, speed);

		
	}
};