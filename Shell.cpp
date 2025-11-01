#include "enemy bullet.cpp"
#include "physics object.cpp"
#pragma once

class Shell : public EnemyBullet {

	physicsObject* phys;

	int vel;

public:
	Shell(Texture* t, Vector2f position, bool right) {
		damage = 3;
		phys = new physicsObject(t, IntRect(334, 653, 21, 23), position, Vector2f(4, 4));
		sprite = phys;

		if (!right) {
			vel = 400;
		}
		else {
			vel = -400;
		}
		phys->setVVelocity(1000);
		phys->enableGravity(true);
		hit = new objectHitbox(IntRect(0, 0, 21, 23), sprite);
	}

	void eachFrame(float* deltaT, list<tile*>* tileList) {
		phys->eachFrame(deltaT);
		hit->updatePos();
		phys->move(0, deltaT, vel);
	}

};