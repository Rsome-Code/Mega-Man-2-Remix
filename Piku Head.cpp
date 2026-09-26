#include "temp physics enemy.cpp"
#pragma once

class PikuHead : public TempPhysicsEnemy {
	using TempPhysicsEnemy::TempPhysicsEnemy;
	
	shared_ptr<animation> anim;
	shared_ptr<animTimer> timer;

	shared_ptr<objectSprite> shadow;

	float delayStart = 0;

	public:
	void initial() {
		
		phys->setPosition(initialPos);
		phys->setRect(IntRect(373, 403, 20, 24));
		anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(373, 403, 20, 24), IntRect(431, 403, 22, 24)}, phys));
		code = "piku head";
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 20, 24), phys));
		hurt = hit;
		hp = 5;
		damage = 3;

		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(465, 402), Vector2i(24, 24)), IntRect(Vector2i(490, 406), Vector2i(16, 16)), IntRect(Vector2i(509, 408), Vector2i(12, 12)), IntRect(Vector2i(527, 409), Vector2i(10, 10)), IntRect(Vector2i(543, 412), Vector2i(4, 4)), IntRect(Vector2i(557, 412), Vector2i(4, 4))}, sprite));
		deathTimer->setAnim(deathAnim);
		offSetList();

		shadow = shared_ptr<objectSprite>(new objectSprite());
		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture());
		shadow->setTexture(t);
		shadow->setRect(IntRect(0, 0, 22, 24));
		shadow->setPosition(phys->getPosition());
		shadow->setColour(Colour::Black());
		shadow->setTransparency(255);
		shadow->setScale(Vector2f(4, 4));

		
		
	}

	void uniqueDeathStart() {
		initialPos = Vector2f(-99999, -999999);
	}

	void setDelay(float f) {
		delayStart = f;
	}

	enum State {
		upAndDown, inAndOut
	};

	
	int moveAngle = Angle::left;
	int moveSpeed = 200;

	State state = upAndDown;


	float currentSin = 0;
	float upDownSpeed = 1;
	float middlePos;
	float moveDist = 300;


	bool upDownLoop(float* deltaT) {
		
		currentSin =  currentSin + (upDownSpeed * *deltaT);
		
		double pi = 2 * acos(0.0);

		float y = sin(currentSin * pi) * moveDist;


		phys->setPosition(Vector2f(phys->getPosition().x, middlePos + y));

		return hasLeftScreen();

	
	}

	bool hasLeftScreen() {
		if (moveAngle == Angle::left) {
			if (phys->getCameraPosition().x < 0 - phys->getSize().x)
			{
				moveAngle = Angle::right;
				return true;
			}
		}
		else {
			if (phys->getCameraPosition().x > 1920) {
				moveAngle = Angle::left;
				return true;
			}
		}
		return false;
	}

	float z = 1;
	float middleZ = 1.5;
	float inOutSpeed = 0.7;
	float zDiff = 0.5;
	bool inOutLoop(float* deltaT) {

		z = z + (inOutSpeed * *deltaT);
		
		double pi = 2 * acos(0.0);

		float newZ = sin(z * pi) * zDiff;

		phys->setZ(newZ + middleZ);

		hitboxZ();

		return hasLeftScreen();
	}

	void hitboxZ() {
		if (phys->getZ() < 0.9 || phys->getZ() > 1.1) {
			hit->setPosition(Vector2f(-99999, -99999));
		}
		//else {
			//hit->updatePos();
		//}
	}
	virtual bool getOffScreen() {
		return false;
	}

	bool deleteOverY() {
		return false;
	}
	bool deleteOverX() {
		return false;
	}

	int maxShadow = 200;
	void shadowUpdate() {
		shadow->setPosition(Vector2f(phys->getPosition().x - 4, phys->getPosition().y));
		//2 = 255, 1 = 0

		shadow->setTransparency(((phys->getZ()-1) * 255));
		if (shadow->getTransparency() > maxShadow) {
			shadow->setTransparency(maxShadow);
		}
		shadow->setZ(phys->getZ());
	}

	void delayLoop(float* deltaT) {
		delayStart -= *deltaT;
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<GameObject>>* obList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol, shared_ptr<camera> cam) {
		middlePos = (1080 / 2 + cam->getPosition().y) -100;
		
		if (delayStart <= 0) {

			if (state == upAndDown) {
				if (upDownLoop(deltaT)) {
					state = inAndOut;
				}
			}
			else if (state == inAndOut) {
				
				if (inOutLoop(deltaT)) {
					phys->setZ(1);
					z = 1;
					state = upAndDown;
				}
				//upDownLoop(deltaT);
			}

			phys->move(moveAngle, deltaT, moveSpeed);

			shadowUpdate();
		}
		else {
			delayLoop(deltaT);
		}

		//inOutLoop(deltaT);
	
	}
	list<shared_ptr<objectSprite>> getExtraSprites() {
		return list<shared_ptr<objectSprite>>{shadow};
	}

	void spawnItem(list<shared_ptr<Item>>* obList, shared_ptr<Texture> t, Vector2f pos, shared_ptr<SoundCollection> soundCol) {}

};