#include "bullet.cpp"
#include "animation timer.cpp"
#include "hitbox detector.cpp"
#include "Crash Explosion.cpp"
#pragma once

class CrashBullet : public bullet {

public:
	enum State {
		FLYING, EXPLODING, ATTACHED
	};

	State state = FLYING;

	shared_ptr<Sound> attachSound;


	shared_ptr<animation> explodeAnim;
	shared_ptr<animTimer> explodeTimer;

	shared_ptr<animation> attachAnim;
	shared_ptr<animTimer> attachTimer;

	shared_ptr<animation> flashAnim;
	shared_ptr<animTimer> flashTimer;

	float explodeTime = 1;
	float explodeTime_left = explodeTime;

	float activeTime = 0.5;
	float activeTime_left = activeTime;

	unique_ptr<CrashExplosion> explosion;

	shared_ptr<Sound> explodeSound;

	shared_ptr<objectHitbox> collHit;
	shared_ptr<objectHitbox> exHit;

public:
	CrashBullet(shared_ptr<objectSprite> o, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {
		origin = o;
		sprite = shared_ptr<movable>(new movable("crash", t, IntRect(494, 243, 16, 11), Vector2f(0, 0), Vector2f(4, 4)));
		collHit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 16)), sprite));

		attachAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(494, 243, 16, 11), IntRect(515, 241, 12, 15), IntRect(531, 242, 13, 13)}, sprite));
		attachTimer = shared_ptr<animTimer>(new animTimer(attachAnim, 8, false));
		flashAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(531, 242, 13, 13), IntRect(548, 242, 13, 13)}, sprite));
		flashTimer = shared_ptr<animTimer>(new animTimer(flashAnim, 8, true));

		attachSound = soundCol->getCrash();

		speed = 1000;

		dinkSetup(soundCol);


		explodeSound = soundCol->getHit();

		hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(-99990, -999990), Vector2i(0, 0)), sprite));
		exHit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(-16*4, -16*4), Vector2i(32, 32)), sprite));
		attachAnim->swapAll();
		flashAnim->swapAll();
	}



	void start(bool r) {

		shooting = true;

		collHit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 16)), sprite));

		deflected = false;

		if (r != right) {
			attachAnim->swapAll();
			flashAnim->swapAll();
		}

		right = r;
		if (right) {
			direction = 0;

		}
		else {
			direction = 180;

		}
		shooting = true;


		sprite->setPosition(Vector2f(origin->getMiddlePos().x, origin->getMiddlePos().y - (5*4)));

		attachAnim->reset();
		attachAnim->thisFrame();
		
		state = FLYING;

	}

	void tileCollision(list<shared_ptr<tile>> tileList) {
		
		if (direction == 0) {
			leftWall(tileList);
		}
		else {
			rightWall(tileList);
		}

	}

	void leftCol(shared_ptr<object> en) {
		if (hitboxDetect::hitboxDetection(en->getLeft(), collHit)) {
			
				if (state == FLYING) {
					state = ATTACHED;
					attachSound->play();
					sprite->setPosition(Vector2f(en->getPosition().x - sprite->getSize().x, sprite->getPosition().y));
					hitbox->updatePos();
				}
			
		}
	}

	void rightCol(shared_ptr<object> en) {
		if (hitboxDetect::hitboxDetection(en->getRight(), collHit)) {

			if (state == FLYING) {
				state = ATTACHED;
				attachSound->play();
				sprite->setPosition(Vector2f(en->getPosition().x + sprite->getSize().x, sprite->getPosition().y));
				hitbox->updatePos();
			}

		}
	}

	void specialColl(shared_ptr<object> en) {
		if (en->getCode() == "break wall") {
			breakWallCollision(en);
		}
	}

	void breakWallCollision(shared_ptr<object> en) {
		
			if (direction == 0) {
				leftCol(en);
			}
			else {
				rightCol(en);
			}
		
	}

	void leftWall(list<shared_ptr<tile>> tileList) {
		for (shared_ptr<tile> t : tileList) {
			if (t->getLeft() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getLeft(), collHit)) {
					if (state == FLYING) {
						state = ATTACHED;
						attachSound->play();
						sprite->setPosition(Vector2f(t->getSprite()->getPosition().x - sprite->getSize().x, sprite->getPosition().y));
						hitbox->updatePos();
					}
					
					break;
				}
			}
		}
	}

	void rightWall(list<shared_ptr<tile>> tileList) {
		for (shared_ptr<tile> t : tileList) {
			if (t->getRight() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getRight(), collHit)) {

					if (state == FLYING) {
						state = ATTACHED;
						attachSound->play();
						sprite->setPosition(Vector2f(t->getSprite()->getPosition().x + t->getSprite()->getSize().x, sprite->getPosition().y));
						hitbox->updatePos();
					}
					
					break;
				}
			}
		}
	}

	bool attachLoop(float* deltaT) {

		flashTimer->run(deltaT);
		explodeTime_left -= *deltaT;

		if (explodeTime_left <= 0) {
			explodeTime_left = explodeTime;
			return true;
		}
		return false;
	
	}

	void startExplosion() {



		explosion = unique_ptr<CrashExplosion>(new CrashExplosion(sprite->getTexture(), Vector2f(sprite->getPosition().x - (16 * 4), sprite->getPosition().y - (16 * 4)), explodeSound));

		hitbox = exHit;

		//hitbox->setRelativePosition(Vector2i(-16*4, -16*4));
		//hitbox->setRelativeSize(Vector2i(32*4, 32*4));
		hitbox->updatePos();
		state = EXPLODING;
		sprite->setPosition(Vector2f(-9999, -9999));
	}

	list<shared_ptr <objectSprite>> getExplos() {
		return explosion->getSprites();
	}

	list<shared_ptr <objectSprite>> getSprites() {
		if (explosion != NULL) {
			return explosion->getSprites();
		}
		return list<shared_ptr <objectSprite>> {NULL};
	}

	bool explodeLoop(float* deltaT) {
		
		return explosion->eachFrame(deltaT);

	}

	bool eachFrame(float* deltaT) {

		if (shooting == false) {
			return false;
		}

		if (state == FLYING) {
			sprite->move(direction, deltaT, speed);
			//hitbox->updatePos();
			collHit->updatePos();
		}

		else if (state == ATTACHED) {
			attachTimer->run(deltaT);
			//hitbox->updatePos();
			if (attachTimer->isFinished()) {
				if (attachLoop(deltaT)) {
					startExplosion();

				}
			}
			
		}

		else if (state == EXPLODING) {
			if (explodeLoop(deltaT)) {
				shooting = false;
				
				explosion.reset();

				hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(-999999990, -9999990), Vector2i(0,0)), sprite));
				hitbox->updatePos();
				
				return true;
			}
		}


		

		return false;
	}

	State checkState() {
		return state;
	}

	void secondFire() {
		if (state != EXPLODING) {
			startExplosion();
		}
	}

	int checkDamage(shared_ptr<object> en) {
		return en->crashDam();
	}

	void shootReset() {}

	void deflect() {

		if (!right) {
			direction = 330;
		}
		else {
			direction = 210;
		}

		if (state != EXPLODING) {
			dink->play();
		}

	}
};