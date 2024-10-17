#include "object Sprite.cpp"
#pragma once


//When creating any kind of object, ALWAYS extend from this class.

class object {
protected:
	objectSprite* sprite;
	bool display = true;
	bool act = true;

public:
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
};