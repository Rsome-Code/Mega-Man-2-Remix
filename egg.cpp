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
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 13, 10), phys));
		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 13, 10), phys));
		grounded = false;
		offSetList();
		damage = 4;
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol) {


		if (dropped) {
			isDropped(tileList, objectList, deltaT, soundCol);
		}
		
	}

	void isDropped(list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, float* deltaT, shared_ptr<SoundCollection> soundCol) {
		phys->enableGravity(true);
		phys->eachFrame(deltaT);
		tileCollision(tileList);
		
		if (grounded) {
			spawnBirds(objectList, soundCol);
			hp = 0;
		}
	}



	void spawnBirds(list<shared_ptr<enemy>>* objectList, shared_ptr<SoundCollection> soundCol) {
		for (int i = 0; i < 8; i++) {
			shared_ptr<SmallBird> temp = shared_ptr<SmallBird>(new SmallBird(sprite->getTexture(), sprite->getMiddlePos()));
			temp->initial();
			temp->setHitSound(soundCol->getHit());
			objectList->push_back(temp);
		}
	}

	void setDropped(bool d){
		dropped = d;
	}
};