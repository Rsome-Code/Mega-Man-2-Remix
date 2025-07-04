#include "object.cpp"
class damageEffect:public object {
	float flickerSpeed = 0.1;
	float tempF;
	objectSprite* follow;

public:
	damageEffect(objectSprite* fol) {
		follow = fol;
		Texture* dET = new Texture();
		dET->loadFromFile("Assets\\misc\\mega buster.png");
		sprite = new objectSprite("effect", dET, Vector2i(1, 22), Vector2i(24, 24), fol->getPosition(), Vector2f(fol->getScale().x + 1, fol->getScale().y + 1), 1);
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