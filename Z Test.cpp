#include "movable object.cpp"
#include "time.cpp"
#include "render Logic.cpp"
#pragma once

class ZTest {


	shared_ptr<movable> sprite;

	bool zPressed = true;
	bool xPressed = true;

	bool upPressed = true;
	bool downPressed = true;
	bool leftPressed = true;
	bool rightPressed = true;

	int moveSpeed = 400;

public:
	ZTest() {
		sprite = shared_ptr<movable>(new movable());

		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture());
		//t->loadFromFile("assets/delete.png");


		sprite->setTexture(t);
		sprite->setZ(1);
		sprite->setColour(Colour::Cyan());
		sprite->setRect(IntRect(0, 0, 100, 100));
	}


	void checkControls(float* deltaT) {
		if (Keyboard::isKeyPressed(Keyboard::Scan::Z)) {
			if (!zPressed) {
				sprite->setZ(sprite->getZ() + 0.1);
				zPressed = true;
			}
			
		}
		else if (zPressed) {
			zPressed = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Scan::X)) {
			if (!xPressed) {
				sprite->setZ(sprite->getZ() - 0.1);
				xPressed = true;
			}

		}
		else if (xPressed) {
			xPressed = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Scan::Up)) {

			sprite->move(270, deltaT, moveSpeed);

		}

		if (Keyboard::isKeyPressed(Keyboard::Scan::Down)) {

			sprite->move(90, deltaT, moveSpeed);

		}

		if (Keyboard::isKeyPressed(Keyboard::Scan::Left)) {

			sprite->move(180, deltaT, moveSpeed);

		}		if (Keyboard::isKeyPressed(Keyboard::Scan::Right)) {

			sprite->move(0, deltaT, moveSpeed);

		}

	
		
	}

	void loop(shared_ptr<renderer> instance, float targetRate) {
		shared_ptr<timer> time = shared_ptr<timer>(new timer());


		auto start = time->timerStart();
		auto* startP = &start;
		float deltaT = 0;


		bool run = true;

		shared_ptr<camera> cam = shared_ptr<camera>(new camera(Vector2f(0,0)));


		while (instance->getWindow()->isOpen() && run) {
			Event event;
			while (instance->getWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					instance->getWindow()->close();
			}
			time->frameLimiter(targetRate, startP);
			deltaT = time->checkTimer(startP);
			start = time->timerStart();
			startP = &start;



			checkControls(&deltaT);


			//instance->objectDisplay(sprite, cam);

			instance->bObjectDisplay(sprite, cam);


			instance->getWindow()->display();
			instance->getWindow()->clear();
		}
	}
};