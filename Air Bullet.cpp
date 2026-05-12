#include "bullet.cpp"
#include "animation timer.cpp"
#pragma once

class AirBullet : public bullet {

	shared_ptr<physicsObject> phys;
	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	shared_ptr<animation> deflectAnim;
	shared_ptr<animTimer> deflectTimer;

	int direction = 0;

	bool deflecting = false;

public:

	AirBullet(shared_ptr<Texture> tex, shared_ptr<objectSprite> pl, shared_ptr<SoundCollection> soundCol, int speed) {
		origin = pl;

		phys = shared_ptr<physicsObject>(new physicsObject(tex, IntRect(222, 82, 12, 15), origin->getMiddlePos(), Vector2f(4, 4)));
		sprite = phys;

		hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 12, 15), phys));

		phys->setGravity(1000);
		phys->enableGravity(true);

		shootTime = 0.5;


		this->speed = speed;
		dinkSetup(soundCol);

		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(222, 82, 12, 15), IntRect(239, 81, 14, 16), IntRect(258, 81, 12, 16)}, phys));
		timer = shared_ptr<animTimer>(new animTimer(anim, 8, true));

		deflectAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(273, 82, 12, 14), IntRect(289, 82, 12, 14), IntRect(305, 83, 14, 14), IntRect(322, 81, 16, 16)}, phys));
		deflectTimer = shared_ptr<animTimer>(new animTimer(deflectAnim, 8, false));
		deflectAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(-1 * 4, 1 * 4), Vector2f(-3 * 4, 1 * 4)});
	}

	bool eachFrame(float* deltaT) {

		hitbox->updatePos();

		if (shooting) {


			if (phys->getCameraPosition().y <0) {
				shootReset();
				
			}


			if (deflecting) {
				deflectTimer->run(deltaT);
				if (deflectTimer->isFinished()) {
					deflectTimer->reset();
					deflectAnim->reset();
					shootReset();
				}
			}
			else {
				flyLoop(deltaT);
				timer->run(deltaT);

			}

			return true;
		}
		return false;

	}

	void flyLoop(float* deltaT) {

		phys->move(direction, deltaT, speed);

		phys->eachFrame(deltaT);

	}

	void uniqueReset() {
		phys->setVVelocity(0);
		phys->setCameraPosition(Vector2f(0, -1000));
		deflecting = false;
	}

	void start(bool r) {
		deflected = false;

		

		if (!r) {
			if (right) {
				anim->swapAll();
			}
			direction = 180;
		}
		else {
			if (!right) {
				anim->swapAll();
			}
			direction = 0;
		}

		phys->setPosition(origin->getMiddlePos());
		phys->setCameraPosition(origin->getCameraPosition());

		shooting = true;
	}

	void deflect() {

		deflecting = true;

		dink->play();

	}

	int checkDamage(shared_ptr<object> en) {
		return en->airDam();
	}
};