#include "movable object.cpp"
#include "animate.cpp"
#include "Animation Timer.cpp"
#pragma once
class teleport {
	movable* sprite;
	float* speed = new float(2000);
	Vector2f* targetLoc;
	Vector2f* startLoc;
	bool* initial;
	animation* teleportAnim;
	animTimer* timer;

	

public:
	teleport(movable* sprite, Vector2f target, float startY){
		this->sprite = sprite;
		targetLoc = new Vector2f(target);
		initial = new bool(true);
		startLoc = new Vector2f(targetLoc->x, startY);
		//this->sprite->setPosition(startLoc);

		teleportAnim = new animation(list<IntRect> {IntRect(Vector2i(124, 128), Vector2i(7, 24)), IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(163, 136), Vector2i(22, 15)), IntRect(Vector2i(134, 132), Vector2i(22, 19))}, sprite);
		
		teleportAnim->setOffsetList(list<Vector2f> {Vector2f(sprite->getScale().x * 5, sprite->getScale().y * 2), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 11), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7)});

		teleportAnim->thisFrame();
		timer = new animTimer(teleportAnim, 12, false);
	}

	teleport(movable* sprite, Vector2f target) {
		this->sprite = sprite;
		targetLoc = new Vector2f (target);
		initial = new bool (false);
		teleportAnim = new animation(list<IntRect> {IntRect(Vector2i(124, 128), Vector2i(7, 24)), IntRect(Vector2i(134, 132), Vector2i(22, 19)), IntRect(Vector2i(163, 136), Vector2i(22, 15)), IntRect(Vector2i(134, 132), Vector2i(22, 19))}, sprite);

		teleportAnim->setOffsetList(list<Vector2f> {Vector2f(sprite->getScale().x * 5, sprite->getScale().y * 2), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 11), Vector2f(sprite->getScale().x * 0, sprite->getScale().y * 7)});

		teleportAnim->thisFrame();
		timer = new animTimer(teleportAnim, 12, false);
	}

	bool eachFrame(float* deltaT) {
		if (sprite->getPosition().y < targetLoc->y) {
			sprite->move(90, deltaT, *speed);
		}
		else {
			timer->run(deltaT);
			if (timer->isFinished(deltaT)) {
				delete timer;
				delete teleportAnim;
				delete initial;
				delete startLoc;
				delete targetLoc;
				delete speed;
				return true;
			}
		}
		return false;
	}

};