#include  "physics enemy.cpp"
#include "death animation.cpp"
#pragma once

class Master : public PhysicsEnemy {
	using PhysicsEnemy::PhysicsEnemy;

protected:
	shared_ptr<DeathAnim> deathAnimation;
	shared_ptr<DeathAnim> deathAnimation1;
	shared_ptr<DeathAnim> deathAnimation2;

	shared_ptr<animation> introAnim;
	shared_ptr<animTimer> introTimer;
	shared_ptr<AmmoBar> healthBar;

	float deathAnimTime = 4;
	float deathAnimTime_left = deathAnimTime;

	int introHP = 0;
	
	float healRate = 0.1;
	float healRate_left = healRate;

	bool tauntDone = false;
	bool introDone = false;
	
	shared_ptr<Music> bossMusic;
	bool noMusic = true;

	float invincibleTime = 0.5;
	float invincibleTime_left = 0;

	float flashTime = 0.04;
	float flashTime_left = flashTime;

	shared_ptr<objectSprite> damSprite;
	Vector2f damPos = Vector2f(sprite->getMiddlePos().x - (12 * 4), sprite->getMiddlePos().y - (12 * 4));

	Vector2i damageSpritePos = Vector2i(433, 0);

public:

	void masterInitial(string weaponName) {
		hp = 28;
		introDone = false;
		tauntDone = false;
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("assets\\bars\\" + weaponName + ".png");

		healthBar = shared_ptr<AmmoBar>(new AmmoBar(t, Vector2f(20 + (3 * (8 * 4)), 20)));
		healthBar->setVertical();
		healthBar->update(0);
		phys->enableGravity(true);
		grounded = false;

		deathAnimation = shared_ptr<DeathAnim>(new DeathAnim(sprite, weaponName));
		deathAnimation1 = shared_ptr<DeathAnim>(new DeathAnim(sprite, weaponName));
		deathAnimation2 = shared_ptr<DeathAnim>(new DeathAnim(sprite, weaponName));

		bossMusic = shared_ptr<Music>(new Music());
		bossMusic->openFromFile("assets\\sound\\music\\14 - Boss Battle.wav");
		bossMusic->setVolume(50);
		bossMusic->setLoop(true);
		bossMusic->setLoopPoints({ sf::seconds(6.4), seconds(100) });

		damSprite = shared_ptr<objectSprite>(new objectSprite("effect", sprite->getTexture(), IntRect(damageSpritePos, Vector2i(24, 24)), Vector2f(0,0), Vector2f(4, 4)));
	}

	bool tauntLoop(float* deltaT, list<shared_ptr<tile>>* tiles, shared_ptr<player> p) {


		if (grounded && p->getGrounded()) {
			introTimer->run(deltaT);
			if (introTimer->isFinished()) {
				return barIntro(deltaT);
			}
		}

		else {
			phys->eachFrame(deltaT);
			hit->updatePos();
			for (shared_ptr<tile> t : *tiles) {
				if (t->getGround() != NULL) {
					groundCheck(t);
					introAnim->thisFrame();
				}
			}
		}

		return false;
	}

	void tauntStart() {
		grounded = false;
	}

	bool titleLoop(float* deltaT, shared_ptr<objectHitbox> floor) {
			
		if (!grounded) {
			phys->eachFrame(deltaT);
			hit->updatePos();
			if (hit->getPosition().y > floor->getPosition().y) {
				grounded = true;
				phys->setPosition(Vector2f(phys->getPosition().x, floor->getPosition().y));
			}
		}
		else {
			introTimer->run(deltaT);
			if (introTimer->isFinished()) {
				return true;
			}
		}


		return false;
	}



	bool barIntro(float* deltaT) {
		healRate_left -= *deltaT;

		if (healRate_left <= 0) {

			if (introHP >= 28) {
				return true;
			}

			introHP += 1;
			healRate_left = healRate;
			healthBar->updateWithSound(introHP);
			
		}
		return false;
	}

	virtual bool eachFrame(float* deltaT, shared_ptr<player> p, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* enemyList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {
		damPos = Vector2f(sprite->getPosition().x + (1 * 4), sprite->getPosition().y + (1 * 4));
		if (introDone) {
			if (hp > 0) {
				if (act) {
					hit->updatePos();
					hurt->updatePos();
					alive(p, deltaT, tileList, enemyList, bList);
				}
			}
			else { return death(deltaT, enemyList); }

			healthBar->update(hp);
		}
		else {
			if (noMusic) {
				bossMusic->play();
				noMusic = false;
			}
			
			introLoop(deltaT, tileList, p);
			
		}

		invincibleLoop(deltaT);

		return false;
	}

	void invincibleLoop(float* deltaT) {
		invincibleTime_left -= *deltaT;
		if (invincibleTime_left > 0) {
			flashLoop(deltaT);
			damSprite->setPosition(damPos);
		}
		else {
			display = true;
			damSprite->setPosition(Vector2f(0, 0));
		}
	}

	void flashLoop(float* deltaT) {
		flashTime_left -= *deltaT;
		if (flashTime_left <= 0) {
			flashTime_left = flashTime;
			display = !display;
		}
	}

	void introLoop(float* deltaT, list<shared_ptr<tile>>* tileList, shared_ptr<player> p) {
		if (!tauntDone) {
			tauntDone = tauntLoop(deltaT, tileList, p);
		}
		else {
			introDone = barIntro(deltaT);
		}
	}

	bool getIntroDone() {
		return introDone;
	}

	void stopMusic() {
		bossMusic->stop();
	}

	bool death(float* deltaT, list<shared_ptr<enemy>>* tempEList){

		bossMusic->stop();

		healthBar->update(0);

		if (act) {
			
			hit->setPosition(Vector2f(-1000, 0));
			hurt->setPosition(Vector2f(-1000, 0));


			if (deathAnimLoop(deltaT)) {

				isDead(tempEList);
			}
		}

		return false;
	}

	bool deathAnimLoop(float* deltaT) {

		display = false;

		deathAnimation->setOrigin(sprite);
		deathAnimation1->setOrigin(sprite);
		deathAnimation2->setOrigin(sprite);

		deathAnimTime_left -= *deltaT;
		if (deathAnimTime_left <= 0) {
			return true;
		}

		if (deathAnimTime_left <= deathAnimTime - 0.15) {
			deathAnimation->run(deltaT);
		}
		if (deathAnimTime_left <= deathAnimTime - 0.65) {
			deathAnimation1->run(deltaT);
		}
		if (deathAnimTime_left <= deathAnimTime - 1.15) {
			deathAnimation2->run(deltaT);
		}
		return false;
	}

	shared_ptr<AmmoBar>* getBar() {
		return &healthBar;
	}

	vector<shared_ptr<DeathAnim>*> getDeathAnims() {
		return vector<shared_ptr<DeathAnim>*> {&deathAnimation, &deathAnimation1, &deathAnimation2};
	}



	void spawnItem(list<shared_ptr<Item>>* obList, shared_ptr<Texture> t, Vector2f pos, shared_ptr<SoundCollection> soundCol) {

	}

	bool checkInvincible() {
		return invincibleTime_left > 0;
	}

	void lowerHP(int h) {
		if (h > 0) {
			hitSound->play();
			damaged = true;
			invincibleTime_left = invincibleTime;
		}
		hp = hp - h;
	}

	shared_ptr<objectSprite> getDamSprite() {
		return damSprite;
	}

	void forceDamSprite() {
		damSprite->setPosition(damPos);
	}
	void forceDamSpriteOff() {
		damSprite->setPosition(Vector2f(0,0));
	}

	int genericDam() {
		return 2;
	}

};