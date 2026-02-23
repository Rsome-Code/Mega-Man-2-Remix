#include "anim tile.cpp"
#pragma once

class FlashTile : public tile {

	int timing;

	float beat = 0.38216560509554140127388535031847;
	float beat_left = 0;

	int currentBeat = 0;

	shared_ptr<Text> beatText;

	float frictD = 7000;

public:

	void deleteInt() {



		//delete& beat;
		//delete& beat_left;
		//delete& currentBeat;
		//delete& frictD;

		deleteHits();

	}

	void setTiming() {
		timing = (int(location.x) + int (location.y)) % 3;
		type = type + "-" + to_string(timing);

	}



	void setTiming(int t) {
		timing = t;
		type = type + "-" + to_string(timing);
	}

	void textUpdate(shared_ptr<Font> font) {


		beatText = shared_ptr<Text> (new Text());
		beatText->setString(to_string(timing + 1));
		beatText->setFillColor(Color::Black);
		beatText->setPosition(sprite->getCameraPosition());
		beatText->setFont(*font);
		beatText->setCharacterSize(20);
	}

	shared_ptr<Text> getText(shared_ptr<Font> font) {
		textUpdate(font);

		return beatText;
	}

	void animate(float* deltaT) {
		beat_left -= *deltaT;

		if (beat_left <= 0) {
			beat_left = beat;

			if (currentBeat == timing) {
				sprite->setRect(IntRect(64, sprite->getRect().getPosition().y, 16, 16));

			}
			currentBeat = (currentBeat + 1) % 3;

			if (currentBeat == timing) {
				sprite->setRect(IntRect(80, sprite->getRect().getPosition().y, 16, 16));
				
			}
		}
	}

	float getFrictionDecrease() {
		return frictD;
	}

	int getTiming() {
		return timing;
	}

	void resetBeat() {
		sprite->setRect(IntRect(64, sprite->getRect().getPosition().y, 16, 16));
	}

	//void animate(float* deltaT) {

	//}



	virtual void deleteHits() {};

};