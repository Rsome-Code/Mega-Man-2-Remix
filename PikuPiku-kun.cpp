#include "Wily Boss.cpp"
#include "Piku Head.cpp"
#pragma once

class PikuPikuKun : public WilyBoss {
public:
	using WilyBoss::WilyBoss;

	vector<shared_ptr<PikuHead>> pikus;

	shared_ptr<Texture> tex;



	void initial() {
		tex = shared_ptr<Texture>(new Texture());
		tex->loadFromFile("Assets//Wily Bosses.png");

		hp = 28;
		
		firstFrame = true;

		double delay = 0.4;

		if (pikus.empty()) {

			for (int i = 0; i < 10; i++) {
				shared_ptr<PikuHead> temp = shared_ptr<PikuHead>(new PikuHead(tex, initialPos));
				temp->initial();
				temp->setDelay(i * delay);
				pikus.push_back(temp);
			}
		}
		code = "pikopiko-kun";
		bossInitial(code);
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(-999999, 0, 0, 0), phys));
		hurt = hit;
		musicSetup();
	}
	bool introAnim(float* deltaT) {


		return true;
	}


	bool firstFrame = true;

	void uniqueDeath() {
		for (shared_ptr <PikuHead> pi : pikus) {
			pi->setDisplay(false);
			pi->setAct(false);
		}
	};

	
	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<GameObject>>* obList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol, shared_ptr<camera> cam) {
		
		if (!dead) {

			if (firstFrame) {
				for (shared_ptr <PikuHead> pi : pikus) {
					pi->setHitSound(soundCol->getHit());
					objectList->push_front(pi);
				}
				firstFrame = false;
			}


			checkDeaths(cam);
		}

		sortPikus(objectList);
		

	}

	void sortPikus(list<shared_ptr<enemy>>* objectList) {
		bool loop = true;

		while (loop) {
			loop = false;
			int i = 0;

			int swap1 = -1;
			int swap2 = -1;

			for (shared_ptr <PikuHead> p1 : pikus) {
				int j = 0;
				for (shared_ptr <PikuHead> p2 : pikus) {
					if (j > i) {
						
						float z1 = p1->getSprite()->getZ();
						float z2 = p2->getSprite()->getZ();
						if (z1 < z2) {
							swap1 = i;
							swap2 = j;
							loop = true;
							break;
						}
						
					}
					
					j++;
				}
				if (loop) {
					break;
				}
				i++;
			}

			if (swap1 != -1) {
				swapPikus(swap1, swap2);
			}
		}

		for (shared_ptr <PikuHead> p1 : pikus) {
			objectList->remove(p1);
		}
		for (shared_ptr <PikuHead> p1 : pikus) {
			objectList->push_back(p1);
		}
	}

	void swapPikus(int swap1, int swap2) {
		swap(pikus[swap1], pikus[swap2]);
	}
	
	void checkDeaths(shared_ptr<camera> cam) {
		int numDead = 0;
		for (shared_ptr<PikuHead> piku : pikus) {
			if (piku->getHP() <= 0) {
				numDead++;
				//piku->setDisplay(false);
				//piku->setAct(false);
			}
			else {
				piku->setDisplay(true);
				piku->setAct(true);
			}
		}

		float change = float(28) / float(pikus.size());

		healthBar->update(28 - (change * numDead));

		if (numDead == pikus.size() - 1) {
			setDeathAnim();
		}
		if (numDead == pikus.size()) {
			hp = 0;
			this->getSprite()->setPosition(cam->getPosition() + deathAnimPos);
			bossMusic->stop();
		}
	}

	Vector2f deathAnimPos = Vector2f(0,0);
	void setDeathAnim() {
		for (shared_ptr<PikuHead> piku : pikus) {
			if (piku->getHP() > 0) {
				deathAnimPos = piku->getSprite()->getCameraPosition();
				piku->setCode(getCode());
			}
		}
	}

	bool deleteOverY() {
		return false;
	}
	bool deleteOverX() {
		return false;
	}


};