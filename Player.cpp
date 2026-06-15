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
#include "time stopper.cpp"
#include "crash bomb.cpp"
#include "air shooter.cpp"
#include "item2.cpp"
#include "item3.cpp"

#pragma once

class player {

	Vector2f centrePoint = Vector2f(0,0);
	Vector2f relativeCPoint = Vector2f(0, 0);

	int lives = 2;
	int ETanks = 0;
	shared_ptr<physicsObject> sprite;
	shared_ptr<Texture> texture;
	shared_ptr<pControls> controls;
	bool tempGround = false;
	bool grounded = true;
	bool groundedOverride = false;
	shared_ptr<teleport> tele;

	int maxHP = 28;

	shared_ptr<objectHitbox> hit;
	shared_ptr<objectHitbox> foot;
	shared_ptr<objectHitbox> head;
	shared_ptr<objectHitbox> ladderHit;
	shared_ptr<objectHitbox> ladderBelow;
	shared_ptr<objectHitbox> ladderAbove;
	shared_ptr<objectHitbox> belowBox;

	bool gotAtomicFire = false;
	bool gotShield = false;
	bool gotLead = false;
	bool gotBlade = false;
	bool gotBoomerang = false;
	bool gotTimeStopper = false;
	bool gotBomb = false;
	bool gotAir = false;

	int holdAdd = 0;
	float holdTime = 0.2;
	int cycles = 0;

	shared_ptr<playerAnimation> pAnim;

	shared_ptr<energyBar> health;
	shared_ptr<AmmoBar> ammoBar;
	Vector2f ammoPos = Vector2f(20 + (2*(8*4)), 20);
	bool displayAmmo = false;

	bool damage = false;
	float damageTime = 0.5;
	float tempDTime = 0.5;

	shared_ptr<damageEffect> dam;

	shared_ptr<MegaBuster> megaBuster;
	shared_ptr<AtomicFire> atomicFire;
	shared_ptr<LeafShield> leafShield;
	shared_ptr<BubbleLead> bubbleLead;
	shared_ptr<MetalBlade> metalBlade;
	shared_ptr<QuickBoomerang> quickBoomerang;
	shared_ptr<TimeStopper> timeStopper;
	shared_ptr<CrashBomb> crashBomb;
	shared_ptr<AirShooter> airShooter;
	shared_ptr<Item1> item1;
	shared_ptr<Item2> item2;
	shared_ptr<Item3> item3;

	shared_ptr<Weapon> active = megaBuster;

	string palette;

	shared_ptr<DeathAnim> deathAnim;
	shared_ptr<DeathAnim> deathAnim1;
	shared_ptr<DeathAnim> deathAnim2;

	float deathTime = 6;
	float deathTime_left = deathTime;

	bool inControl = true;

	float invincibilityTime = 2;
	float invincibilityTime_left = 0;

	float flashTime = 0.01666666;
	float flashTime_left = flashTime;
	bool display = true;

	shared_ptr<SoundBuffer> damageB;
	shared_ptr<Sound> damageSound;

	shared_ptr<Splash> splash;
	bool inWater = false;

	float frictionDecrease = 0;

	bool movable = true;

	bool shootem = false;

public:

	virtual ~player() {

	}

	player(shared_ptr<pController> p1, shared_ptr<SoundCollection> soundCol) {
		
		texture = shared_ptr<Texture> (new Texture());
		if (!texture->loadFromFile("Assets\\player\\NES - Mega Man 2 - Mega Man.png")) {
			cout << "error";
		}
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\weapons.png");

		//shared_ptr<Image> image = shared_ptr<Image> (&texture->copyToImage());
		sprite = shared_ptr<physicsObject> (new physicsObject("player", texture, IntRect(2, 21, 22, 24), Vector2f(1000, 2000), Vector2f(4, 4), 1));

		sprite->setFullColour(Colour::Red());

		pAnim = shared_ptr<playerAnimation> (new playerAnimation(sprite));
		controls = shared_ptr<pControls>(new pControls(p1, sprite, pAnim));


		relativeCPoint = Vector2f(sprite->getSize().x / 2, sprite->getSize().y / 2);
		

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(5 * sprite->getScale().x, 2 * sprite->getScale().y), Vector2i(11, 22)), true, sprite));
		foot = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(7 * sprite->getScale().x, 7 * sprite->getScale().y), Vector2i(7, 20)), true, sprite));
		head = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(7 * sprite->getScale().x, 1 * sprite->getScale().y), Vector2i(7, 2)), true, sprite));
		ladderHit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 24 * sprite->getScale().y), Vector2i(10, 2)), true, sprite));
		ladderBelow = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 26 * sprite->getScale().y), Vector2i(10, 2)), true, sprite));
		ladderAbove = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 21 * sprite->getScale().y), Vector2i(10, 2)), true, sprite));
		belowBox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 26 * sprite->getScale().y), Vector2i(10, 20)), true, sprite));

		shared_ptr<Texture> hbT = shared_ptr<Texture> (new Texture());
		hbT->loadFromFile("Assets\\Health.png");
		health = shared_ptr <energyBar> (new energyBar(hbT, Vector2f(20,20)));

		dam = shared_ptr <damageEffect>(new damageEffect(sprite));



		megaBuster = shared_ptr <MegaBuster>(new MegaBuster(sprite, t, soundCol));
		setActiveWeapon(megaBuster);
		atomicFire = shared_ptr<AtomicFire>(new AtomicFire(sprite, t, soundCol));
		leafShield = shared_ptr< LeafShield>(new  LeafShield(sprite, t, soundCol, relativeCPoint));
		bubbleLead = shared_ptr<BubbleLead>(new BubbleLead(sprite, t, soundCol));
		metalBlade = shared_ptr<MetalBlade>(new MetalBlade(sprite, t, soundCol));
		quickBoomerang = shared_ptr<QuickBoomerang>(new QuickBoomerang(sprite, t, soundCol));
		timeStopper = shared_ptr<TimeStopper>(new TimeStopper(t));
		crashBomb = shared_ptr<CrashBomb>(new CrashBomb(sprite, t, soundCol));
		item1 = shared_ptr<Item1>(new Item1(sprite, t));
		item2 = shared_ptr<Item2>(new Item2(sprite, t));
		item3 = shared_ptr<Item3>(new Item3(sprite, t));
		airShooter = shared_ptr<AirShooter>(new AirShooter(sprite, t, soundCol));

		//Define ammo bars here
		shared_ptr<Texture> aB = shared_ptr<Texture> (new Texture());
		aB->loadFromFile("assets\\bars\\atomic fire.png");
		ammoBar = shared_ptr<AmmoBar>(new AmmoBar(aB, ammoPos));
		ammoBar->setVertical();

		//health->increaseAmount(-27);

		palette = active->getName();
		deathAnim = NULL;
		deathAnim1 = NULL;
		deathAnim2 = NULL;

		damageB = shared_ptr<SoundBuffer> (new SoundBuffer());
		damageB->loadFromFile("assets\\sound\\hit.wav");
		damageSound = shared_ptr<Sound> (new Sound());
		damageSound->setBuffer(*damageB);

		
	}

	void setAutoOn(bool b) {
		controls->setAutoOn(b);
	}

	void decreaseFriction(float frictD) {
		frictionDecrease = frictD;
		controls->setGroundAccel(controls->getStandardAccel() - (frictD));
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
		timeStopper->eachFrame(t);
		delete t;
	}

	Vector2f getPosition() {
		return sprite->getPosition();
	}

	Vector2f getMiddlePos() {
		return Vector2f(sprite->getPosition().x + (sprite->getSize().x / 2), sprite->getPosition().y + (sprite->getSize().y / 2));
	}

	void ladderJumpExtend(list<shared_ptr<tile>> tiles) {
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

	shared_ptr<objectHitbox> getBelowBox() {
		return belowBox;
	}

	bool ladderNotBelow(list<shared_ptr<tile>> tiles) {
		belowBox->updatePos();
		for (shared_ptr<tile> t : tiles) {
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

	shared_ptr<objectHitbox> getFoot() {
		return foot;
	}

	int getETanks() {
		return ETanks;
	}

	shared_ptr<pController> getController() {
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


	void setTimeStopper(bool b) {
		gotTimeStopper = b;
	}

	void setBomb(bool b) {
		gotBomb = b;
	}

	void setAir(bool b) {
		gotAir = b;
	}

	void setLeaf(bool b) {
		gotShield = b;
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

	void setActiveWeapon(shared_ptr<Weapon> w) {
		active = w;
		controls->setWeapon(active);
		loadPallete();
		if (active->getName() == "Mega Buster") {
			displayAmmo = false;
		}
		else {
			displayAmmo = true;
			shared_ptr<Texture> temp = shared_ptr<Texture> (new Texture());
			temp->loadFromFile("assets\\bars\\" + active->getName() + ".png");
			ammoBar = shared_ptr<AmmoBar>(new AmmoBar(temp, ammoPos));
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

	shared_ptr<MegaBuster> getMegaBuster() {
		return megaBuster;
	}
	shared_ptr<AtomicFire> getAtomicFire() {
		return atomicFire;
	}
	shared_ptr<TimeStopper> getTimeStopper() {
		return timeStopper;
	}

	shared_ptr<CrashBomb> getCrashBomb() {
		return crashBomb;
	}

	shared_ptr<AirShooter> getAirShooter() {
		return airShooter;
	}

	shared_ptr<LeafShield> getLeafShield() {
		return leafShield;
	}
	shared_ptr<LeafShield> getShield() {
		return leafShield;
	}

	void setCrashBomb(bool b) {
		gotBomb = b;
	}

	shared_ptr<Item1> getItem1() {
		return item1;
	}

	shared_ptr<Item2> getItem2() {
		return item2;
	}

	shared_ptr<Item3> getItem3() {
		return item3;
	}

	shared_ptr<BubbleLead> getBubbleLead() {
		return bubbleLead;
	}
	shared_ptr<MetalBlade> getMetalBlade() {
		return metalBlade;
	}
	shared_ptr<QuickBoomerang> getBoomerang() {
		return quickBoomerang;
	}

	void setLives(int l) {
		lives = l;
	}


	bool hasAtomicFire() {
		return gotAtomicFire;
	}

	bool hasTimeStopper() {
		return gotTimeStopper;
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

	void iniSplash(shared_ptr<Texture> miscT) {
	
		splash = shared_ptr<Splash>(new Splash(miscT));
	}

	bool setDead() {
		
		invincibilityTime_left = 0;
		display = true;
		if (deathAnim == NULL) {
			deathAnim = shared_ptr<DeathAnim>(new DeathAnim(sprite, palette));
			return true;
		}
		else if (deathTime_left <= deathTime - 0.75 && deathAnim1 == NULL) {
			deathAnim1 = shared_ptr<DeathAnim>(new DeathAnim(sprite, palette));
		}
		else if (deathTime_left <= deathTime - 1.5 && deathAnim2 == NULL) {
			deathAnim2 = shared_ptr<DeathAnim>(new DeathAnim(sprite, palette));
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

	shared_ptr<objectHitbox> getBelow() {
		return ladderBelow;
	}
	shared_ptr<objectHitbox> getAbove() {
		return ladderAbove;
	}

	void start(int startHeight) {

		tele = shared_ptr<teleport>(new teleport(sprite, startHeight));
		//setPosition(Vector2f(pos.x, pos.y));
	}

	shared_ptr<Weapon> getActiveWeapon() {
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

	void updateAmmoWithSound() {
		ammoBar->updateWithSound(active->getAmmo());
	}

	void setShootemControls(bool b) {
		pAnim->resetIdle();
		shootem = b;

		if (b) {
			pAnim->swapDirection(true);
			sprite->setVVelocity(-1);

			setGrounded(false);
			setTempGround(false);
		}

		pAnim->resetIdle();

		//pAnim->setFacingRight(true);
	}

	bool getShootemControls() {
		return shootem;
	}

	Vector2f getRelativeCentrePoint(){
		return relativeCPoint;
	}

	void eachFrame(float* deltaT, list<shared_ptr<tile>> tiles, list<shared_ptr<ItemBullet>>* IBullets, int maxTelePos, shared_ptr<camera> cam) {

		//centrePoint = relativeCPoint + sprite->getPosition();

		splash->eachFrame(deltaT);
		
		if (deathAnim == NULL) {
			if (movable) {
				if (!shootem) {
					alive(deltaT, tiles, IBullets, maxTelePos);
					ladderJumpExtend(tiles);
				}
				else {
					shootemAlive(deltaT, tiles, IBullets, cam);
				}
				
			}
			else {
				display = true;
				invincibilityTime_left = 0;
			}

		}
		else {
			dead(deltaT);
		}

		debugStuff();
		
	}


	

	void debugStuff() {
		if (controls->getController()->checkR()) {
			takeDamage (100);
			lives = 0;
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

	void resetBullets() {
		for (shared_ptr<bullet> b : megaBuster->getBullets()) {
			b->shootReset();
		}
		for (shared_ptr<bullet> b : atomicFire->getBullets()) {
			b->shootReset();
		}
		for (shared_ptr<bullet> b : metalBlade->getBullets()) {
			b->shootReset();
		}
		for (shared_ptr<bullet> b : quickBoomerang->getBullets()) {
			b->shootReset();
		}
		for (shared_ptr<bullet> b : timeStopper->getBullets()) {
			b->uniqueReset();
		}
		for (shared_ptr<bullet> b : bubbleLead->getBullets()) {
			b->shootReset();
		}
		for (shared_ptr<bullet> b : crashBomb->getBullets()) {
			b->shootReset();
		}
	}

	void cameraLogic(shared_ptr<camera> cam, float* deltaT) {
		if (!shootem) {
			cam->followX();
		}
		else {
			if (health->getAmount() > 0) {
			cam->move(Angle::right, deltaT, controls->getAutoSpeed());
		}
		}
	}



	void setFlightPush(bool b) {
		controls->setFlightPush(b);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Do this

	float shootemTele = 0.2;
	float shootemTele_left = shootemTele;

	int getFlightSpeed() {
		return controls->getAutoSpeed();
	}

	void shootemAlive(float* deltaT, list<shared_ptr<tile>> tiles, list<shared_ptr<ItemBullet>>* IBullets, shared_ptr<camera> cam) {
		sprite->setVVelocity(-1);

		ammoBar->update(active->getAmmo());
		if (inControl) {
			if (controls->shootemEachFrame(deltaT, IBullets, cam)) {
				takeDamage(100);
			}
		}
		else {
			//controls->shootemNoControl(deltaT);
		}
		updateFlightHitboxes();

		



		invincibilityTime_left -= *deltaT;

		if (damage) {
			takingDamage(deltaT, tiles);
			dam->flicker(deltaT);
		}

		if (invincibilityTime_left > 0) {
			flash(deltaT);

		}
		else {
			display = true;
		}

		pAnim->shootDecide(deltaT);

		if (pAnim->getShootTime() <= 0) {
			pAnim->idleAnim(deltaT);

		}

		if (unPaused) {
			
			if (tele != NULL) {
				if (tele->justAnimate(deltaT)) {

					pAnim->resetIdle();
					tele = NULL;
					unPaused = false;


				}
			}
			
		}

		controls->shootEachFrame(deltaT, tiles, *IBullets);

	}

	void updateFlightHitboxes() {
		hit->updatePos();
		//ladderHit->updatePos();
		foot->updatePos();
		head->updatePos();
	}

	void alive(float* deltaT, list<shared_ptr<tile>> tiles, list<shared_ptr<ItemBullet>>* IBullets, int maxTelePos) {
		
		ammoBar->update(active->getAmmo());

		if (!damage) {

			if (tele == NULL) {
				if (inControl) {
					controls->checkControls(deltaT, IBullets, frictionDecrease);
				}
				else {
					controls->runWithoutControl(deltaT);
				}
				if (active != timeStopper) {
					controls->shootEachFrame(deltaT, tiles, *IBullets);
				}
				pAnim->shootDecide(deltaT);

				if (!controls->getOnLadder()) {
					//sprite->setFriction(sprite->getFriction() - frictionDecrease);
					sprite->eachFrame(deltaT);
				}
				

				if (controls->checkTeleport()) {
					tele = shared_ptr<teleport>(new teleport(sprite, sprite->getPosition().x));
				}

			}
			else {
				//hit->setRelativePosition(Vector2i(-9999, -9999));
				if (tele->eachFrame(deltaT, tiles, foot, maxTelePos)) {

					tele = NULL;
					pAnim->resetIdle();
					pAnim->getIdle()->thisFrame();
				}
			}
		}
		else {
			if (active != timeStopper) {
				controls->shootEachFrame(deltaT, tiles, *IBullets);
			}
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

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//This checks if the player should be invincible
		if (inControl && tele == NULL) {
			updateHitbox();
		}
		else {
			moveHitbox();
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		invincibilityTime_left -= *deltaT;
		if (invincibilityTime_left > 0) {
			flash(deltaT);
		}
		else {
			display = true;
		}


		//checkHold();

		if (active == timeStopper) {
			controls->shootEachFrame(deltaT, tiles, *IBullets);
		}

		

	}

	void enableControls(bool e) {
		inControl = e;
	}

	void enableMoving(bool e) {
		movable = e;
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

	int getAutoSpeed() {
		return controls->getAutoSpeed();
	}

	int getHoldAdd() {
		return holdAdd;
	}

	void updateLighting() {
		sprite->updateLighting();
	}

	list<shared_ptr<RectangleShape>> getLightPixels() {
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

	shared_ptr<teleport> getTeleport() {

		return tele;

	}

	bool unPaused = false;
	void teleportForceEnd(Vector2f prevPos) {
		if (tele != NULL) {
			tele->forceEnd(prevPos);
			unPaused = true;
			
		}
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
		timeStopper->setAmmo(28);
		item1->setAmmo(28);
		item2->setAmmo(28);
		item3->setAmmo(28);
		leafShield->setAmmo(28);
		airShooter->setAmmo(28);
		quickBoomerang->setAmmo(28);
		metalBlade->setAmmo(28);
		bubbleLead->setAmmo(28);
		crashBomb->setAmmo(28);
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
		if (!shootem) {
			if (movable) {
				return tele != NULL;
			}
		}

		return NULL;
	}

	shared_ptr<objectHitbox> getHitbox() {
		return hit;
	}
	shared_ptr<objectHitbox> getLadderHitbox() {
		return ladderHit;
	}

	shared_ptr<objectHitbox> getHead() {
		return head;
	}

	shared_ptr<physicsObject> getSprite() {
		return sprite;
		

	}

	list<shared_ptr<objectSprite>> getSprites() {

		list<shared_ptr<objectSprite>> temp;

		if (deathAnim == NULL) {
			temp = list<shared_ptr<objectSprite>> {sprite};
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

	shared_ptr<object> getDamEffect() {
		if (deathAnim == NULL) {
			return dam;
		}
		return NULL;
		
	}

	list<shared_ptr<UISprite>> getUI() {
		if (displayAmmo) {
			list<shared_ptr<UISprite>> temp = ammoBar->getSprites();
			temp.push_back(health->getSprite());
			return temp;
		}
		else {
			return list<shared_ptr<UISprite>> {health->getSprite()};
		}
	}

	list<shared_ptr<bullet>> getBullets() {
		return controls->getRealBullets();
	}

	list<shared_ptr<bullet>> getRealBullets() {
		return controls->getRealBullets();
	}

	list<shared_ptr<UISprite>> getUIBullets() {
		return controls->getUIBullets();
	};

	list<shared_ptr<objectHitbox>> getBulletHitboxes() {
		return controls->getBulletHitboxes();
	}

	void setGrounded(bool b) {
		if (!controls->getOnLadder()) {
			grounded = b;
			this->controls->setGrounded(b);
		}
	}
	void setGroundedOverride(bool b) {
		groundedOverride = b;
	}
	bool getGroundedOverride() {
		return groundedOverride;
	}
	shared_ptr<playerAnimation> getAnimation() {
		return pAnim;
	}
	shared_ptr<pControls> getControls() {
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
			if (getShootemControls()) {
				sprite->setVVelocity(-1);
			}
			invincibilityTime_left = invincibilityTime;
			damageSound->play();
		}
	}

	void heal(int h) {
		health->increaseAmount(h);
	}

	string getBulletType() {
		shared_ptr<bullet> b = controls->getBulletObject();
		return b->getSprite()->getType();
	}


	void loadPallete() {
		shared_ptr<Weapon> temp = getWeapon();
		this->texture = temp->getTexture();
		sprite->setTexture(texture);
	}

	shared_ptr<Weapon> getWeapon() {
		return getControls()->getWeapon();
	}

	int getLives() {
		return lives;
	}




private:
	bool takingDamage(float* deltaT, list<shared_ptr<tile>> tileList) {
		tempDTime = tempDTime - *deltaT;
		if (tempDTime <= 0) {
			tempDTime = damageTime;
			damage = false;
			sprite->setMovable(true);
			dam->reset();
			return true;
		}

		if (pAnim->getFacingRight()) {
			sprite->move(180, deltaT, 200);
		}
		else {
			sprite->move(0, deltaT, 200);
		}

		return false;
		
	}

	void tileCollide(list<shared_ptr<tile>> tiles) {
		for (shared_ptr<tile> t : tiles) {
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
		return gotAir;
	}

	bool checkAir() {
		return gotAir;
	}

	bool checkBlade() {
		return gotBlade;
	}
	bool checkBomb() {
		return gotBomb;
	}
	bool checkStopper() {
		return gotTimeStopper;
	}
	bool checkLead() {
		return gotLead;
	}
	bool checkBoomerang() {
		return gotBoomerang;
	}

	bool getTempGround() {
		return tempGround;
	}
	void setTempGround(bool b) {
		tempGround = b;
	}

	
};