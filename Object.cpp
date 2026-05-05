#include "object Sprite.cpp"
#include "object hitbox.cpp"
#include "light source.cpp"
#include "transition angle.cpp"
#include "camera.cpp"


#pragma once


//When creating any kind of object, ALWAYS extend from this class.

class object {
protected:
	shared_ptr<objectSprite> sprite;
	bool display = true;
	bool act = true;
	string code;

public:

	virtual ~object() {
		//delete sprite;
	}

	object() {

	}
	object(shared_ptr<object> o) {
		sprite = shared_ptr<objectSprite>(new objectSprite(o->getSprite()));
		display = o->getDisplay();
		act = o->getAct();
		code = o->getCode();
	}

	virtual void setCode() {};

	string getCode() {
		return code;
	}

	virtual shared_ptr<objectSprite> getSprite() {
		return sprite;
	}
	bool getDisplay() {
		return display;
	}
	virtual void setDisplay(bool b) {
		display = b;
	}
	bool getAct() {
		return act;
	}
	void setAct(bool b) {
		act = b;
	}

	void deleteSprite() {
		//sprite->deleteStuff();
		//delete sprite;
		//sprite = NULL;
	}

	void setCode(string cod) {
		code = cod;
	}

	virtual void eachFrame(float* deltaT, shared_ptr<objectSprite> player, shared_ptr<camera> cam) {};
	virtual void eachFrame(float* deltaT, shared_ptr<objectSprite> player) {};
	virtual void initial() {};
	virtual bool getOffScreen() { return false; };
	virtual void reset() {};
	virtual void setOffScreen(bool b) {};
	virtual Vector2f getInitialPosition() { return Vector2f(0,0); };
	//virtual void offsetList() {};
	virtual void lowerHP(int h) {};
	virtual int busterDam() { return NULL; };
	virtual int atomicDam() { return NULL; };
	virtual int bubbleDam() { return NULL; };
	virtual int metalDam() { return NULL; };
	virtual int quickDam() { return NULL; };
	virtual int airDam() { return NULL; };
	virtual int leafDam() { return NULL; };
	virtual int flashDam() { return NULL; };
	virtual int crashDam() { return NULL; };

	virtual bool checkHurt(shared_ptr<objectHitbox> bullet) { return false; };
	virtual bool checkHit(shared_ptr<objectHitbox> pHit) { return false; };
	virtual shared_ptr<objectHitbox> getHitbox() { return NULL; };
	virtual shared_ptr<objectHitbox> getLeft() { return NULL; };
	virtual shared_ptr<objectHitbox> getRight() { return NULL; };
	virtual shared_ptr<objectHitbox> getHurtbox() {return NULL;};
	virtual int getDamage() { return NULL; };
	virtual LightSource* getLightSource() { return NULL; };
	virtual void setInitOffScreen(bool o) {};

	virtual list<shared_ptr<objectSprite>> getSprites() { return list<shared_ptr<objectSprite>>{NULL}; };

	virtual bool getInitOffScreen() {
		return NULL;
	}

	virtual int getHP() { return NULL; };

	virtual int getSection(){ return NULL; };

	virtual int getIncrease() { return NULL; };
	virtual void used(){};

	virtual enum transitionAngle getAngle() {
		enum transitionAngle r = RIGHT;
		return r;
	}
	virtual void spawnItem(list<shared_ptr<object>>* obList, shared_ptr<Texture> t, Vector2f pos) {
	}
	virtual void setCheckpoint(){}
	Vector2f getPosition() {
		return sprite->getPosition();
	}
	virtual bool getCheckpoint() { return false; };
	virtual void setCamera(shared_ptr<camera> camer) {};
	void forceDisplay(bool b) {
		display = true;
	}

	virtual void setPallete(string levelName) {};

	virtual void setPosition(Vector2f pos) {
		sprite->setPosition(pos);
	}
};