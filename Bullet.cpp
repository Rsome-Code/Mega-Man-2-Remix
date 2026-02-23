#include "movable object.cpp"
#include "object hitbox.cpp"
#include "object.cpp"
#include "tile.cpp"
#include "hitbox detector.cpp"
#include <SFML/audio.hpp>
#include "sound collection.cpp"

#pragma once

class bullet {
protected:
	shared_ptr<movable> sprite;
	bool shooting = false;
	float shootTime;
	float shootTemp;

	shared_ptr<objectSprite> origin;
	shared_ptr<objectHitbox> hitbox;
	float speed = 1750;
	bool right;

	float direction;
	shared_ptr<Sound> dink;
	shared_ptr<SoundBuffer> dinkB;

	bool deflected = false;

public:


	virtual void specialColl(shared_ptr<object> ob) {};


	void dinkSetup(shared_ptr<SoundCollection> soundCol) {
		dink = soundCol->getDink();
	}

	virtual void setSounds(shared_ptr<SoundCollection> soundCol) {
		dinkSetup(soundCol);
	}

	virtual list<shared_ptr <objectSprite>> getSprites() {
		return list< shared_ptr <objectSprite>> {NULL};
	}

	bullet() {}

	virtual bool eachFrame(float* deltaT) = 0;
	virtual void start(bool r) {};
	virtual void secondFire() {};
	virtual bool release(bool r) { return false; };

	bool getShooting() {
		return shooting;
	}

	shared_ptr<objectSprite> getSprite() {
		return sprite;
	}

	virtual list<shared_ptr<UISprite>> getUISprites() {
		return list<shared_ptr<UISprite>>{};
	}


	virtual shared_ptr<objectHitbox> getHitbox() { return hitbox; };
	virtual void onHit(shared_ptr<object> e) {
		if (e->getHP() > 0) {
			shootReset();
		}
	};
	virtual int checkDamage(shared_ptr<object> en) { return NULL; };
	virtual void hold(float* deltaT) {};
	virtual float getHoldTime() { return NULL; };
	virtual float getMaxHoldTime() { return NULL; };
	
	virtual void deflect() {
		
		if (!right) {
			direction = 330;
		}
		else {
			direction = 210;
		}

		dink->play();
		
	}

	void setDirection(float num) {
		direction = num;
	}

	virtual void shootReset() {
		sprite->setPosition(Vector2f(0, 0));
		hitbox->updatePos();
		shooting = false;
		uniqueReset();
	}

	virtual void uniqueReset() {};

	virtual void tileCollision(list<shared_ptr<tile>> tileList) {};
	
};