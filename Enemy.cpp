#include "object.cpp"
#include "object hitbox.cpp"
#include "Hitbox Detector.cpp"
#include "animate.cpp"
#include "animation Timer.cpp"
#include "movable object.cpp"
#include "big health.cpp"
#include "small health.cpp"
#include "big ammo.cpp"
#include "small ammo.cpp"

#pragma once
class enemy:public object{
protected:

	objectHitbox* hit;
	objectHitbox* hurt;
	int hp;
	int damage;
	animation* deathAnim;
	animTimer* deathTimer;
	bool offScreen = true;
	bool initOffScreen = true;
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

	bool getInitOffScreen() {
		return initOffScreen;
	}
	void setInitOffScreen(bool o) {
		initOffScreen = o;
	}

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
			//sprite->setPosition(Vector2f(0, 0));
		}
	}

	void reset() {
		deathAnim->reset();
	}

	virtual int busterDam() {
		return genericDam();
	}

	virtual int atomicDam() {
		return genericDam();
	}

	

	virtual int genericDam() { return 1; }

	void setOffScreen(bool b) {
		offScreen = b;
	}
	
	bool getOffScreen() {
		return offScreen;
	}

	virtual void spawnItem(list<object*>* obList, Texture* t, Vector2f pos) {
		int itemChance = rand();
		itemChance = itemChance % 100;

		object* item = NULL;

		if (itemChance > 25 && itemChance <= 50) {
			item = new SmallAmmo(t, pos);
		}
		else if (itemChance > 50 && itemChance <= 75) {
			item = new SmallHealth(t, pos);
		}
		else if (itemChance > 75 && itemChance <= 85) {
			item = new BigHealth(t, pos);
		}
		else if (itemChance > 85 && itemChance <= 95) {
			item = new BigAmmo(t, pos);
		}
		else if (itemChance > 95) {
			
		}

		if (item != NULL) {
			cout << item->getSprite()->getSize().x;
			item->getSprite()->setPosition(Vector2f(item->getSprite()->getPosition().x - (item->getSprite()->getSize().x / 6), item->getSprite()->getPosition().y - (item->getSprite()->getSize().y / 2)));
			obList->push_back(item);
		}
	}

};