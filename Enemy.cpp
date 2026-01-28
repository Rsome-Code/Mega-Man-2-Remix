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
#include "extra life.cpp"
#include "sound Collection.cpp"
#include <sfml/audio.hpp>

#pragma once
class enemy:public GameObject{
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

	SoundBuffer* hitB;
	Sound* hitSound;

	bool damaged = false;
	float flashTime = 0.04;
	float flashTime_left = flashTime;

	bool frozen = false;

	movable* mov;

public:
	enemy(){}
	enemy(Texture* t, Vector2f i) {

		mov = new movable(t);
		sprite = mov;
		//deathAnim = new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite);
		
		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite);

		offSetList();

		deathTimer = new animTimer(deathAnim, 16, false);
		initialPos = i;
		act = false;
		display = false;
		
		mov->setScale(Vector2f(4, 4));

		setCode();

	}

	enemy(Vector2f i) {

		mov = new movable();
		sprite = mov;
		//deathAnim = new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite);

		deathAnim = new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite);

		offSetList();

		deathTimer = new animTimer(deathAnim, 16, false);
		initialPos = i;
		act = false;
		display = false;

		mov->setScale(Vector2f(4, 4));
		setCode();
	}

public:

	virtual bool deleteOverY() {
		return true;
	}

	virtual bool deleteOverX() {
		return true;
	}

	virtual void loadSound(SoundCollection* soundCol) {}

	virtual void playerHit(){}

	void setHitB(SoundBuffer* sB) {
		hitB = sB;
		hitSound = new Sound();
		hitSound->setBuffer(*hitB);
	}
	void setHitSound(Sound* s) {
		hitSound = s;
	}

	virtual void stopMusic() {};

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

	void setHP(int h) {
		hp = h;
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

	void setFrozen(bool b) {
		frozen = b;
	}

	virtual bool eachFrame(float* deltaT, player* p, list<tile*>* tileList, list<enemy*>* enemyList, list<EnemyBullet*>* bList, SoundCollection* soundCol) {
		flashTime_left -= *deltaT;
		if (hp > 0) {
			if (damaged) {
				display = false;
				
				if (flashTime_left <= 0) {
					
					damaged = false;
					display = true;
				}
			}
			
			if (act && !frozen) {
				hit->updatePos();
				hurt->updatePos();
				alive(p, deltaT, tileList, enemyList, bList);
				alive(p, deltaT, tileList, enemyList, bList, soundCol);
			}
		}
		else { return death(deltaT, enemyList); }
		return false;
	}

	virtual bool getIntroDone() {
		return true;
	}

	virtual void onDamage() {};

	virtual void lowerHP(int h) {
		if (h > 0) {
			onDamage();
			hitSound->play();
			damaged = true;
			flashTime_left = flashTime;
		}
		hp = hp - h;
	}

	virtual objectSprite* getDamSprite() { return NULL; };

	virtual bool checkInvincible() {
		return false;
	}

	virtual bool freezeDam(player* p) {
		return false;
	}

	virtual void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList) {};

	virtual void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList, SoundCollection* soundCol) {};

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

	virtual bool yDeath(list<enemy*>* enemies) {
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

	virtual int metalDam() {
		return genericDam();
	}
	virtual int crashDam() {
		return genericDam();
	}

	virtual int leafDam() {
		return genericDam();
	}

	virtual int bubbleDam() {
		return genericDam();
	}
	virtual int quickDam() {
		return genericDam();
	}

	virtual int flashDam() {
		return genericDam();
	}

	virtual int airDam() {
		return genericDam();
	}

	virtual void forceDamSprite(){}
	virtual void forceDamSpriteOff() {}

	virtual int genericDam() { return 1; }

	void setOffScreen(bool b) {
		offScreen = b;
	}
	
	bool getOffScreen() {
		return offScreen;
	}

	virtual void spawnItem(list<Item*>* obList, Texture* t, Vector2f pos, SoundCollection* soundCol) {
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
			item = new ExtraLife(t, pos);
			item->setSoundPointer(soundCol->getLifeGet());
		}

		//item = new BigAmmo(t, pos);

		if (item != NULL) {
			item->getSprite()->setPosition(Vector2f(item->getSprite()->getPosition().x + (item->getSprite()->getSize().x / 4), item->getSprite()->getPosition().y + (item->getSprite()->getSize().y / 4)));
			obList->push_back(item);
		}
	}

	bool groundCheck(tile* t) {
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			return true;
		}
		return false;
	}


	virtual AmmoBar** getBar() {
		return NULL;
	}

	virtual vector<DeathAnim**> getDeathAnims() {
		return vector<DeathAnim**>{NULL, NULL, NULL};
	}

	virtual void spawnEnemy(list<enemy*>* enemies, SoundCollection* soundCol) {

	}

};