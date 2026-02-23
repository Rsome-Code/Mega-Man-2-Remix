#include "GObject.cpp"
#include "transition angle.cpp"
#pragma once

using namespace sf;
using namespace std;

class EndFlag : public GameObject {

	enum transitionAngle angle;
	int section;
	bool checkpoint = false;

public:
	EndFlag(shared_ptr<Texture> t, Vector2f pos, int section) {
		sprite = shared_ptr<objectSprite>(new objectSprite("Flag", t, IntRect(288, 685,17,27), pos, Vector2f(4,4), 1));
		angle = RIGHT;
		this->section = section;
		setCode();
	}

	EndFlag(shared_ptr<Texture> t, Vector2f pos, enum transitionAngle ang, int section) {
		if (ang == RIGHT) {
			sprite = shared_ptr<objectSprite>(new objectSprite("Flag", t, IntRect(288, 685, 17, 27), pos, Vector2f(4, 4), 1));
		}
		else if (ang == UP) {
			sprite = shared_ptr<objectSprite>(new objectSprite("Flag", t, IntRect(310, 685, 18, 27), pos, Vector2f(4, 4), 1));
		}
		else if (ang == DOWN) {
			sprite = shared_ptr<objectSprite>(new objectSprite("Flag", t, IntRect(342, 685, 17, 27), pos, Vector2f(4, 4), 1));
		}
		angle = ang;
		setCode();
		this->section = section;
	}

	void setCheckpoint() {
		checkpoint = true;
	}

	bool getCheckpoint() {
		return checkpoint;
	}

	enum transitionAngle getAngle() {
		return angle;
	}

	void setCode() {
		if (angle == RIGHT) {
			code = "flag";
		}
		else if (angle == DOWN) {
			code = "flag-down";
		}
		else if (angle == UP) {
			code = "flag-up";
		}

	}

	int getSection() {
		return section;
	}


};