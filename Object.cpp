#include "object Sprite.cpp"
#include "object hitbox.cpp"
#pragma once


//When creating any kind of object, ALWAYS extend from this class.

class object {
protected:
	objectSprite* sprite;
	bool display = true;
	bool act = true;
	string code;

public:
	object() {

	}
	object(object* o) {
		sprite = new objectSprite(o->getSprite());
		display = o->getDisplay();
		act = o->getAct();
		code = o->getCode();
	}

	virtual void setCode() {};

	string getCode() {
		return code;
	}

	objectSprite* getSprite() {
		return sprite;
	}
	bool getDisplay() {
		return display;
	}
	void setDisplay(bool b) {
		display = b;
	}
	bool getAct() {
		return act;
	}
	void setAct(bool b) {
		act = b;
	}

	virtual void eachFrame(float* deltaT, objectSprite* player) {};
	virtual void initial() {};
	virtual bool getOffScreen() { return false; };
	virtual void reset() {};
	virtual void setOffScreen(bool b) {};
	virtual Vector2f getInitialPosition() { return Vector2f(NULL, NULL); };
	virtual void offsetList() {};
	virtual void lowerHP(int h) {};
	virtual int busterDam() { return NULL; };
	virtual bool checkHurt(objectHitbox* bullet) { return false; };
	virtual bool checkHit(objectHitbox* pHit) { return false; };
	virtual objectHitbox* getHitbox() { return NULL; };
	virtual int getDamage() { return NULL; };
};