#include "enemy.cpp"
#pragma once

class TempEnemy :public enemy {
	using enemy::enemy;

protected:
	bool spawned = true;

public:
	bool isDead(list<enemy*>* tempEList) {
		//tempEList->remove(this);
		//delete this;
		
		return spawned;
	}

	virtual void initial(Vector2f pos) {};
};