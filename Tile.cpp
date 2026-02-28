#include "Object Sprite.cpp"
#include "Object Hitbox.cpp"
#include "Object.cpp"
#include "movable object.cpp"

#pragma once

class tile:public object {
protected:
	
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

	virtual shared_ptr<objectHitbox> getGround() { return NULL; };
	virtual shared_ptr<objectHitbox> getCeiling() { return NULL; };
	virtual shared_ptr<objectHitbox> getLeft() { return NULL; };
	virtual shared_ptr<objectHitbox> getRight() { return NULL; };
	virtual shared_ptr<objectHitbox> getLadder() { return NULL; };
	virtual shared_ptr<objectHitbox> getDeathBox() { return NULL; };
	virtual shared_ptr<objectHitbox> getWaterBox() {return NULL;};
	virtual void animate(float* deltaT) {};
	virtual void reset() {};

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

};