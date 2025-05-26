#include "object.cpp"
#include "transition angle.cpp"
#pragma once

using namespace sf;
using namespace std;

class EndFlag : public object {

	enum transitionAngle angle;

public:
	EndFlag(Texture* t, Vector2f pos) {
		sprite = new objectSprite("Flag", t, IntRect(288, 685,17,27), pos, Vector2f(4,4), 1);
		angle = RIGHT;
	}

	EndFlag(Texture* t, Vector2f pos, enum transitionAngle ang) {
		if (ang == RIGHT) {
			sprite = new objectSprite("Flag", t, IntRect(288, 685, 17, 27), pos, Vector2f(4, 4), 1);
		}
		else if (ang == UP) {
			sprite = new objectSprite("Flag", t, IntRect(310, 685, 18, 27), pos, Vector2f(4, 4), 1);
		}
		else if (ang == DOWN) {
			sprite = new objectSprite("Flag", t, IntRect(342, 685, 17, 27), pos, Vector2f(4, 4), 1);
		}
		angle = ang;
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
};