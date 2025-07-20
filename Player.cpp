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

	Weapon* active = megaBuster;

	string palette;

	DeathAnim* deathAnim;
	DeathAnim* deathAnim1;
	DeathAnim* deathAnim2;

	float deathTime = 6;
	float deathTime_left = deathTime;

	

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
		foot = new objectHitbox(IntRect(Vector2i(9 * sprite->getScale().x, 7 * sprite->getScale().y), Vector2i(4, 20)), true, sprite);
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
	}

	void ladderJumpExtend(list<tile*> tiles) {
		if ((controls->isJumping() && ladderNotBelow(tiles) )|| (controls->getOnLadder() && ladderNotBelow(tiles))) {
			ladderHit->setRelativePosition(Vector2f(ladderHit->getRelativePosition().x, 0 * sprite->getScale().y));
			ladderAbove->setRelativePosition(Vector2f(ladderHit->getRelativePosition().x, -3 * sprite->getScale().y));
			ladderBelow->setRelativePosition(Vector2f(ladderHit->getRelativePosition().x, 2 * sprite->getScale().y));
		 }
		else {
			ladderHit->setRelativePosition(Vector2f(ladderHit->getRelativePosition().x, 24 * sprite->getScale().y));
			ladderAbove->setRelativePosition(Vector2f(ladderHit->getRelativePosition().x, 21 * sprite->getScale().y));
			ladderBelow->setRelativePosition(Vector2f(ladderHit->getRelativePosition().x, 26 * sprite->getScale().y));
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
	void setETanks(int e) {
		ETanks = e;
	}

	int getMaxHP() {
		return maxHP;
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

	void setLives(int l) {
		lives = l;
	}

	bool hasAtomicFire() {
		return gotAtomicFire;
	}

	bool getGrounded() {
		return grounded;
	}

	bool setDead() {
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


	void eachFrame(float* deltaT, list<tile*> tiles) {
		if (deathAnim == NULL) {
			alive(deltaT, tiles);
			ladderJumpExtend(tiles);
		}
		else {
			dead(deltaT);
		}
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
			deathAnim = NULL;
			deathAnim1 = NULL;
			deathAnim2 = NULL;
			return true;
		}
		return false;

	}

	void alive(float* deltaT, list<tile*> tiles) {
		ammoBar->update(active->getAmmo());

		if (!damage) {

			if (tele == NULL) {
				controls->checkControls(deltaT);
				controls->shootEachFrame(deltaT);

				pAnim->shootDecide(deltaT);


				sprite->eachFrame(deltaT);
				

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
			if (tele == NULL) {
				dam->flicker(deltaT);
				takingDamage(deltaT);
			}
			else {
				sprite->setMovable(true);
				tempDTime = 0;
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

		updateHitbox();


		//checkHold();


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

	teleport* getTeleport() {
		return tele;
	}

	bool getDamage() {
		return damage;
	}

	int getHP() {
		return health->getAmount();
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
		if (deathAnim == NULL) {
			return list<objectSprite*> {sprite};
		}
		else {
			list<objectSprite*> temp = {deathAnim->getSprite()};
			if (deathAnim1 != NULL) {
				temp.push_back(deathAnim1->getSprite());
			}
			if (deathAnim2 != NULL) {
				temp.push_back(deathAnim2->getSprite());
			}
			return temp;
		}
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
		health->increaseAmount(-dam);
		sprite->setMovable(false);
		damage = true;
		pAnim->hurtAnim();
		sprite->setHVelocity(0);
		sprite->setVVelocity(0);
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
	void takingDamage(float* deltaT) {
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

	
};