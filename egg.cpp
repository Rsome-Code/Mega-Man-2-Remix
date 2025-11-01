#include "temp physics enemy.cpp"
#include "small bird.cpp"
#pragma once

class Egg :public TempPhysicsEnemy {
	using TempPhysicsEnemy::TempPhysicsEnemy;
	bool dropped = false;

public:
	void initial() {

		hp = 1;
		phys->setRect(IntRect(469, 437, 13, 10));
		hit = new objectHitbox(IntRect(0, 0, 13, 10), phys);
		hurt = new objectHitbox(IntRect(0, 0, 13, 10), phys);
		grounded = false;
		offSetList();
		damage = 4;
	}

	void alive(player* p, float* deltaT, list<tile*>* tileList, list<enemy*>* objectList, list<EnemyBullet*>* bList, SoundCollection* soundCol) {


		if (dropped) {
			isDropped(tileList, objectList, deltaT, soundCol);
		}
		
	}

	void isDropped(list<tile*>* tileList, list<enemy*>* objectList, float* deltaT, SoundCollection* soundCol) {
		phys->enableGravity(true);
		phys->eachFrame(deltaT);
		tileCollision(tileList);
		
		if (grounded) {
			spawnBirds(objectList, soundCol);
			hp = 0;
		}
	}



	void spawnBirds(list<enemy*>* objectList, SoundCollection* soundCol) {
		for (int i = 0; i < 8; i++) {
			SmallBird* temp = new SmallBird(sprite->getTexture(), sprite->getMiddlePos());
			temp->initial();
			temp->setHitSound(soundCol->getHit());
			objectList->push_back(temp);
		}
	}

	void setDropped(bool d){
		dropped = d;
	}
};