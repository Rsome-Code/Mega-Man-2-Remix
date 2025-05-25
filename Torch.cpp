#include "object.cpp"
#include "light source.cpp"

class Torch : public object {

	LightSource* light;

public:
	Torch(Texture* t, Vector2f pos, Color col, float range, float brightness) {
		sprite = new objectSprite("ob-1", t, IntRect(832, 23, 26, 16), pos, Vector2f(4,4), 1);
		light = new LightSource(col, range, brightness);

	}

	void setCode() {
		code = "trch-R";
	}

	LightSource* getLightSource() {
		return light;
	}
};