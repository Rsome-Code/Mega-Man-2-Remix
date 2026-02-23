#include "UI Sprite.cpp"
#include "Maths.cpp"
#include <sfml/audio.hpp>
#pragma once;

class AmmoBar {
	list<shared_ptr<UISprite>> sprites;
	int barMax = 28;

	int barVal;
	Vector2f position;

	IntRect four = IntRect(0, 0, 8, 8);
	IntRect three = IntRect(9, 0, 8, 8);
	IntRect two = IntRect(18, 0, 8, 8);
	IntRect one = IntRect(27, 0, 8, 8);
	IntRect zero = IntRect(36, 0, 8, 8);

	shared_ptr<SoundBuffer> soundB;
	shared_ptr<Sound> sound;



public:

	virtual ~AmmoBar() {


		
	}

	AmmoBar(shared_ptr<Texture> t, Vector2f pos) {
		position = pos;


		for (int i = 0; i < 7; i++) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite("option", t, four, Vector2f (position.x + (i*8) * 4, position.y), Vector2f(4,4)));
			sprites.push_back(temp);
		}

		soundB = shared_ptr<SoundBuffer> (new SoundBuffer());
		soundB->loadFromFile("assets\\sound\\refill.wav");
		sound = shared_ptr<Sound> (new Sound());
		sound->setBuffer(*soundB);
	}

	void setVertical() {
		int i = 6;
		for (shared_ptr<UISprite> sprite : sprites) {
			sprite->setCameraPosition(Vector2f(position.x, position.y + (i * 8) * 4));
			i--;
		}
	}

	void stopSound() {
		sound->stop();
	}

	void updateWithSound(float newVal) {
		if (newVal != barVal) {
			sound->play();
		}

		update(newVal);
	}

	void update(float newVal) {

		//barVal = Maths::map(0, ammoMax, 0, barMax, newVal);
		barVal = ceil(newVal);

		int fullBars = barVal/ 4;
		int left = barVal % 4;

		list<shared_ptr<UISprite>>::iterator listI;
		listI = sprites.begin();

		for (int i = 0; i < fullBars; i++) {
			shared_ptr<UISprite> temp = *listI;
			temp->setRect(four);
			listI = next(listI);
		}
		if (listI != sprites.end()) {
			shared_ptr<UISprite> temp = *listI;
			if (left == 0) {
				temp->setRect(zero);
			}
			else if (left == 1) {
				temp->setRect(one);
			}
			else if (left == 2) {
				temp->setRect(two);
			}
			else if (left == 3) {
				temp->setRect(three);
			}
			listI = next(listI);
		}
		if (listI != sprites.end()) {
			while (listI != sprites.end()){
				shared_ptr<UISprite> temp = *listI;
				temp->setRect(zero);
				listI = next(listI);
			}
		}
	}

	list<shared_ptr<UISprite>> getSprites() {
		return sprites;
	}
};