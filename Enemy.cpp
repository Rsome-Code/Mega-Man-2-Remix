#include "object.cpp"
#include "object hitbox.cpp"
#include "Hitbox Detector.cpp"
#include "animate.cpp"
#include "animation Timer.cpp"
#include "movable object.cpp"
#pragma once
class enemy:public object{
protected:

	objectHitbox* hit;
	objectHitbox* hurt;
	int hp;
	int damage;
	animation* deathAnim;
	animTimer* deathTimer;
	bool offScreen = false;
	Vector2f initialPos;
	bool spawnDisplay = true;


public:
	enemy(Texture* t, Vector2f i) {

		sprite = new movable("enemy", t, Vector2i(3, 117), Vector2i(16, 23), Vector2f(1200, 1200), Vector2f(4, 4), 1);
		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite);
		
		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite);


		deathTimer = new animTimer(deathAnim, 16, false);
		initialPos = i;
		act = false;
		display = false;

		

	}

public:



	void offSetList() {
		Vector2f center = sprite->getRelativeCenter();
		list<Vector2f> temp;
		temp.push_back(Vector2f(center.x - 48, center.y - 48));
		temp.push_back(Vector2f(center.x - 32, center.y - 32));
		temp.push_back(Vector2f(center.x - 24, center.y - 24));
		temp.push_back(Vector2f(center.x - 20, center.y - 20));
		temp.push_back(Vector2f(center.x - 8, center.y - 8));

		
		deathAnim->setOffsetList(temp);
	}

	Vector2f getInitialPosition() {
		return initialPos;
	}

	virtual void initial() {};

	bool getSpawnDisplay() {
		return spawnDisplay;
	}

	int getHP() {
		return hp;
	}
	int getDamage() {
		return damage;
	}
	objectHitbox* getHitbox() {
		return hit;
	}
	objectHitbox* getHurtbox() {
		return hurt;
	}

	bool checkHit(objectHitbox* pHit) {
		return hitboxDetect::hitboxDetection(pHit, hit);
	}
	bool checkHurt(objectHitbox* bullet) {
		return hitboxDetect::hitboxDetection(bullet, hurt);
	}

	virtual void eachFrame(float* deltaT, objectSprite* player) {
		if (hp > 0) {
			hit->updatePos();
			hurt->updatePos(); 
			alive(player, deltaT); }
		else { death(deltaT); }
	}

	void lowerHP(int h) {
		hp = hp - h;
	}

	virtual void alive(objectSprite* player, float* deltaT) = 0;


	void death(float* deltaT) {
		hit->setPosition(Vector2f(0, 0));
		hurt->setPosition(Vector2f(0, 0));
		deathAnim->thisFrame();
		if (!deathTimer->isFinished()) {
			deathTimer->run(deltaT);
		}
		else{
			act = false;
			display = false;
		}
	}

	void reset() {
		deathAnim->reset();
	}

	virtual int busterDam() {
		return 1;
	}

	void setOffScreen(bool b) {
		offScreen = b;
	}
	
	bool getOffScreen() {
		return offScreen;
	}
};