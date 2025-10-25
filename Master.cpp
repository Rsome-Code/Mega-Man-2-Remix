#include  "physics enemy.cpp"
#include "death animation.cpp"
#pragma once

class Master : public PhysicsEnemy {
	using PhysicsEnemy::PhysicsEnemy;

protected:
	DeathAnim* deathAnimation;
	DeathAnim* deathAnimation1;
	DeathAnim* deathAnimation2;

	animation* introAnim;
	animTimer* introTimer;
	AmmoBar* healthBar;

	float deathAnimTime = 4;
	float deathAnimTime_left = deathAnimTime;

	int introHP = 0;
	
	float healRate = 0.1;
	float healRate_left = healRate;

	bool tauntDone = false;
	bool introDone = false;
	
	Music* bossMusic;
	bool noMusic = true;

	float invincibleTime = 0.5;
	float invincibleTime_left = 0;

	float flashTime = 0.04;
	float flashTime_left = flashTime;

	objectSprite* damSprite;
	Vector2f damPos = Vector2f(sprite->getMiddlePos().x - (12 * 4), sprite->getMiddlePos().y - (12 * 4));

public:

	void masterInitial(string weaponName) {
		hp = 28;

		Texture* t = new Texture();
		t->loadFromFile("assets\\bars\\" + weaponName + ".png");

		healthBar = new AmmoBar(t, Vector2f(20 + (3 * (8 * 4)), 20));
		healthBar->setVertical();
		healthBar->update(0);
		phys->enableGravity(true);
		grounded = false;

		deathAnimation = new DeathAnim(sprite, weaponName);
		deathAnimation1 = new DeathAnim(sprite, weaponName);
		deathAnimation2 = new DeathAnim(sprite, weaponName);

		bossMusic = new Music();
		bossMusic->openFromFile("assets\\sound\\music\\14 - Boss Battle.wav");
		bossMusic->setVolume(50);
		bossMusic->setLoop(true);
		bossMusic->setLoopPoints({ sf::seconds(6.4), seconds(100) });

		damSprite = new objectSprite("effect", sprite->getTexture(), IntRect(433, 0, 24, 24), Vector2f(0,0), Vector2f(4, 4));
	}

	bool tauntLoop(float* deltaT, list<tile*>* tiles) {
		
		if (grounded) {
			introTimer->run(deltaT);
			if (introTimer->isFinished()) {
				return barIntro(deltaT);
			}
		}
		else {
			phys->eachFrame(deltaT);
			hit->updatePos();
			for (tile* t : *tiles) {
				if (t->getGround() != NULL) {
					groundCheck(t);
				}
			}
		}

		return false;
	}

	bool titleLoop(float* deltaT, UIHitbox* floor) {
			
		if (!grounded) {
			phys->eachFrame(deltaT);
			hit->updatePos();
			if (hit->getPosition().y > floor->getCameraPos().y) {
				grounded = true;
				phys->setPosition(Vector2f(phys->getPosition().x, floor->getCameraPos().y));
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
			healthBar->update(introHP);
			
		}
		return false;
	}

	bool eachFrame(float* deltaT, player* p, list<tile*>* tileList, list<enemy*>* enemyList, list<EnemyBullet*>* bList) {
		damPos = Vector2f(sprite->getMiddlePos().x - (12 * 4), sprite->getMiddlePos().y - (12 * 4));
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
			if (p->getGrounded()) {
				introLoop(deltaT, tileList);
			}
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

	void introLoop(float* deltaT, list<tile*>* tileList) {
		if (!tauntDone) {
			tauntDone = tauntLoop(deltaT, tileList);
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

	bool death(float* deltaT, list<enemy*>* tempEList){

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

	AmmoBar** getBar() {
		return &healthBar;
	}

	vector<DeathAnim**> getDeathAnims() {
		return vector<DeathAnim**> {&deathAnimation, &deathAnimation1, &deathAnimation2};
	}

	void spawnItem(list<Item*>* obList, Texture* t, Vector2f pos) {

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

	objectSprite* getDamSprite() {
		return damSprite;
	}

	int genericDam() {
		return 2;
	}

};