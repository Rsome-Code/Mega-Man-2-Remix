#include "dis tile.cpp"
#include "GObject.cpp"
#include "animation timer.cpp"
#include <sfml/audio.hpp>
#pragma once

class DisappearingTile : public GameObject {
	solidTile* t;
	int currentBeat = 0;
	int maxBeat = 4;
	int onBeat;

	float beatTime = 1.25;
	float beatTime_left = beatTime;
	
	animation* anim;
	animTimer* timer;


	bool on = false;

	Sound* sound;
	
public:
	DisappearingTile(Texture* tex, Vector2f loc, int onBeat) {
		this->onBeat = onBeat;
		Vector2f location = Vector2f(loc.x / 16, loc.y / 16);
		t = new DisTile(location, tex, 0);

		//t->getSprite()->setTexture(tex);

		t->getSprite()->setRect(IntRect(641, 635, 16, 19));


		anim = new animation(list<IntRect>{IntRect(641, 635, 16, 19), IntRect(658, 635, 16, 22), IntRect(675, 635, 16, 22), IntRect(692, 635, 16, 22), IntRect(709, 635, 16, 22)}, t->getSprite());
		timer = new animTimer(anim, 10, false);

		sprite = new objectSprite("h", tex, IntRect(641 + (onBeat *16), 662, 16, 19), loc, Vector2f(4, 4));
		

		setCode("disappearing tile-" + to_string(onBeat));

		display = true;
		t->setAct(true);


		//sprite = t->getSprite();
		t->setDisplay(true);

		t->getSprite()->setPosition(loc);
		
	}

	void initial() {
		t->setDisplay(false);
	}

	void setDisplay(bool b) {
		display = false;
	}

	void eachFrame(float* deltaT, objectSprite* player, camera* cam) {

		t->getSprite()->setCameraPosition(t->getSprite()->getPosition() - cam->getPosition());

		sprite = t->getSprite();

		t->update();
		
		//t->setDisplay(false);
		beatTime_left -= *deltaT;

		if (on) {
			timer->run(deltaT);	
		}
		else {
			t->setAct(false);
		}


		if (beatTime_left <= 0) {
			beatTime_left = beatTime;
			currentBeat = (currentBeat + 1) % 4;

			if (currentBeat == onBeat) {
				timer->reset();
				anim->reset();
				on = true;

				if (t->getSprite()->getCameraPosition().x > 0 && t->getSprite()->getCameraPosition().x < 1920) {
					
					//Sound* s = *sound;
					if (sound->getStatus() != Sound::Playing) {
						sound->play();
					}
					else {
						cout << "playing\n";
					}
					
				}
			}
			else if (currentBeat == (onBeat + 2) % 4) {
				on = false;
				//setDisplay(false);

				
			}
		}

		if (on) {
			t->setAct(true);
			t->setDisplay(true);
		}
		else {
			t->setAct(false);
			t->setDisplay(false);
		}

	}

	list<tile*> getTiles(){
		return (list<tile*>{t});
	}

	virtual void setSoundPointer(Sound* sou) {
		sound = sou;
	}

};