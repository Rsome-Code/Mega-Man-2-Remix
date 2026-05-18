#include "enemy.cpp"
#include "fly platform.cpp"
#include "lightning.cpp"
#pragma once

class LightningLord : public enemy {

	using enemy::enemy;

protected:
	shared_ptr<objectSprite> platform;
	shared_ptr<animation> platAnim;
	shared_ptr<animTimer> platTimer;

	shared_ptr<animation> idleAnim;
	shared_ptr<animTimer> idleAnimTimer;

	shared_ptr<animation> throwAnim;

	float throwTimer = 2;
	//This is the delay for the first throw
	float throwTimer_left = 0.5;

	float throwReel = 0.25;
	float throwReel_left = throwReel;

	int angleChange = 80;
	float angle;
	int speed = 300;

	void initial() {

		mov->setRect(IntRect(1, 300, 39, 24));
		mov->setScale(Vector2f(4, 4));
		mov->setPosition(initialPos);

		platform = shared_ptr<objectSprite>(new objectSprite(sprite->getTexture(), IntRect(187, 335, 18, 11), Vector2f(mov->getPosition().x + 14 * 4, mov->getPosition().y + 24 * 4), Vector2f(4, 4)));
		platAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(187, 335, 18, 11), IntRect(206, 335, 18, 11)}, platform));
		platTimer = shared_ptr<animTimer>(new animTimer(platAnim, 8, true));

		idleAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(1, 300, 39, 24), IntRect(42, 302, 39, 24)}, sprite));
		idleAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0, 2 * 4)});
	
		idleAnimTimer = shared_ptr<animTimer>(new animTimer(idleAnim, 8, true));

		throwAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(128, 306, 35, 23)}, sprite));
		throwAnim->setOffsetList(list<Vector2f>{Vector2f(4*4, 6*4)});

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 18, 24), mov));
		hurt = hit;

		hp = 3;

		setCode();
		angle = 270;
	}

	void setCode() {
		code = "lightning lord";
	}
	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {

		move(deltaT);
		hit->updatePos();

		platformUpdate(deltaT);

		throwReel_left -= *deltaT;



		if (throwReel_left <= 0) {
			throwTimer_left -= *deltaT;

			idleAnimTimer->run(deltaT);

			if (throwTimer_left <= 0) {
				throwAnim->thisFrame();
				shoot(bList, p);
				throwReel_left = throwReel;
				throwTimer_left = throwTimer;
			}
		}

	}

	void platformUpdate(float* deltaT) {
		platTimer->run(deltaT);
		platform->setPosition(Vector2f(mov->getPosition().x + 14 * 4, mov->getPosition().y + 24 * 4));
	}

	virtual void move(float* deltaT) {
		angle = angle + (angleChange * *deltaT);

		mov->move(angle, deltaT, speed);
	}

	void shoot(list<shared_ptr<EnemyBullet>>* bList, shared_ptr<player> p) {

		float throwAngle = Maths::getAngle(sprite->getMiddlePos(), p->getMiddlePos());

		shared_ptr<Lightning> temp = shared_ptr<Lightning>(new Lightning(sprite->getTexture(), Vector2f(mov->getPosition().x + 14 * 4, mov->getPosition().y + 24 * 4), throwAngle));
		bList->push_back(temp);
	}


	void spawnObject(list<shared_ptr<GameObject>>* objects, shared_ptr<SoundCollection> soundCol) {

		bool spawn = true;

		for (shared_ptr<GameObject> ob : *objects) {
			if (ob->getCode() == "fly platform") {
				spawn = false;
				break;
			}
		}

		if (spawn) {

			shared_ptr<FlyPlatform> temp = shared_ptr<FlyPlatform>(new FlyPlatform(sprite->getTexture(), Vector2f(mov->getPosition().x + 14 * 4, mov->getPosition().y + 24 * 4)));
			temp->setMoveInCircle(angle, angleChange, speed);

			objects->push_back(temp);
		}
	}

	list<shared_ptr<objectSprite>> getExtraSprites() {
		return list<shared_ptr<objectSprite>>{platform};
	}

	void uniqueDeathLoop(float* deltaT) {
		platform->setPosition(Vector2f(-9999999, -99999999));
	}
};