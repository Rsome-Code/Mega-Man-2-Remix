#include <SFML/graphics.hpp>
#include "camera.cpp"
#include "object.cpp"
#pragma once


class Background:public object {
	camera* cam;

public:
	Background(Color col) {

		sprite = new objectSprite();
		sprite->getSprite()->setColor(col);
		sprite->setRect(IntRect(0,0,1, 1));
		sprite->setScale(Vector2f(200, 200));

		sprite->setTexture(new Texture());

		setCode("background-" + to_string(col.toInteger()));

	}

	Background(int col) {

		sprite = new objectSprite();
		sprite->getSprite()->setColor(Color::Color(col));
		sprite->setRect(IntRect(0, 0, 1, 1));
		sprite->setScale(Vector2f(200, 200));

		sprite->setTexture(new Texture());

		setCode("background-" + to_string(col));


	}

	void setCamera(camera* camer) {
		cam = camer;
	}

	virtual void eachFrame(float* deltaT, objectSprite* player) {
		sprite->setScale(Vector2f(1940, 1080));
		sprite->setPosition(Vector2f(cam->getPosition().x - 10, cam->getPosition().y));
	};
};