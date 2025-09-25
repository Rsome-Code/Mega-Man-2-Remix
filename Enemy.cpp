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
#include "tile.cpp"
#include "player.cpp"
#include "enemy Bullet.cpp"


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

	bool dead = false;

	bool faceRight = false;




public:
	enemy(){}
	enemy(Texture* t, Vector2f i) {

		sprite = new movable(t);
		//deathAnim = new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite);
		
		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite);

		//offSetList();

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
		list<Vector2f> temp = list<Vector2f>{ Vector2f(0, 0), Vector2f(4 * 4, 4 * 4), Vector2f(6 * 4, 6 * 4), Vector2f(7 * 4, 7 * 4), Vector2f(10 * 4, 10 * 4) };
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

	void updateHitbox() {
		hit->updatePos();
		hurt->updatePos();
	}

	bool checkHit(objectHitbox* pHit) {
		return hitboxDetect::hitboxDetection(pHit, hit);
	}
	bool checkHurt(objectHitbox* bullet) {
		return hitboxDetect::hitboxDetection(bullet, hurt);
	}

	virtual bool eachFrame(float* deltaT, player* p, list<tile*>* tileList, list<enemy*>* enemyList, list<EnemyBullet*>* bList) {
		if (hp > 0) {
			if (act) {
				hit->updatePos();
				hurt->updatePos();
				alive(p, deltaT, tileList, enemyList, bList);
			}
		}
		else { return death(deltaT, enemyList); }
		return false;
	}

	virtual bool getIntroDone() {
		return true;
	}

	void lowerHP(int h) {
		hp = hp - h;
	}

	virtual void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {};

	void checkDirection(objectSprite* player) {
		if (player->getPosition().x > sprite->getPosition().x) {
			setFacing(true);
		}
		else {
			setFacing(false);
		}
	}

	virtual void setFacing(bool right) {};

	virtual bool death(float* deltaT, list<enemy*>* tempEList) {
		if (!dead) {
			sprite->setPosition(Vector2f(sprite->getMiddlePos().x - (12*4), sprite->getMiddlePos().y - (12 * 4)));
			dead = true;
			deathAnim->reset();
		}
		hit->setPosition(Vector2f(-1000, 0));
		hurt->setPosition(Vector2f(-1000, 0));
		
		deathAnim->thisFrame();
		
		if (!deathTimer->isFinished()) {
			deathTimer->run(deltaT);
		}
		else{
			return isDead(tempEList);
			
		}
		return false;
	}

	virtual bool isDead(list<enemy*>* tempEList) {
		act = false;
		display = false;
		sprite->setPosition(Vector2f(-1100, -1000));
		return false;
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

	virtual void spawnItem(list<Item*>* obList, Texture* t, Vector2f pos) {
		int itemChance = rand();
		itemChance = itemChance % 100;

		Item* item = NULL;

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
			item->getSprite()->setPosition(Vector2f(item->getSprite()->getPosition().x + (item->getSprite()->getSize().x / 4), item->getSprite()->getPosition().y + (item->getSprite()->getSize().y / 4)));
			obList->push_back(item);
		}
	}

	bool groundCheck(tile* t) {
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			return true;
		}
	}


	virtual AmmoBar** getBar() {
		return NULL;
	}

	virtual vector<DeathAnim**> getDeathAnims() {
		return vector<DeathAnim**>{NULL, NULL, NULL};
	}

};