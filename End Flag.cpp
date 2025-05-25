#include "object.cpp"
#pragma once

using namespace sf;
using namespace std;

class EndFlag : public object {

public:
	EndFlag(Texture* t, Vector2f pos) {
		sprite = new objectSprite("Flag", t, IntRect(288, 685,17,27), pos, Vector2f(4,4), 1);
	}


	void setCode() {
		code = "flag";
	}
};