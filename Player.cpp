#include "Physics Object.cpp"
#include "Player Controls.cpp"
#include "Object Hitbox.cpp"
#include "animate.cpp"
#include "animation Timer.cpp"
#include "Player Animations.cpp"
#include "teleport.cpp"
#include "bullet.cpp";
#include "mega buster.cpp"
#include "Energy Bar.cpp"
#include "Damage Effect.cpp"
#include "ammo bar.cpp"
#include "Death Animation.cpp"
#include "hitbox detector.cpp"
#include "leaf Shield.cpp"
#include "bubble lead.cpp"
#include <SFML/audio.hpp>
#include "splash effect.cpp"
#include "item1.cpp"
#include "metal blade.cpp"
#include "quick boomerang.cpp"
#pragma once

class player {
	int lives = 2;
	int ETanks = 0;
	physicsObject* sprite;
	Texture* texture;
	pControls* controls;
	bool grounded = true;
	teleport* tele;

	int maxHP = 28;

	objectHitbox* hit;
	objectHitbox* foot;
	objectHitbox* head;
	objectHitbox* ladderHit;
	objectHitbox* ladderBelow;
	objectHitbox* ladderAbove;
	objectHitbox* belowBox;

	bool gotAtomicFire = false;
	bool gotShield = false;
	bool gotLead = false;
	bool gotBlade = false;
	bool gotBoomerang = false;

	int holdAdd = 0;
	float holdTime = 0.2;
	int cycles = 0;

	playerAnimation* pAnim;

	energyBar* health;
	AmmoBar* ammoBar;
	Vector2f ammoPos = Vector2f(20 + (2*(8*4)), 20);
	bool displayAmmo = false;

	bool damage = false;
	float damageTime = 0.5;
	float tempDTime = 0.5;

	damageEffect* dam;

	MegaBuster* megaBuster;
	AtomicFire* atomicFire;
	LeafShield* leafShield;
	BubbleLead* bubbleLead;
	MetalBlade* metalBlade;
	QuickBoomerang* quickBoomerang;
	Item1* item1;

	Weapon* active = megaBuster;

	string palette;

	DeathAnim* deathAnim;
	DeathAnim* deathAnim1;
	DeathAnim* deathAnim2;

	float deathTime = 6;
	float deathTime_left = deathTime;

	bool inControl = true;

	float invincibilityTime = 2;
	float invincibilityTime_left = 0;

	float flashTime = 0.01666666;
	float flashTime_left = flashTime;
	bool display = true;

	SoundBuffer* damageB;
	Sound* damageSound;

	Splash* splash;
	bool inWater = false;

public:
	player(pController* p1) {
		
		texture = new Texture();
		if (!texture->loadFromFile("Assets\\player\\NES - Mega Man 2 - Mega Man.png")) {
			cout << "error";
		}
		Texture* t = new Texture();
		t->loadFromFile("assets\\weapons.png");

		Image im = texture->copyToImage();
		Image* image = &im;
		sprite = new physicsObject("player", texture, im, IntRect(2, 21, 22, 24), Vector2f(1000, 2000), Vector2f(4, 4), 1, 200);

		sprite->setFullColour(&Color::Red);

		pAnim = new playerAnimation(sprite);
		controls = new pControls(p1, sprite, pAnim);
		megaBuster = new MegaBuster(sprite, t);
		setActiveWeapon(megaBuster);

		

		hit = new objectHitbox(IntRect(Vector2i(5 * sprite->getScale().x, 2 * sprite->getScale().y), Vector2i(11, 22)), true, sprite);
		foot = new objectHitbox(IntRect(Vector2i(7 * sprite->getScale().x, 7 * sprite->getScale().y), Vector2i(7, 20)), true, sprite);
		head = new objectHitbox(IntRect(Vector2i(7 * sprite->getScale().x, 1 * sprite->getScale().y), Vector2i(7, 2)), true, sprite);
		ladderHit = new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 24 * sprite->getScale().y), Vector2i(10, 2)), true, sprite);
		ladderBelow = new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 26 * sprite->getScale().y), Vector2i(10, 2)), true, sprite);
		ladderAbove = new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 21 * sprite->getScale().y), Vector2i(10, 2)), true, sprite);
		belowBox = new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 26 * sprite->getScale().y), Vector2i(10, 20)), true, sprite);

		Texture* hbT = new Texture();
		hbT->loadFromFile("Assets\\Health.png");
		health = new energyBar(hbT, Vector2f(20,20));

		dam = new damageEffect(sprite);



		megaBuster = new MegaBuster(sprite, t);
		atomicFire = new AtomicFire(sprite, t);
		leafShield = new LeafShield(sprite, t);
		bubbleLead = new BubbleLead(sprite, t);
		metalBlade = new MetalBlade(sprite, t);
		quickBoomerang = new QuickBoomerang(sprite, t);
		item1 = new Item1(sprite, t);

		//Define ammo bars here
		Texture* aB = new Texture();
		aB->loadFromFile("assets\\bars\\atomic fire.png");
		ammoBar = new AmmoBar(aB, ammoPos);
		ammoBar->setVertical();

		//health->increaseAmount(-27);

		palette = active->getName();
		deathAnim = NULL;
		deathAnim1 = NULL;
		deathAnim2 = NULL;

		damageB = new SoundBuffer();
		damageB->loadFromFile("assets\\sound\\hit.wav");
		damageSound = new Sound();
		damageSound->setBuffer(*damageB);

		
	}

	void resetGravity() {
		if (inWater) {
			splash->start(sprite);
			inWater = false;
		}
		sprite->setGravity(-3000);
	}
	void setGravity(float grav) {
		sprite->setGravity(grav);
	}
	void setWaterGravity() {
		if (!inWater) {
			splash->start(sprite);
			inWater = true;
		}
		sprite->setGravity(-800);
	}

	void shootReset() {
		float* t = new float(0.000001);
		megaBuster->eachFrame(t);
		atomicFire->eachFrame(t);
		bubbleLead->eachFrame(t);
		metalBlade->eachFrame(t);
		quickBoomerang->eachFrame(t);
	}

	Vector2f getPosition() {
		return sprite->getPosition();
	}

	void ladderJumpExtend(list<tile*> tiles) {
		if ((controls->isJumping() && ladderNotBelow(tiles) )|| (controls->getOnLadder() && ladderNotBelow(tiles))) {
			ladderHit->setRelativePosition(Vector2i(ladderHit->getRelativePosition().x, 0 * sprite->getScale().y));
			ladderAbove->setRelativePosition(Vector2i(ladderHit->getRelativePosition().x, -3 * sprite->getScale().y));
			ladderBelow->setRelativePosition(Vector2i(ladderHit->getRelativePosition().x, 2 * sprite->getScale().y));
		 }
		else {
			ladderHit->setRelativePosition(Vector2i(ladderHit->getRelativePosition().x, 24 * sprite->getScale().y));
			ladderAbove->setRelativePosition(Vector2i(ladderHit->getRelativePosition().x, 21 * sprite->getScale().y));
			ladderBelow->setRelativePosition(Vector2i(ladderHit->getRelativePosition().x, 26 * sprite->getScale().y));
		}
	}

	objectHitbox* getBelowBox() {
		return belowBox;
	}

	bool ladderNotBelow(list<tile*> tiles) {
		belowBox->updatePos();
		for (tile* t : tiles) {
			if (t->getLadder() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getLadder(), belowBox)) {
					return false;
				}
			}
			if (t->getGround() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getGround(), belowBox)) {
					return false;
				}
			}
		}
		return true;
	}

	void flash(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			flashTime_left = flashTime;
			display = !display;
		}
	}

	bool getDisplay() {
		return display;
	}

	objectHitbox* getFoot() {
		return foot;
	}

	int getETanks() {
		return ETanks;
	}

	pController* getController() {
		return controls->getController();
	}

	void setAtomicFire(bool b) {
		gotAtomicFire = b;
	}

	void setBubbleLead(bool b) {
		gotLead = b;
	}
	void setMetalBlade(bool b) {
		gotBlade = b;
	}
	void setBoomerang(bool b) {
		gotBoomerang = b;
	}

	void setETanks(int e) {
		ETanks = e;
	}

	int getMaxHP() {
		return maxHP;
	}

	void HPReset() {
		health->reset();
	}

	void setActiveWeapon(Weapon* w) {
		active = w;
		controls->setWeapon(active);
		loadPallete();
		if (active->getName() == "Mega Buster") {
			displayAmmo = false;
		}
		else {
			displayAmmo = true;
			Texture* temp = new Texture();
			temp->loadFromFile("assets\\bars\\" + active->getName() + ".png");
			ammoBar = new AmmoBar(temp, ammoPos);
			ammoBar->update(active->getAmmo());
			ammoBar->setVertical();
		}

		palette = active->getName();

		controls->setThrow(active->checkThrow());
		pAnim->setThrow(active->checkThrow());
	}

	string getPalette() {
		return palette;
	}

	MegaBuster* getMegaBuster() {
		return megaBuster;
	}
	AtomicFire* getAtomicFire() {
		return atomicFire;
	}
	Item1* getItem1() {
		return item1;
	}
	BubbleLead* getBubbleLead() {
		return bubbleLead;
	}
	MetalBlade* getMetalBlade() {
		return metalBlade;
	}
	QuickBoomerang* getBoomerang() {
		return quickBoomerang;
	}

	void setLives(int l) {
		lives = l;
	}

	Weapon* getShield() {
		gotShield = true;
		return leafShield;
	}

	bool hasAtomicFire() {
		return gotAtomicFire;
	}

	bool getGrounded() {
		return grounded;
	}

	bool isInvincible() {
		if (invincibilityTime_left > 0) {
			return true;
		}
		return false;
	}

	void iniSplash(Texture* miscT) {
		splash = new Splash(miscT);
	}

	bool setDead() {
		
		invincibilityTime_left = 0;
		display = true;
		if (deathAnim == NULL) {
			deathAnim = new DeathAnim(sprite, palette);
			return true;
		}
		else if (deathTime_left <= deathTime - 0.75 && deathAnim1 == NULL) {
			deathAnim1 = new DeathAnim(sprite, palette);
		}
		else if (deathTime_left <= deathTime - 1.5 && deathAnim2 == NULL) {
			deathAnim2 = new DeathAnim(sprite, palette);
		}
		return false;
	}
	void setNotDead() {
		deathAnim = NULL;
		deathAnim1 = NULL;
		deathAnim2 = NULL;
	}

	void setDeathNull() {
		deathAnim = NULL;
		deathAnim1 = NULL;
		deathAnim2 = NULL;
	}

	objectHitbox* getBelow() {
		return ladderBelow;
	}
	objectHitbox* getAbove() {
		return ladderAbove;
	}

	void start(int startHeight) {
		tele = new teleport(sprite, startHeight);
		//setPosition(Vector2f(pos.x, pos.y));
	}

	Weapon* getActiveWeapon() {
		return active;
	}

	void setPosition(Vector2f pos) {
		sprite->setPosition(pos);
		hit->updatePos();
		ladderHit->updatePos();
		foot->updatePos();
		head->updatePos();
		ladderBelow->updatePos();
		ladderAbove->updatePos();
	}

	void updateAmmo() {
		ammoBar->update(active->getAmmo());
	}


	void eachFrame(float* deltaT, list<tile*> tiles, list<ItemBullet*>* IBullets) {

		splash->eachFrame(deltaT);
		
			if (deathAnim == NULL) {
				alive(deltaT, tiles, IBullets);
				ladderJumpExtend(tiles);
			}
			else {
				dead(deltaT);
			}
		
	}

	bool checkInControl() {
		return inControl;
	}

	void dead(float* deltaT) {
		deathAnim->run(deltaT);

		if (deathAnim1 != NULL) {
			deathAnim1->run(deltaT);
		}
		if (deathAnim2 != NULL) {
			deathAnim2->run(deltaT);
		}

		deathTime_left -= *deltaT;
		
	}

	bool checkDeathFinish() {
		if (deathTime_left <= 0) {
			deathTime_left = deathTime;

			return true;
		}
		return false;

	}



	void alive(float* deltaT, list<tile*> tiles, list<ItemBullet*>* IBullets) {
		ammoBar->update(active->getAmmo());

		if (!damage) {

			if (tele == NULL) {
				if (inControl) {
					controls->checkControls(deltaT, IBullets);
				}
				else {
					controls->runWithoutControl(deltaT);
				}
				controls->shootEachFrame(deltaT, tiles, *IBullets);

				pAnim->shootDecide(deltaT);

				if (!controls->getOnLadder()) {
					sprite->eachFrame(deltaT);
				}
				

				if (controls->checkTeleport()) {
					tele = new teleport(sprite, sprite->getPosition().x);
				}

			}
			else {
				if (tele->eachFrame(deltaT, tiles, foot)) {
					delete tele;
					tele = NULL;
					pAnim->resetIdle();
					pAnim->getIdle()->thisFrame();
				}
			}
		}
		else {

			controls->shootEachFrame(deltaT, tiles, *IBullets);

			if (tele == NULL) {
				dam->flicker(deltaT);
				takingDamage(deltaT, tiles);
			}
			else {
				sprite->setMovable(true);
				tempDTime = damageTime;
				damage = false;
				sprite->setMovable(true);
				dam->reset();
			}
			
		}

		if (sprite->getSpeed() > 3000) {

			//foot->setSize(Vector2i(4, 25));
		}
		else {

			//foot->setSize(Vector2i(4, 20));
		}

		
		if (inControl) {
			updateHitbox();
		}
		else {
			moveHitbox();
		}

		invincibilityTime_left -= *deltaT;
		if (invincibilityTime_left > 0) {
			flash(deltaT);
		}
		else {
			display = true;
		}


		//checkHold();


	}

	void enableControls(bool e) {
		inControl = e;
	}

	int beforeHold = 0;

	void swapDirection() {
		controls->swapDirection();
	}

	void checkHold() {
		int holdAdd = 0;
		

		holdAdd = controls->holdin();
		
		beforeHold = sprite->getRect().getPosition().y;

		sprite->setRect(IntRect(Vector2i(sprite->getRect().getPosition().x, sprite->getRect().getPosition().y + holdAdd), sprite->getRect().getSize()));
	}

	int getBeforeHold() {
		return beforeHold;
	}



	int getHoldAdd() {
		return holdAdd;
	}

	void updateLighting() {
		sprite->updateLighting();
	}

	list<RectangleShape*> getLightPixels() {
		return sprite->getPixels();
	}

	void lightingCheck(LightSource* light) {

		sprite->lightingCheck(light);

	}

	void tileMovement(float m, float* deltaT) {
		if (inControl) {
			sprite->move(0, deltaT, m);
		}
	}

	teleport* getTeleport() {
		return tele;
	}

	bool getDamage() {
		return damage;
	}

	int getHP() {
		return health->getAmount();
	}
	void setHP(int num) {
		health->setAmount(num);
	}

	void ammoReset() {
		atomicFire->setAmmo(28);
	}

	void updateHitbox() {
		hit->updatePos();
		ladderHit->updatePos();
		foot->updatePos();
		head->updatePos();
		ladderBelow->updatePos();
		ladderAbove->updatePos();
		belowBox->updatePos();
	}
	void moveHitbox() {
		hit->setPosition(Vector2f(-10000, -10000));
		ladderHit->updatePos();
		foot->updatePos();
		head->updatePos();
		ladderBelow->updatePos();
		ladderAbove->updatePos();
		belowBox->updatePos();
	}

	bool isTeleporting() {
		return tele != NULL;
	}

	objectHitbox* getHitbox() {
		return hit;
	}
	objectHitbox* getLadderHitbox() {
		return ladderHit;
	}

	objectHitbox* getHead() {
		return head;
	}

	physicsObject* getSprite() {
		return sprite;
		

	}

	list<objectSprite*> getSprites() {

		list<objectSprite*> temp;

		if (deathAnim == NULL) {
			temp = list<objectSprite*> {sprite};
		}
		else {
			temp = {deathAnim->getSprite()};
			if (deathAnim1 != NULL) {
				temp.push_back(deathAnim1->getSprite());
			}
			if (deathAnim2 != NULL) {
				temp.push_back(deathAnim2->getSprite());
			}
			
		}

		if (splash->getSprite() != NULL) {
			temp.push_back(splash->getSprite());
		}
		return temp;
	}

	object* getDamEffect() {
		if (deathAnim == NULL) {
			return dam;
		}
		return NULL;
		
	}

	list<UISprite*> getUI() {
		if (displayAmmo) {
			list<UISprite*> temp = ammoBar->getSprites();
			temp.push_back(health->getSprite());
			return temp;
		}
		else {
			return list<UISprite*> {health->getSprite()};
		}
	}

	list<objectSprite*> getBullets() {
		return controls->getBullets();
	}
	list<objectHitbox*> getBulletHitboxes() {
		return controls->getBulletHitboxes();
	}

	void setGrounded(bool b) {
		if (!controls->getOnLadder()) {
			grounded = b;
			this->controls->setGrounded(b);
		}
	}
	playerAnimation* getAnimation() {
		return pAnim;
	}
	pControls* getControls() {
		return controls;
	}

	void takeDamage(int dam) {
		if (!damage) {
			health->increaseAmount(-dam);
			sprite->setMovable(false);
			damage = true;
			pAnim->hurtAnim();
			sprite->setHVelocity(0);
			sprite->setVVelocity(0);
			invincibilityTime_left = invincibilityTime;
			damageSound->play();
		}
	}

	void heal(int h) {
		health->increaseAmount(h);
	}

	string getBulletType() {
		bullet* b = controls->getBulletObject();
		return b->getSprite()->getType();
	}


	void loadPallete() {
		Weapon* temp = getWeapon();
		this->texture = temp->getTexture();
		sprite->setTexture(texture);
	}

	Weapon* getWeapon() {
		return getControls()->getWeapon();
	}

	int getLives() {
		return lives;
	}



private:
	void takingDamage(float* deltaT, list<tile*> tileList) {
		tempDTime = tempDTime - *deltaT;
		if (tempDTime <= 0) {
			tempDTime = damageTime;
			damage = false;
			sprite->setMovable(true);
			dam->reset();
			
		}

		if (pAnim->getFacingRight()) {
			sprite->move(180, deltaT, 200);
		}
		else {
			sprite->move(0, deltaT, 200);
		}
		
	}

	void tileCollide(list<tile*> tiles) {
		for (tile* t : tiles) {
			if (t->getLeft() != NULL) {
				if (hitboxDetect::hitboxDetection(hit, t->getLeft())) {
					sprite->setPosition(Vector2f(t->getPosition().x - hit->getSize().x, sprite->getPosition().y));
				}
			}
			if (t->getRight() != NULL) {
				if (hitboxDetect::hitboxDetection(hit, t->getRight())) {
					sprite->setPosition(Vector2f(t->getPosition().x + t->getSprite()->getSize().x, sprite->getPosition().y));
				}
			}
		}
	}
	public:
	bool checkAtomicFire() {
		return gotAtomicFire;
	}
	bool checkShield() {
		return gotShield;
	}
	bool checkTornado() {
		return false;
	}
	bool checkBlade() {
		return gotBlade;
	}
	bool checkBomb() {
		return false;
	}
	bool checkStopper() {
		return false;
	}
	bool checkLead() {
		return gotLead;
	}
	bool checkBoomerang() {
		return gotBoomerang;
	}

	

	
};