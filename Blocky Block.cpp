#include "physics bullet.cpp"
#include "animation timer.cpp"

#pragma once

class BlockyBlock : public PhysicsBullet {


	int jumpForce = 500;
	int bounceForce = 50;
	int angle = 180;
	int moveSpeed;

	animation* deathAnim;
	animTimer* deathTimer;

public:
	BlockyBlock(Texture* t, Vector2f pos, int speed, bool right) {
		phys = new physicsObject(t, IntRect(567, 76, 16, 16), pos, Vector2f(4, 4));
		sprite = phys;
		hit = new objectHitbox(IntRect(0, 0, 16, 16), phys);

		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite);
		deathTimer = new animTimer(deathAnim, 8, false);
		list<Vector2f> temp = list<Vector2f>{ Vector2f(0, 0), Vector2f(4 * 4, 4 * 4), Vector2f(6 * 4, 6 * 4), Vector2f(7 * 4, 7 * 4), Vector2f(10 * 4, 10 * 4) };
		deathAnim->setOffsetList(temp);

		moveSpeed = speed;

		if (right) {
			angle = 0;
		}
		else {
			angle = 180;
		}
		phys->setVVelocity(jumpForce);
		phys->enableGravity(true);
	}

	enum State {
		launch, bounce, die
	};
	State state = launch;

	void eachFrame(float* deltaT, list<tile*>* tileList) {

		tileCollision(tileList);
		
		

		if (state == launch) {
			phys->eachFrame(deltaT);
			hit->updatePos();
			phys->move(angle, deltaT, moveSpeed);
			if (grounded) {
				state = bounce;
				phys->setVVelocity(bounceForce);
				grounded = false;
			}
		}
		else if (state == bounce){
			phys->eachFrame(deltaT);
			hit->updatePos();
			if (grounded) {
				state = die;
				hit = new objectHitbox(IntRect(-99999, -999999, 0, 0), sprite);
			}
		}

		else {
			deathTimer->run(deltaT);
			if (deathTimer->isFinished()) {
				phys->setPosition(Vector2f(-9999, -9999));
			}
		}


	}


};