#include "solid tile.cpp"
#pragma once

enum Placement {
	above, rightOf, below, leftOf
};

class CrashTile : public solidTile {
protected:

	using solidTile::solidTile;

	
	bool above;
	bool right;
	bool below;
	bool left;
public:

	void setAbove(bool b) {
		above = b;
	}
	void setRight(bool b) {
		right = b;
	}
	void setLeft(bool b) {
		left = b;
	}
	void setBelow(bool b) {
		below = b;
	}

	bool getAbove() {
		return above;
	}
	bool getRightB() {
		return right;
	}
	bool getBelow() {
		return below;
	}
	bool getLeftB() {
		return left;
	}


	virtual void crashSetup() = 0;

	void groundSetup() {
		ceilingHitbox = NULL;
		leftHitbox = NULL;
		rightHitbox = NULL;
	}
	void ceilingSetup() {
		groundHitbox = NULL;
		leftHitbox = NULL;
		rightHitbox = NULL;
	}
	void nonSolidSetup() {
		groundHitbox = NULL;
		ceilingHitbox = NULL;
		leftHitbox = NULL;
		rightHitbox = NULL;
	}
	void leftSetup() {
		groundHitbox = NULL;
		ceilingHitbox = NULL;
		rightHitbox = NULL;
	}
	void rightSetup() {
		groundHitbox = NULL;
		ceilingHitbox = NULL;
		leftHitbox = NULL;
	}

	void hitboxCopy(shared_ptr<tile> t) {
		ceilingHitbox = t->getCeiling();
		leftHitbox = t->getLeft();
		rightHitbox = t->getRight();
		groundHitbox = t->getGround();
	}

	bool checkCrash() {
		return true;
	}

};