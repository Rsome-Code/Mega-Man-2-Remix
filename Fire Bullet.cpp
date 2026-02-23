#include "enemy bullet.cpp"
#include "physics object.cpp"
#include "player.cpp"
#pragma once

class FireBullet : public EnemyBullet {

	float distance;
	shared_ptr<physicsObject> phys;

public:


	

	FireBullet(shared_ptr<Texture> t, Vector2f position, float target){
		distance = position.x - target;
		speed = 600;
		angle = 180;
		phys = shared_ptr<physicsObject> (new physicsObject(t, IntRect(398, 115, 16, 16), position, Vector2f(4,4)));
		mov = phys;
		sprite = mov;
		
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), sprite));
		phys->enableGravity(true);
		phys->setVVelocity(700);

		damage = 3;
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		phys->eachFrame(deltaT);
		mov->move(angle, deltaT, speed);
		hit->updatePos();

		if (distance <= 0) {
			speed = 0;
		}
	}

	void updateDistance(shared_ptr<player> p) {
		distance = sprite->getPosition().x - p->getPosition().x;
	}
};