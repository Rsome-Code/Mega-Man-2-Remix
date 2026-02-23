#include "bullet.cpp"
#include "animation timer.cpp"

#pragma once

class MetalBladeBullet : public bullet {

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;



	//int angle;

public:

	MetalBladeBullet(shared_ptr<objectSprite> o, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {
		origin = o;

		sprite = shared_ptr<movable>(new movable(t, IntRect(521, 58, 16, 16), Vector2f(0, 0), Vector2f(4, 4)));
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(521, 58, 16, 16), IntRect(538, 58, 16, 16)}, sprite));
		timer = shared_ptr<animTimer> (new animTimer(anim, 8, true));
		dinkSetup(soundCol);

		hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 16, 16), sprite));

		shootTime = 1;
		shootTemp = shootTime;

		speed = 800;
		direction = 0;
		
	}

	void start(int angle, bool r) {
		deflected = false;

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
		this->direction = angle;
	}

	bool eachFrame(float* deltaT) {
		hitbox->updatePos();
		if (shooting) {

			sprite->move(direction, deltaT, speed);

			shootTemp = shootTemp - *deltaT;
			if (shootTemp <= 0) {
				shootReset();
				shooting = false;
				return true;
			}


			timer->run(deltaT);
		}

		return false;
	}

	int checkDamage(shared_ptr<object> en) {
		return en->metalDam();
	}

	void deflect() {
		if (!deflected) {
			if (90 < direction <= 270) {
				direction = direction + 135;
				deflected = true;
				dink->play();
			}
			else {
				direction = direction - 135;
				deflected = true;
				dink->play();
			}
		}
	}
	
};