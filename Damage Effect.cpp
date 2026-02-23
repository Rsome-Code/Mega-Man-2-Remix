#include "object.cpp"
class damageEffect:public object {
	float flickerSpeed = 0.1;
	float tempF;
	shared_ptr<objectSprite> follow;

public:
	damageEffect(shared_ptr<objectSprite> fol) {
		follow = fol;
		shared_ptr<Texture> dET = shared_ptr<Texture> (new Texture());
		dET->loadFromFile("Assets\\misc\\mega buster.png");
		sprite = shared_ptr<objectSprite>(new objectSprite("effect", dET, Vector2i(1, 22), Vector2i(24, 24), fol->getPosition(), Vector2f(fol->getScale().x + 1, fol->getScale().y + 1), 1));
		setDisplay(false);
		sprite->setVisualOffset(Vector2f(-8, 0));

	}

	void flicker(float* deltaT) {
		tempF = tempF - *deltaT;
		if (tempF <= 0) {
			tempF = flickerSpeed;
			if (getDisplay()) {
				setDisplay(false);
			}
			else {
				setDisplay(true);
			}
		}
		sprite->setPosition(follow->getPosition());
	}
	void reset() {
		tempF = flickerSpeed;
		setDisplay(false);
	}
};