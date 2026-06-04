#include "physics object.cpp"
#include "object hitbox.cpp"
#include "animation timer.cpp"
#include "tile.cpp"
#pragma once

class ItemBullet {
protected:
	shared_ptr<physicsObject> phys;

	shared_ptr<objectHitbox> hitbox;
	string code;

	bool display = true;

	shared_ptr<animation> exploAnim;
	shared_ptr<animTimer> exploTimer;

	bool explode = false;

public:

	void exploSetup() {
		exploAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(565, 241, 16, 16), IntRect(584, 243, 12, 12), IntRect(602, 244, 10, 10), IntRect(618, 247, 4, 4)}, phys));
		exploAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, -6 * 4), Vector2f(2 * 4, -4 * 4), Vector2f(3 * 4, -3 * 4), Vector2f(6 * 4, 0 * 4)});
		exploTimer = shared_ptr<animTimer>(new animTimer(exploAnim, 16, false));
	}

	bool explodeLoop(float* deltaT) {

		exploTimer->run(deltaT);
		return exploTimer->isFinished();

	}

	void setExplode(bool b) {


		if (b) {
			Vector2f prevMiddle = phys->getMiddlePos();
			exploAnim->thisFrame();
			phys->setPosition(Vector2f(prevMiddle.x - (phys->getSize().x / 2), prevMiddle.y));
		}
		explode = b;
	}

	bool getExplode() {
		return explode;
	}

	virtual bool eachFrame(float* deltaT, list<shared_ptr<tile>> tiles) {
		return false;
	}

	shared_ptr<objectHitbox> getHit() {
		return hitbox;
	}

	shared_ptr<physicsObject> getSprite() {
		if (display) {
			return phys;
		}
		return NULL;
	}

	string getCode() {
		return code;
	}

	virtual Vector2f getThisframeDistance() {
		return Vector2f(0,0);
	}
};