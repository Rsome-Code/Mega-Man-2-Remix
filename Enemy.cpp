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

	shared_ptr<objectHitbox> hit;
	shared_ptr<objectHitbox> hurt;
	int hp;
	int damage;
	shared_ptr<animation> deathAnim;
	shared_ptr<animTimer> deathTimer;
	bool offScreen = true;
	bool initOffScreen = true;
	Vector2f initialPos;
	bool spawnDisplay = true;

	bool dead = false;

	bool faceRight = false;

	shared_ptr<SoundBuffer> hitB;
	shared_ptr<Sound> hitSound;

	bool damaged = false;
	float flashTime = 0.04;
	float flashTime_left = flashTime;

	bool frozen = false;

	shared_ptr<movable> mov;

public:

	virtual ~enemy() {

		//delete mov;
		//delete sprite;

	}
public:
	enemy(){}
	enemy(shared_ptr<Texture> t, Vector2f i) {

		mov = shared_ptr<movable>(new movable(t));
		sprite = mov;
		//deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite));
		
		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite));

		offSetList();

		deathTimer = shared_ptr<animTimer> (new animTimer(deathAnim, 16, false));
		initialPos = i;
		act = false;
		display = false;
		
		mov->setScale(Vector2f(4, 4));

		setCode();

	}

	enemy(Vector2f i) {

		mov = shared_ptr<movable>(new movable());
		sprite = mov;
		//deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(926, 79), Vector2i(4, 4)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(848, 69), Vector2i(24, 24))}, sprite));

		deathAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(848, 69), Vector2i(24, 24)), IntRect(Vector2i(873, 73), Vector2i(16, 16)), IntRect(Vector2i(892, 75), Vector2i(12, 12)), IntRect(Vector2i(910, 76), Vector2i(10, 10)), IntRect(Vector2i(926, 79), Vector2i(4, 4))}, sprite));

		offSetList();

		deathTimer = shared_ptr<animTimer> (new animTimer(deathAnim, 16, false));
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

	virtual void loadSound(shared_ptr<SoundCollection> soundCol) {}

	virtual void playerHit(){}

	void setHitB(shared_ptr<SoundBuffer> sB) {
		hitB = sB;
		hitSound = shared_ptr<Sound> (new Sound());
		hitSound->setBuffer(*hitB);
	}
	void setHitSound(shared_ptr<Sound> s) {
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
	shared_ptr<objectHitbox> getHitbox() {
		return hit;
	}
	shared_ptr<objectHitbox> getHurtbox() {
		return hurt;
	}

	void setHP(int h) {
		hp = h;
	}

	void updateHitbox() {
		hit->updatePos();
		hurt->updatePos();
	}

	bool checkHit(shared_ptr<objectHitbox> pHit) {
		return hitboxDetect::hitboxDetection(pHit, hit);
	}
	bool checkHurt(shared_ptr<objectHitbox> bullet) {
		return hitboxDetect::hitboxDetection(bullet, hurt);
	}

	void setFrozen(bool b) {
		frozen = b;
	}

	virtual bool eachFrame(float* deltaT, shared_ptr<player> p, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* enemyList, list<shared_ptr<EnemyBullet>>* bList, list<shared_ptr<GameObject>>* objectList, shared_ptr<SoundCollection> soundCol, shared_ptr<camera> cam) {
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
				alive(p, deltaT, tileList, enemyList, objectList, bList, soundCol);
				alive(p, deltaT, tileList, enemyList, objectList, bList, soundCol, cam);
			}
		}
		else {
			uniqueDeathLoop(deltaT);
			uniqueDeathLoop(deltaT, p);
			
			return death(deltaT, enemyList);
		}
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

	virtual shared_ptr<objectSprite> getDamSprite() { return NULL; };

	virtual bool checkInvincible() {
		return false;
	}

	virtual bool freezeDam(shared_ptr<player> p) {
		return false;
	}

	virtual void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList) {};

	virtual void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {};
	virtual void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<GameObject>>* obList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {};
	virtual void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<GameObject>>* obList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol, shared_ptr<camera> cam) {};

	void checkDirection(shared_ptr<objectSprite> player) {
		if (player->getPosition().x > sprite->getPosition().x) {
			setFacing(true);
		}
		else {
			setFacing(false);
		}
	}

	virtual void setFacing(bool right) {};

	virtual bool death(float* deltaT, list<shared_ptr<enemy>>* tempEList) {
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

	virtual bool isDead(list<shared_ptr<enemy>>* tempEList) {
		act = false;
		display = false;
		sprite->setPosition(Vector2f(-1100, -1000));

		uniqueDeath();

		return false;
	}

	virtual void uniqueDeath() {};

	virtual void uniqueDeathLoop(float* deltaT) {};

	virtual void uniqueDeathLoop(float* deltaT, shared_ptr<player> p) {};

	virtual bool yDeath(list<shared_ptr<enemy>>* enemies) {
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

	virtual void spawnItem(list<shared_ptr<Item>>* obList, shared_ptr<Texture> t, Vector2f pos, shared_ptr<SoundCollection> soundCol) {
		int itemChance = rand();
		itemChance = itemChance % 100;

		shared_ptr<Item> item = NULL;

		if (itemChance > 25 && itemChance <= 50) {
			item = shared_ptr<SmallAmmo> (new SmallAmmo(t, pos));
		}
		else if (itemChance > 50 && itemChance <= 75) {
			item = shared_ptr<SmallHealth> (new SmallHealth(t, pos));
		}
		else if (itemChance > 75 && itemChance <= 85) {
			item = shared_ptr<BigHealth> (new BigHealth(t, pos));
		}
		else if (itemChance > 85 && itemChance <= 95) {
			item = shared_ptr<BigAmmo> (new BigAmmo(t, pos));
		}
		else if (itemChance > 95) {
			item = shared_ptr<ExtraLife> (new ExtraLife(t, pos));
			item->setSoundPointer(soundCol->getLifeGet());
		}

		//item = shared_ptr<BigAmmo> (new BigAmmo(t, pos);

		if (item != NULL) {
			item->getSprite()->setPosition(Vector2f(item->getSprite()->getPosition().x + (item->getSprite()->getSize().x / 4), item->getSprite()->getPosition().y + (item->getSprite()->getSize().y / 4)));
			obList->push_back(item);
		}
	}

	bool groundCheck(shared_ptr<tile> t) {
		if (hitboxDetect::hitboxDetection(t->getGround(), hit)) {
			return true;
		}
		return false;
	}


	virtual shared_ptr<AmmoBar>* getBar() {
		return NULL;
	}

	virtual vector<shared_ptr<DeathAnim>*> getDeathAnims() {
		return vector<shared_ptr<DeathAnim>*>{NULL, NULL, NULL};
	}

	virtual void spawnEnemy(list<shared_ptr<enemy>>* enemies, shared_ptr<SoundCollection> soundCol) {

	}

	virtual void spawnObject(list<shared_ptr<GameObject>>* objects, shared_ptr<SoundCollection> soundCol) {

	}

	shared_ptr<objectHitbox> getGround() {
		return NULL;
	}

	virtual list<shared_ptr<objectSprite>> getExtraSprites() {
		return list<shared_ptr<objectSprite>>{};
	}
};