#include "gobject.cpp"
#pragma once

class LightingRect : public GameObject {
	
	shared_ptr<camera> cam;
	float transparency = maxTrans;
	int minTrans = 80;
	int maxTrans = 255;

	int minRed = 0;
	int maxRed = 80;

	float red = 0;

	int changeSpeed = 150;

	//Color* col = new Color(0, 0, 0, 0);

public:
	LightingRect() {

		sprite = shared_ptr<objectSprite>(new objectSprite());
		sprite->getSprite()->setColor(sf::Color::Black);
		sprite->setRect(IntRect(0, 0, 1, 1));
		sprite->setScale(Vector2f(200, 200));

		sprite->setTexture(shared_ptr<Texture> (new Texture()));

		setCode("lighting rect");

		//sprite->setColour(&sf::Color::Red);

		

	}



	void setCamera(shared_ptr<camera> camer) {
		cam = camer;
	}

	virtual void eachFrame(float* deltaT, shared_ptr<objectSprite> player, list<shared_ptr<GameObject>> enemies) {

		float thisChange = changeSpeed * *deltaT;

		sprite->setScale(Vector2f(1940, 1080));
		sprite->setPosition(Vector2f(cam->getPosition().x - 10, cam->getPosition().y));

		transparency = (transparency + thisChange) - ((thisChange * 2) * checkEnemies(enemies));
		red = (red - (thisChange)) + (((thisChange*0.8) * 2) * checkEnemies(enemies));


		if (transparency > maxTrans) {
			transparency = maxTrans;
		}
		else if (transparency < minTrans) {
			transparency = minTrans;
		}

		if (red > maxRed) {
			red = maxRed;
		}
		else if (red < minRed) {
			red = minRed;
		}

		

		//sprite->setTransparency(transparency);
		sprite->setColour(Color(red, 0, 0, transparency));
	};

	bool checkEnemies(list<shared_ptr<GameObject>> enemies) {
		for (shared_ptr<GameObject> e : enemies) {
			if (e->getDisplay()) {
				return true;
			}
		}
		return false;
	}
};
