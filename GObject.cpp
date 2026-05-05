#include "object.cpp"
#include "tile.cpp"
#include "player.cpp"
#include <SFML/Audio.hpp>
#pragma once

class GameObject : public object {
protected:
	shared_ptr<SoundBuffer> soundB;
	shared_ptr<Sound> sound;

	int section = NULL;

public:

	virtual ~GameObject() {
	
	}

	GameObject(){
	
	}
	GameObject(shared_ptr<GameObject> o) {
		sprite = shared_ptr<objectSprite>(new objectSprite(o->getSprite()));
		display = o->getDisplay();
		act = o->getAct();
		code = o->getCode();
	}
	virtual list<shared_ptr<tile>> getTiles() { return list<shared_ptr<tile>> {}; }

	void setSoundB(shared_ptr<SoundBuffer> soundB) {
		this->soundB = soundB;
		sound = shared_ptr<Sound>(new Sound());
		sound->setBuffer(*soundB);

	}
	void setSound(shared_ptr<Sound> s) {
		sound = s;
	}


	virtual void setPallete(string pal) {
		if (pal == "wood man") {
			woodmanPallete();
		}
		else if (pal == "heat man") {
			heatmanPallete();
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

	virtual void eachFrame(float* deltaT, shared_ptr<player> player, shared_ptr<camera> cam) {};
	virtual void eachFrame(float* deltaT, shared_ptr<objectSprite> player, shared_ptr<camera> cam) {};
	virtual void eachFrame(float* deltaT, shared_ptr<objectSprite> player, shared_ptr<camera> cam, list<shared_ptr<tile>>* tileList) {};
	virtual void eachFrame(float* deltaT, shared_ptr<player> player, shared_ptr<camera> cam, list<shared_ptr<tile>>* tileList) {};
	virtual void eachFrame(float* deltaT, shared_ptr<objectSprite> player){};
	virtual void eachFrame(float* deltaT, shared_ptr<objectSprite> player, list<shared_ptr<GameObject>> enemies) {};
	virtual void eachFrame(float* deltaT, shared_ptr<player> p, shared_ptr<camera> cam, list<shared_ptr<GameObject>> objects) {};
	virtual void eachFrame() {};


	virtual void setSoundPointer(shared_ptr<Sound> sou) {

	}

	virtual void initial() {};

	virtual list<shared_ptr<objectSprite>> getSprites() {
		return list<shared_ptr<objectSprite>> {sprite};
	}

	virtual void deleteInt() {

	}

	virtual void setSection(int sect) {
		section = sect;
	}
	virtual int getSection() {
		return section;
	}

	
};