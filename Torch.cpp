#include "GObject.cpp"
#include "light source.cpp"

class Torch : public GameObject {

	LightSource* light;
	shared_ptr<Texture> t;
	Vector2f pos;
	Colour col;
	float range;
	float brightness;

public:

	virtual ~Torch() {
		delete light;
		
	}

	Torch(shared_ptr<Texture> t, Vector2f pos, Colour col, float range, float brightness) {
		sprite = shared_ptr<objectSprite>(new objectSprite("ob-1", t, IntRect(832, 23, 26, 16), pos, Vector2f(4,4), 1));
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
		sprite = shared_ptr<objectSprite>(new objectSprite("ob-1", t, IntRect(832, 23, 26, 16), pos, Vector2f(4, 4), 1));
		light = new LightSource(col, range, brightness);
		display = false;
		act = false;
	}
};