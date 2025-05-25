#include "object.cpp"
#include "light source.cpp"

class Torch : public object {

	LightSource* light;
	Texture* t;
	Vector2f pos;
	Color col;
	float range;
	float brightness;

public:
	Torch(Texture* t, Vector2f pos, Color col, float range, float brightness) {
		sprite = new objectSprite("ob-1", t, IntRect(832, 23, 26, 16), pos, Vector2f(4,4), 1);
		light = new LightSource(col, range, brightness);
		this->t = t;
		this->pos = pos;
		this->col = col;
		this->range = range;
		this->brightness = brightness;

	}

	void setCode() {
		code = "trch-R";
	}

	LightSource* getLightSource() {
		return light;
	}

	void initial() {
		sprite = new objectSprite("ob-1", t, IntRect(832, 23, 26, 16), pos, Vector2f(4, 4), 1);
		light = new LightSource(col, range, brightness);
		display = false;
		act = false;
	}
};