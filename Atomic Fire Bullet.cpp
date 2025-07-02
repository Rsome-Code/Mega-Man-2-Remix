#include "bullet.cpp"
#include "animate.cpp"
#include "animation timer.cpp"
#include "Maths.cpp"
#pragma once

class AtomicFireB : public bullet {
public:

	animation* anim;
	animTimer* aTimer;

	float holdTime = 0;
	float maxHold = 2;

	float minPower = 1;
	float maxPower = 5;

	float power = 1;

	float minSize = 1;
	float maxSize = 4;
	float size = 2;

	float yPosAdd = 0;
	float maxYPos = 32;
	

	AtomicFireB(objectSprite* o, Texture* t) {
		shootTime = 1;
		shootTemp = shootTime;

		origin = o;
		sprite = new movable("buster", t, Vector2i(347, 42), Vector2i(24, 24), Vector2f(0, 0), Vector2f(2, 2), 1);

		hitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(24, 24)), true, sprite);

		anim = new animation(list<IntRect>{IntRect(347, 42, 24, 24), IntRect(374, 44, 20, 20)}, sprite);
		aTimer = new animTimer(anim, 10, true);
		list<Vector2f> testOffset = list<Vector2f>{ Vector2f(0, 0), Vector2f(8, 8) };
		anim->setOffsetList(testOffset);
	}

	bool eachFrame(float* deltaT) {
		
		shootTemp = shootTemp - *deltaT;
		if (shootTemp <= 0) {
			shootReset();
			return true;
		}

		if (shooting) {
			aTimer->run(deltaT);
			if (right) {
				sprite->move(0, deltaT, speed);
			}
			else { 
				sprite->move(180, deltaT, speed);
			}
			hitbox->updatePos();
			return false;
		}
		return false;
	}

	void start(bool r) {
		
	}

	int checkDamage(object* en) {
		return (en->atomicDam() * power);
	}

	void hold(float* deltaT) {
		holdTime = holdTime + *deltaT;
		
		if (holdTime <= maxHold) {
			power = Maths::map(float(0), maxHold, minPower, maxPower, holdTime);
			size = Maths::map(float(0), maxHold, minSize, maxSize, holdTime);
			yPosAdd = Maths::map(float(0), maxHold, 0, maxYPos, holdTime);
		}
		else {
			power = maxPower;
			size = maxSize;
			yPosAdd = maxYPos;
		}
	}

	bool release(bool r) {
		holdTime = 0;
		if (!shooting) {
			sprite->setScale(Vector2f(size, size));


			shooting = true;
			float offset;
			if (r) {
				offset = 80;
			}
			else {
				offset = -24;
			}
			sprite->setPosition(Vector2f(origin->getPosition().x + offset, (origin->getPosition().y + 38) - yPosAdd));
			right = r;
			shootTemp = shootTime;
			return true;
		}
		return false;
	}

	void resetHoldTime() {
		holdTime = 0;
	}

	void onHit() {
		shootReset();
	}

	float getHoldTime() {
		return holdTime;
	}
	float getMaxHoldTime() {
		return maxHold;
	}

};