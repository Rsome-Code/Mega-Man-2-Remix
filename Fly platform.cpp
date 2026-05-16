#include "player.cpp"
#include "gobject.cpp"
#pragma once

class FlyPlatform:public GameObject {

	shared_ptr<objectHitbox> hit;

	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	shared_ptr<movable> mov;

	bool moveCircle = false;

	enum State {
		start, on, ending, end
	};

	State state = start;

public:
	FlyPlatform(shared_ptr<Texture> t, Vector2f pos) {

		mov = shared_ptr<movable>(new movable(t, IntRect(187, 335, 18, 11), pos, Vector2f(4, 4)));
		sprite = mov;

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 18, 2), sprite));

		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(187, 335, 18, 11), IntRect(206, 335, 18, 11)}, sprite));
		timer = shared_ptr<animTimer>(new animTimer(anim, 8, true));

		code = "fly platform";
	}


	void eachFrame(float* deltaT, shared_ptr<player> player, shared_ptr<camera> cam, list<shared_ptr<GameObject>> objects) {
		
		timer->run(deltaT);

		if (state == on) {
			follow(player);
			checkExit(objects, player, cam);
		}

		else if (state == start){

			if (moveCircle) {
				circle(deltaT);
			}

			checkColl(player);

			
		}

		else if (state == ending) {
			moveTowardsExit(deltaT, objects, player, cam);
		}

		else if (state == end) {
			hit->updatePos();
			platformLoop(player);
		}


	}


	
	void moveTowardsExit(float* deltaT, list<shared_ptr<GameObject>> objects, shared_ptr<player> p, shared_ptr<camera> cam) {
		
		int autoSpeed = p->getAutoSpeed();

		mov->move(Angle::right, deltaT, autoSpeed);
		mov->updateCameraPosition(cam->getPosition());

		for (shared_ptr<GameObject> o : objects) {
			if (o->getCode() == "flight exit") {
				if (o->getPosition().y > mov->getPosition().y) {
					mov->move(Angle::down, deltaT, autoSpeed);
					if (o->getPosition().y <= mov->getPosition().y) {
						mov->setPosition(Vector2f(mov->getPosition().x, o->getPosition().y));
					}
				}
				else if (o->getPosition().y < mov->getPosition().y) {
					mov->move(Angle::up, deltaT, autoSpeed);
					if (o->getPosition().y >= mov->getPosition().y) {
						mov->setPosition(Vector2f(mov->getPosition().x, o->getPosition().y));
					}
				}

				if (o->getPosition().x < mov->getPosition().x) {
					state = end;
					p->enableControls(true);
					p->setShootemControls(false);
					p->getSprite()->setHVelocity(0);
				}

				
			}
		}
		cameraCorrection(cam, deltaT, autoSpeed);

		p->setPosition(Vector2f(mov->getPosition().x - (2 * 4), mov->getPosition().y - p->getSprite()->getSize().y));
	}

	bool camCor = true;



	void cameraCorrection(shared_ptr<camera> cam, float* deltaT, int autoSpeed) {
		int camOffset = cam->getOffset().x - (10);
		if (rightHalf) {
			
				
			if (camCor) {
				if (mov->getCameraPosition().x <= -camOffset) {
					mov->setPosition(Vector2f(cam->getPosition().x - camOffset, mov->getPosition().y));
					camCor = false;
				}
				else {
					mov->move(Angle::left, deltaT, autoSpeed);
				}
			}
			else {
				mov->setPosition(Vector2f(cam->getPosition().x - camOffset, mov->getPosition().y));
			}
			
		}
		else {
			if (camCor) {


				if (mov->getCameraPosition().x >= -camOffset) {
					mov->setPosition(Vector2f(cam->getPosition().x - camOffset, mov->getPosition().y));
					mov->updateCameraPosition(cam->getPosition());
					camCor = false;
				}
				else{
					mov->move(Angle::right, deltaT, autoSpeed);
					
				}
			}
			else {
				mov->setPosition(Vector2f(cam->getPosition().x - camOffset, mov->getPosition().y));
			}
		}
	}

	bool rightHalf = false;
	void checkExit(list<shared_ptr<GameObject>> objects, shared_ptr<player> p, shared_ptr<camera> cam) {
		
		for (shared_ptr<GameObject> o : objects) {
			if (o->getCode() == "flight exit") {
				if (o->getSprite()->getCameraPosition().x < 1920) {
					state = ending;

					p->enableControls(false);

					if (mov->getCameraPosition().x > -cam->getOffset().x) {
						rightHalf = true;
					}
				}
			}
		}
		
	}

	void circle(float* deltaT) {
		angle = angle + (angleChange * *deltaT);

		mov->move(angle, deltaT, speed);
		hit->updatePos();
	}

	float angle;
	float angleChange;
	int speed;
	void setMoveInCircle(float ang, float angChange, float speed) {
		angle = ang;
		angleChange = angChange;
		this->speed = speed;
		moveCircle = true;
	}

	void platformLoop(shared_ptr<player> player) {

		if (hitboxDetect::hitboxDetection(player->getFoot(), hit)) {
			if (!player->getGrounded()) {
				player->setGrounded(true);
				player->setGroundedOverride(true);
				player->setPosition(Vector2f(player->getPosition().x, mov->getPosition().y - player->getSprite()->getSize().y));
			}

		}
		else {
			player->setGroundedOverride(false);
		}

	}

	void checkColl(shared_ptr<player> player) {

		if (hitboxDetect::hitboxDetection(player->getFoot(), hit)) {
			state = on;
			if (!player->getGrounded()) {
				player->setGrounded(true);
				player->setGroundedOverride(true);
			}
			player->setShootemControls(true);
			
		}
	}

	void follow(shared_ptr<player> player) {

		mov->setPosition(Vector2f(player->getPosition().x + (2 * 4), player->getFoot()->getPosition().y + player->getFoot()->getSize().y - (2*4)));

	}
};