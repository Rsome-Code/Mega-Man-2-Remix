#include "enemy bullet.cpp"
#include "crash explosion.cpp"
#pragma once

class BossCrashBomb : public EnemyBullet {

	enum State {
		moving, sticking, exploding
	};
	State state = moving;

	float timeLeft = 1;

	shared_ptr<animation> tickAnim;
	shared_ptr<animTimer> tickTimer;

	shared_ptr<CrashExplosion> explosion;


	shared_ptr<animation> flyAnim;

	shared_ptr<Sound> explodeSound;
	shared_ptr<Sound> landSound;

public:

	BossCrashBomb(Vector2f pos, float angle, float speed, shared_ptr<Texture> t, shared_ptr<Sound>landS, shared_ptr<Sound>exploS) {

		mov = shared_ptr<movable>(new movable(t, IntRect(42, 125, 13, 13), pos, Vector2f(4, 4)));
		flyAnim = shared_ptr<animation>(new animation(IntRect(42, 125, 13, 13), mov));


		sprite = mov;

		tickAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(82, 131, 10, 10), IntRect(99, 131, 10, 10)}, mov));
		tickTimer = shared_ptr<animTimer>(new animTimer(tickAnim, 8, true));

		if (angle > 90) {
			flyAnim->swapAll();
			flyAnim->thisFrame();
			tickAnim->swapAll();
			tickAnim->setOffsetList(list<Vector2f>{Vector2f(-4 * 4, 4 * 4), Vector2f(-4 * 4, 4 * 4)});
		}
		else {
			tickAnim->setOffsetList(list<Vector2f>{Vector2f(4 * 4, 4 * 4), Vector2f(4 * 4, 4 * 4)});
		}

		this->angle = angle;
		this->speed = speed;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 13, 13), mov));
		damage = 4;

		landSound = landS;
		explodeSound = exploS;

		
	}

	bool tileColl(list<shared_ptr<tile>>* tileList) {
		for (shared_ptr<tile> t : *tileList) {
			if (t->getGround() != NULL) {
				if (hitboxDetect::hitboxDetection(hit, t->getGround())) {
					return true;
				}
			}
			if (t->getLeft() != NULL) {
				if (hitboxDetect::hitboxDetection(hit, t->getLeft())) {
					return true;
				}
			}
			if (t->getRight() != NULL) {
				if (hitboxDetect::hitboxDetection(hit, t->getRight())) {
					return true;
				}
			}
		}
		return false;
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {

		if (state == moving) {
			mov->move(angle, deltaT, speed);

			if (tileColl(tileList)) {
				state = sticking;
				landSound->play();
				
			}
		}

		if (state == sticking) {
			timeLeft -= *deltaT;
			tickTimer->run(deltaT);

			if (timeLeft <= 0) {
				explode();
				state = exploding;
			}
		}

		if (state == exploding) {
			if (explosion->eachFrame(deltaT)) {
				sprite->setPosition(Vector2f(0, 0));
			}
		}

		hit->updatePos();
	}

	void explode() {
		explosion = shared_ptr<CrashExplosion>(new CrashExplosion(sprite->getTexture(), mov->getPosition() - mov->getSize(), explodeSound, IntRect(110, 125, 16, 16), IntRect(129, 127, 12, 12), IntRect(147, 128, 10, 10), IntRect(163, 131, 4, 4)));
		hit->setRelativeRect(IntRect(-16, -16, 32,32));
		setDisplay(false);
		sprite->setRect(IntRect(0,0,0, 0));
		damage = 6;
	}

	list<shared_ptr<objectSprite>> getSprites() {
		if (state == exploding) {
			return explosion->getSprites();
		}
		return list<shared_ptr<objectSprite>>{NULL};
	}


};