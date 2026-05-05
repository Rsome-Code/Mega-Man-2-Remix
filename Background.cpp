#include <SFML/graphics.hpp>
#include "camera.cpp"
#include "GObject.cpp"
#pragma once


class Background:public GameObject {
	shared_ptr<camera> cam;

public:
	Background(Colour col) {

		sprite = shared_ptr<objectSprite>(new objectSprite());
		sprite->setColour(col);
		sprite->setRect(IntRect(0,0,1, 1));
		sprite->setScale(Vector2f(200, 200));

		sprite->setTexture(shared_ptr<Texture> (new Texture()));

		setCode("background-" + to_string(col.toInteger()));

	}

	Background(int col) {

		sprite = shared_ptr<objectSprite>(new objectSprite());
		sprite->setColour(Colour::Colour(col));
		sprite->setRect(IntRect(0, 0, 1, 1));
		sprite->setScale(Vector2f(200, 200));

		sprite->setTexture(shared_ptr<Texture> (new Texture()));

		setCode("background-" + to_string(col));


	}

	void setCamera(shared_ptr<camera> camer) {
		cam = camer;
	}

	virtual void eachFrame(float* deltaT, shared_ptr<objectSprite> player) {
		sprite->setScale(Vector2f(1940, 1080));
		sprite->setPosition(Vector2f(cam->getPosition().x - 10, cam->getPosition().y));
	};
};