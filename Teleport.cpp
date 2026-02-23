#include "movable object.cpp"
#include "animate.cpp"
#include "Animation Timer.cpp"
#include "tile.cpp"
#include "hitbox detector.cpp"
#include <SFML/audio.hpp>

#pragma once
class teleport {
	shared_ptr<movable> sprite;
	float speed = float(2000);
	float startX;
	Vector2f startLoc;
	bool initial;
	shared_ptr<animation> teleportAnim;
	shared_ptr<animTimer> timer;


	bool hitFloor = false;

	bool looped = false;

	shared_ptr<Sound> teleSound;
	shared_ptr<SoundBuffer> teleB;

	

public:

	virtual ~teleport() {

		//delete sprite;

		//delete teleportAnim;
		//delete timer;

	}

	teleport(shared_ptr<movable> sprite, float startX, float startY){
		this->sprite = sprite;
		this->startX = startX;
		initial = true;
		startLoc = Vector2f(startX, startY);
		this->sprite->setPosition(startLoc);

		teleportAnim = shared_ptr<animation>(new animation(list<IntRect> {IntRect(Vector2i(124, 128), Vector2i(7, 24)), IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(163, 136), Vector2i(22, 15)), IntRect(Vector2i(134, 132), Vector2i(22, 19))}, sprite));
		
		teleportAnim->setOffsetList(list<Vector2f> {Vector2f(sprite->getScale().x * 5, sprite->getScale().y * 2), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 11), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7)});

		teleportAnim->thisFrame();
		timer = shared_ptr<animTimer> (new animTimer(teleportAnim, 12, false));

		teleB = shared_ptr<SoundBuffer> (new SoundBuffer());
		teleB->loadFromFile("assets\\sound\\teleport_in.wav");

		teleSound = shared_ptr<Sound>(new Sound());
		teleSound->setBuffer(*teleB);
	}

	teleport(shared_ptr<movable> sprite, float startX) {
		this->sprite = sprite;

		this->startX = startX;
		initial = false;
		teleportAnim = shared_ptr<animation>(new animation(list<IntRect> {IntRect(Vector2i(124, 128), Vector2i(7, 24)), IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(163, 136), Vector2i(22, 15)), IntRect(Vector2i(134, 132), Vector2i(22, 19))}, sprite));

		teleportAnim->setOffsetList(list<Vector2f> {Vector2f(sprite->getScale().x * 5, sprite->getScale().y * 2), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 11), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7)});

		teleportAnim->thisFrame();
		timer = shared_ptr<animTimer> (new animTimer(teleportAnim, 12, false));
		teleB = shared_ptr<SoundBuffer> (new SoundBuffer());
		teleB->loadFromFile("assets\\sound\\teleport_in.wav");

		teleSound = shared_ptr<Sound>(new Sound());
		teleSound->setBuffer(*teleB);

	}

	bool eachFrame(float* deltaT, list<shared_ptr<tile>> tiles, shared_ptr<objectHitbox> foot) {
		teleportAnim->thisFrame();
		//if (sprite->getPosition().y < targetLoc->y) {

		if (looped && sprite->getCameraPosition().y > 300) {

			
			if (floorCheck(tiles, foot)) {

				if (hitFloor == false) {
					teleSound->play();
				}

				hitFloor = true;
			}
			
		}
		else {
			looped = true;
		}
		if(!hitFloor){
			sprite->move(90, deltaT, speed);
		}
		else {
			timer->run(deltaT);
			if (timer->isFinished(deltaT)) {


				return true;
			}
		}
		return false;
	}

	void forceEnd(Vector2f pos) {
		sprite->setPosition(pos);
		teleSound->play();
		hitFloor = true;
	}

	bool floorCheck(list<shared_ptr<tile>> tiles, shared_ptr<objectHitbox> foot) {

		float currentX = sprite->getSprite()->getPosition().x;
		for (shared_ptr<tile> t : tiles) {

			if (t->getGround() != NULL) {
				if (hitboxDetect::hitboxDetection(foot, t->getGround())) {
					
					sprite->setPosition(Vector2f(sprite->getPosition().x, t->getSprite()->getPosition().y - (24 * 4)));

					return true;

						
					
				}
			}

		}

		return false;
	}



};