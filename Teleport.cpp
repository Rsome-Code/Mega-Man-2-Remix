#include "movable object.cpp"
#include "animate.cpp"
#include "Animation Timer.cpp"
#include "tile.cpp"
#include "hitbox detector.cpp"

#pragma once
class teleport {
	movable* sprite;
	float* speed = new float(2000);
	float startX;
	Vector2f* startLoc;
	bool* initial;
	animation* teleportAnim;
	animTimer* timer;


	bool hitFloor = false;

	bool looped = false;



	

public:
	teleport(movable* sprite, float startX, float startY){
		this->sprite = sprite;
		this->startX = startX;
		initial = new bool(true);
		startLoc = new Vector2f(startX, startY);
		this->sprite->setPosition(*startLoc);

		teleportAnim = new animation(list<IntRect> {IntRect(Vector2i(124, 128), Vector2i(7, 24)), IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(163, 136), Vector2i(22, 15)), IntRect(Vector2i(134, 132), Vector2i(22, 19))}, sprite);
		
		teleportAnim->setOffsetList(list<Vector2f> {Vector2f(sprite->getScale().x * 5, sprite->getScale().y * 2), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 11), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7)});

		teleportAnim->thisFrame();
		timer = new animTimer(teleportAnim, 12, false);


	}

	teleport(movable* sprite, float startX) {
		this->sprite = sprite;

		this->startX = startX;
		initial = new bool (false);
		teleportAnim = new animation(list<IntRect> {IntRect(Vector2i(124, 128), Vector2i(7, 24)), IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(163, 136), Vector2i(22, 15)), IntRect(Vector2i(134, 132), Vector2i(22, 19))}, sprite);

		teleportAnim->setOffsetList(list<Vector2f> {Vector2f(sprite->getScale().x * 5, sprite->getScale().y * 2), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 11), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7)});

		teleportAnim->thisFrame();
		timer = new animTimer(teleportAnim, 12, false);

	}

	bool eachFrame(float* deltaT, list<tile*> tiles, objectHitbox* foot) {
		//if (sprite->getPosition().y < targetLoc->y) {

		if (looped) {

			if (floorCheck(tiles, foot)) {
				hitFloor = true;
			}
			
		}
		else {
			looped = true;
		}
		if(!hitFloor){
			sprite->move(90, deltaT, *speed);
		}
		else {
			timer->run(deltaT);
			if (timer->isFinished(deltaT)) {
				delete timer;
				delete teleportAnim;
				delete initial;
				delete startLoc;
				
				delete speed;
				return true;
			}
		}
		return false;
	}

	void forceEnd(Vector2f pos) {
		sprite->setPosition(pos);
		hitFloor = true;
	}

	bool floorCheck(list<tile*> tiles, objectHitbox* foot) {

		float currentX = sprite->getSprite()->getPosition().x;
		for (tile* t : tiles) {

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