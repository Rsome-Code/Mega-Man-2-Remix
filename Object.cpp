#include "object Sprite.cpp"
#include "object hitbox.cpp"
#include "light source.cpp"
#include "transition angle.cpp"
#include "camera.cpp"


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
		sprite = NULL;
	}

	void setCode(string cod) {
		code = cod;
	}

	virtual void eachFrame(float* deltaT, objectSprite* player, camera* cam) {};
	virtual void eachFrame(float* deltaT, objectSprite* player) {};
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

	virtual bool checkHurt(objectHitbox* bullet) { return false; };
	virtual bool checkHit(objectHitbox* pHit) { return false; };
	virtual objectHitbox* getHitbox() { return NULL; };
	virtual objectHitbox* getHurtbox() {return NULL;};
	virtual int getDamage() { return NULL; };
	virtual LightSource* getLightSource() { return NULL; };
	virtual void setInitOffScreen(bool o) {};

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
	virtual void spawnItem(list<object*>* obList, Texture* t, Vector2f pos) {
	}
	virtual void setCheckpoint(){}
	Vector2f getPosition() {
		return sprite->getPosition();
	}
	virtual bool getCheckpoint() { return false; };
	virtual void setCamera(camera* camer) {};
	void forceDisplay(bool b) {
		display = true;
	}

	void setPallete(string levelName) {};

	virtual void setPosition(Vector2f pos) {
		sprite->setPosition(pos);
	}
};