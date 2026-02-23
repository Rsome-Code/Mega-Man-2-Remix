#include "enemy bullet.cpp"
#include "physics object.cpp"
#include "animation timer.cpp"
#pragma once

class FireBall : public EnemyBullet {
	float upDistance;
	float upSpeed;

	float timeToUp = 0.5;
	float timeToDown = timeToUp * 2;

	float angle;
	float speed;

	shared_ptr<physicsObject> phys;
	shared_ptr<animation> burnAnim;
	shared_ptr<animTimer> burnTimer;

	enum State {
		flying, burning
	};

	State state = flying;

public:
	FireBall(float startAngle, float speed, shared_ptr<Texture> t, Vector2f pos, float upDist) {
		phys = shared_ptr<physicsObject> (new physicsObject(t, IntRect(248, 125, 8, 8), pos, Vector2f(4, 4)));
		mov = phys;
		sprite = mov;
		angle = startAngle;
		this->speed = speed;
		upDistance = upDist;
		

		burnAnim = shared_ptr<animation>(new animation(list<IntRect> {IntRect(248, 125, 8, 8), IntRect(258, 125, 8, 8), IntRect(266, 125, 8, 8)}, phys));
		burnTimer = shared_ptr<animTimer> (new animTimer(burnAnim, 6, true));
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 8, 8), mov));

		upCalc();

		throwCalc();

		phys->enableGravity(true);

	}

	void throwCalc() {

		float vSpeed = Maths::map(0, 90, 0, 2, angle);

		vSpeed = speed * vSpeed;
		if (angle > 45) {
			//vSpeed += 200;
		}

		phys->setVVelocity(fabs(vSpeed));
		phys->setHVelocity(speed);
		phys->setFriction(speed);
	}

	void upCalc() {
		upSpeed = upDistance / timeToUp;
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {

		burnTimer->run(deltaT);
		hit->updatePos();

		if (state == flying) {
			phys->eachFrame(deltaT);

			tileCheck(tileList);
		}

		else {
			riseLoop(deltaT);
		}

	}

	void riseLoop(float* deltaT) {
		timeToUp -= *deltaT;
		timeToDown -= *deltaT;

		if (timeToUp > 0) {
			phys->move(270, deltaT, upSpeed);
		}
		else if (timeToDown > 0) {
			phys->move(90, deltaT, upSpeed);
		}
		else {
			phys->setPosition(Vector2f(0, 0));
		}

	}

	void tileCheck(list<shared_ptr<tile>>* tileList) {
		for (shared_ptr<tile> t : *tileList) {
			if (t->getGround() != NULL) {
				if (hitboxDetect::hitboxDetection(hit, t->getGround())) {
					state = burning;
					phys->setPosition(Vector2f(phys->getPosition().x, t->getGround()->getPosition().y - sprite->getSize().y));
					return;
				}
			}
		}
	}
	
};