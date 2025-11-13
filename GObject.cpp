#include "object.cpp"
#include "tile.cpp"
#include "player.cpp"
#include <SFML/Audio.hpp>
#pragma once

class GameObject : public object {
protected:
	SoundBuffer* soundB;
	Sound* sound;

public:
	GameObject(){
	
	}
	GameObject(GameObject* o) {
		sprite = new objectSprite(o->getSprite());
		display = o->getDisplay();
		act = o->getAct();
		code = o->getCode();
	}
	virtual list<tile*> getTiles() { return list<tile*> {NULL}; }

	void setSoundB(SoundBuffer* soundB) {
		this->soundB = soundB;
		sound = new Sound();
		sound->setBuffer(*soundB);

	}
	void setSound(Sound* s) {
		sound = s;
	}

	virtual void setPallete(string pal) {
		if (pal == "wood man") {
			woodmanPallete();
		}
		else if (pal == "heat man") {
			woodmanPallete();
		}
		else if(pal == "metal man") {
			metalmanPallete();
		}
		else if (pal == "air man") {
			airmanPallete();
		}
		else if (pal == "crash man") {
			crashmanPallete();
		}
		else if (pal == "flash man") {
			flashmanPallete();
		}
		else if (pal == "quick man") {
			quickmanPallete();
		}
		else if (pal == "bubble man") {
			bubblemanPallete();
		}
	}

	virtual void woodmanPallete(){}
	virtual void heatmanPallete() {}
	virtual void metalmanPallete() {}
	virtual void airmanPallete() {}
	virtual void crashmanPallete() {}
	virtual void flashmanPallete() {}
	virtual void quickmanPallete() {}
	virtual void bubblemanPallete() {}

	virtual void eachFrame(float* deltaT, player* player, camera* cam) {};
	virtual void eachFrame(float* deltaT, objectSprite* player, camera* cam) {};
	virtual void eachFrame(float* deltaT, objectSprite* player, camera* cam, list<tile*>* tileList) {};
	virtual void eachFrame(float* deltaT, player* player, camera* cam, list<tile*>* tileList) {};
	virtual void eachFrame(float* deltaT, objectSprite* player){};

	virtual void setSoundPointer(Sound* sou) {

	}

	virtual void initial() {};

	virtual list<objectSprite*> getSprites() {
		return list<objectSprite*> {sprite};
	}
};