#include "enemy.cpp"
#pragma once

class TempEnemy :public enemy {
	using enemy::enemy;

public:
	bool isDead(list<enemy*>* tempEList) {
		//tempEList->remove(this);
		//delete this;
		return true;
	}

	virtual void initial(Vector2f pos) {};
};