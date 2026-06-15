#include "physics enemy.cpp"
#pragma once

class WilyBoss : public PhysicsEnemy {
public:
	using PhysicsEnemy::PhysicsEnemy;
protected:

	float healRate = 0.1;
	float healRate_left = healRate;

	bool introDone = false;
	bool tauntDone = false;

	shared_ptr<Music> bossMusic;
	bool noMusic = true;

	float invincibleTime = 0.5;
	float invincibleTime_left = 0;

	shared_ptr<AmmoBar> healthBar;

	int introHP = 0;

	Vector2f prevPos = Vector2f(0, 0);

	void bossInitial(string bossName) {
		hp = 28;
		introDone = false;
		tauntDone = false;
		shared_ptr<Texture> t = shared_ptr<Texture>(new Texture());
		t->loadFromFile("assets\\bars\\" + bossName + ".png");

		healthBar = shared_ptr<AmmoBar>(new AmmoBar(t, Vector2f(20 + (3 * (8 * 4)), 20)));
		healthBar->setVertical();
		healthBar->update(0);


	}

	void musicSetup() {
		bossMusic = shared_ptr<Music>(new Music());
		bossMusic->openFromFile("assets\\sound\\music\\14 - Boss Battle.wav");
		bossMusic->setVolume(50);
		bossMusic->setLoop(true);
		bossMusic->setLoopPoints({ sf::seconds(6.4), seconds(100) });
	}

	void introLoop(float* deltaT, list<shared_ptr<tile>>* tileList, shared_ptr<player> p) {
		if (!tauntDone) {
			tauntDone = tauntLoop(deltaT, tileList, p);
		}
		else {
			introDone = barIntro(deltaT);
		}
	}

	virtual bool tauntLoop(float* deltaT, list<shared_ptr<tile>>* tiles, shared_ptr<player> p) {


		if (p->getGrounded() || p->getShootemControls()) {
			return introAnim(deltaT);
		}

		return false;
	}
	shared_ptr<AmmoBar>* getBar() {
		return &healthBar;
	}

	virtual bool introAnim(float* deltaT) {
		return true;
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
	void stopMusic() {
		bossMusic->stop();
	}

	


	bool death(float* deltaT, list<shared_ptr<enemy>>* tempEList) {
		if (!dead) {
			//sprite->setPosition(Vector2f(sprite->getMiddlePos().x - (12 * 4), sprite->getMiddlePos().y - (12 * 4)));
			dead = true;
			deathAnim->reset();
		}
		hit->setPosition(Vector2f(-1000, 0));
		hurt->setPosition(Vector2f(-1000, 0));

		//deathAnim->thisFrame();

		if (!deathTimer->isFinished()) {
			deathTimer->run(deltaT);
		}
		else {
			return isDead(tempEList);

		}
		return false;
	}


	//void introLoop(float* deltaT, list<shared_ptr<tile>>* tileList, shared_ptr<player> p) {
		
		
		
		//else {
			//introDone = barIntro(deltaT);
		//}
	//}

	bool getIntroDone() {
		return introDone;
	}

	virtual bool eachFrame(float* deltaT, shared_ptr<player> p, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* enemyList, list<shared_ptr<EnemyBullet>>* bList, list<shared_ptr<GameObject>>* objectList, shared_ptr<SoundCollection> soundCol, shared_ptr<camera> cam) {

		prevPos = phys->getPosition();

		if (introDone) {
			healthBar->update(hp);
			if (hp > 0) {
				if (act) {
					hit->updatePos();
					hurt->updatePos();
					alive(p, deltaT, tileList, enemyList, bList);
					alive(p, deltaT, tileList, enemyList, objectList, bList, soundCol);
					alive(p, deltaT, tileList, enemyList, objectList, bList, soundCol, cam);

				}
			}
			else { return death(deltaT, enemyList); }

			
		}
		else {
			if (noMusic) {
				bossMusic->play();
				noMusic = false;
			}

			if (!tauntDone) {
				tauntDone = tauntLoop(deltaT, tileList, p);
			}
			else {
				introDone = barIntro(deltaT);
			}
			

		}

		//invincibleLoop(deltaT);

		return false;
	}
};