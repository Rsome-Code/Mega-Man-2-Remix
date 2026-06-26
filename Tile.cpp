#include "Object Sprite.cpp"
#include "Object Hitbox.cpp"
#include "Object.cpp"
#include "movable object.cpp"

#pragma once

class tile:public object {
protected:
	
	shared_ptr<objectHitbox> groundHitbox;
	shared_ptr<objectHitbox> leftHitbox;
	shared_ptr<objectHitbox> rightHitbox;
	shared_ptr<objectHitbox> ceilingHitbox;
	shared_ptr<objectHitbox> waterHit;
	shared_ptr<objectHitbox> ladder;
	shared_ptr<objectHitbox> deathBox;

	Vector2f location;
	float size = 16 * 4;
	float z;
	int tileNumber;

	string type;

public:
	tile() {
		type = "0";
	}
	tile(Vector2f loc, shared_ptr<Texture> t, int tileNum, float z) {
		this->z = z;
		tileNumber = tileNum;

		int tY = tileNum / 4;
		int tX = tileNum % 4;
		

		location = loc;
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", t, Vector2i(tX * (16), tY * (16)), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1));
		//setTileNum(tileNum);
		type = "0";

		groundHitbox = NULL;
		leftHitbox = NULL;
		rightHitbox = NULL;
		ceilingHitbox = NULL;
		waterHit = NULL;
		ladder = NULL;
		deathBox = NULL;
	}

	tile(shared_ptr<tile> ti) {
		z = ti->getZ();
		tileNumber = ti->getTileNum();
		int tY = tileNumber / 4;
		int tX = tileNumber % 4;

		location = ti->getLocation();
		sprite = shared_ptr<objectSprite>(new objectSprite("Tile", ti->getSprite()->getTexture(), Vector2i(tX * (16), tY * (16)), Vector2i(16, 16), Vector2f(location.x * size, location.y * size), Vector2f(4, 4), 1));
		//setTileNum(tileNum);
		type = ti->getType();

		if (ti->getGround() != NULL) {
			groundHitbox = shared_ptr<objectHitbox>(new objectHitbox(*ti->getGround()));
			groundHitbox->setSprite(sprite);
		}
		if (ti->getLeft() != NULL) {
			leftHitbox = shared_ptr<objectHitbox>(new objectHitbox(*ti->getLeft()));
			leftHitbox->setSprite(sprite);
		}
		if (ti->getRight() != NULL) {
			rightHitbox = shared_ptr<objectHitbox>(new objectHitbox(*ti->getRight()));
			rightHitbox->setSprite(sprite);
		}
		if (ti->getCeiling() != NULL) {
			ceilingHitbox = shared_ptr<objectHitbox>(new objectHitbox(*ti->getCeiling()));
			ceilingHitbox->setSprite(sprite);
		}
		if (ti->getWaterBox() != NULL) {
			waterHit = shared_ptr<objectHitbox>(new objectHitbox(*ti->getWaterBox()));
			waterHit->setSprite(sprite);
		}
		if (ti->getLadder() != NULL) {
			ladder = shared_ptr<objectHitbox>(new objectHitbox(*ti->getLadder()));
			ladder->setSprite(sprite);
		}
		if (ti->getDeathBox() != NULL) {
			deathBox = shared_ptr<objectHitbox>(new objectHitbox(*ti->getDeathBox()));
			deathBox->setSprite(sprite);
		}
	}

	void updateHitboxPos() {
		if (getGround() != NULL) {
			groundHitbox->updatePos();
		}
		if (getLeft() != NULL) {
			leftHitbox->updatePos();
		}
		if (getRight() != NULL) {
			rightHitbox->updatePos();
		}
		if (getCeiling() != NULL) {
			ceilingHitbox->updatePos();
		}
		if (getWaterBox() != NULL) {
			waterHit->updatePos();
		}
		if (getDeathBox() != NULL) {
			deathBox->updatePos();
		}
		if (getLadder() != NULL) {
			ladder->updatePos();
		}
	}

	void setLocation(Vector2f loc) {
		location = loc;
		sprite->setPosition(Vector2f(loc.x * size, loc.y * size));
	}

	shared_ptr<objectSprite> getSprite(){
		return sprite;
	}

	Vector2f getLocation() {
		return location;
	}

	int getTileNum() {
		return tileNumber;
	}
	void setTileNum(int i) {
		int tY = i / 4;
		int tX = i % 4;
		tileNumber = i;


		sprite->setRect(Vector2i(tX * (16), tY * (16)), Vector2i(16, 16));
	}

	float getZ() {
		return z;
	}


	shared_ptr<objectHitbox> getGround() {
		return groundHitbox;
	}
	shared_ptr<objectHitbox> getCeiling() {
		return ceilingHitbox;
	}
	shared_ptr<objectHitbox> getLeft() {
		return leftHitbox;
	}
	shared_ptr<objectHitbox> getRight() {
		return rightHitbox;
	}

	shared_ptr<objectHitbox> getLadder() {
		return ladder;
	}

	shared_ptr<objectHitbox> getDeathBox() {
		return deathBox;
	}

	shared_ptr<objectHitbox> getWaterBox() {
		return waterHit;
	}
	virtual void animate(float* deltaT) {};
	virtual void reset() {};

	virtual void setAbove(bool b) {
		
	}
	virtual void setRight(bool b) {
		
	}
	virtual void setLeft(bool b) {
		
	}
	virtual void setBelow(bool b) {
		
	}

	virtual bool getAbove() {
		return false;
	}
	virtual bool getRightB() {
		return false;
	}
	virtual bool getBelow() {
		return false;
	}
	virtual bool getLeftB() {
		return false;
	}

	virtual void hitboxCopy(shared_ptr<tile> t) {
	}

	virtual list<shared_ptr<objectSprite>> getInternalSprites() {
		return list<shared_ptr<objectSprite>> {};
	}


	virtual void update() {};

	virtual bool checkDist() {
		return true;
	}


	string getType() {
		return type;
	}

	virtual float getMovement() {
		return 0;
	}

	virtual float getFrictionDecrease() {
		return 0;
	}

	virtual void setMoveRight(bool right) {

	}

	virtual shared_ptr<Text> getText(shared_ptr<Font> font) {
		return NULL;
	}

	virtual int getTiming() {
		return NULL;
	}

	virtual void resetBeat() {};

	virtual void deleteInt() {
		
	}

	virtual bool checkCrash() {
		return false;
	}

	virtual void crashSetup() {};

};