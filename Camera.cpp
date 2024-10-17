//This class contains information related to the camera

#include "Object Sprite.cpp"
#pragma once

class camera {
	Vector2f position;
	objectSprite* following;
	Vector2f offset; //This will allow the object the camera is following to be uncentered if you wish
	float fAmount;
	Vector2f xLimit = Vector2f(NULL, NULL);
	Vector2f yLimit = Vector2f(NULL, NULL);

	bool xLim = false;
	bool yLim = false;

	bool startInterp = true;
	bool positiveTargetX = false;
	bool positiveTargetY = false;

	float zoom = 1;

public:
	camera() {
		position = Vector2f(0, 0);
	}

	camera(Vector2f v) {
		position = v;
	}

	camera(objectSprite* following, Vector2f offset) {
		this->position = Vector2f(0, 0);
		this->following = following;
		this->offset = offset;
		fAmount = getFAmount();
	}
public:

	void setZoom(float z){
	zoom = z;
	}

	float getZoom() {
		return zoom;
	}

	camera(objectSprite* following) {
		this->position = Vector2f(0, 0);
		this->following = following;
		this->offset = Vector2f(-960, -800);
		fAmount = getFAmount();
	}

	void setPosition(Vector2f pos) {
		  this->position = pos;
	}

	void setXLimit(Vector2f l) {
		xLimit = l;
		xLim = true;
	}

	void setYLimit(Vector2f l) {
		yLimit = l;
		yLim = false;
	}

	void follow() {
		followX();
		followY();
	}

	void followX() {
		if (xLim) {
			Vector2f newPos = Vector2f(following->getPosition().x + offset.x, position.y);
			if (newPos.x > xLimit.x && newPos.x < xLimit.y) {
				position = Vector2f(following->getPosition().x + offset.x, position.y);
			}
		}
		else {
			position = Vector2f(following->getPosition().x + offset.x, position.y);
		}
	}
	void followY() {
		if (yLim) {
			if (position.y > yLimit.x && position.y < yLimit.y) {
				position = Vector2f(position.x, following->getPosition().y + offset.y);
			}
		}
		else {
			position = Vector2f(position.x, following->getPosition().y + offset.y);
		}
	}

	Vector2f getPosition() {
		return this->position;
	}
	Vector2f getOffset() {
		return this->offset;
	}
public:
	void setOffset(Vector2f off) {
		offset = off;
	}


	bool interpTo(Vector2f pos, float speed, float* deltaT) {

		if (startInterp == true) {
			if (pos.x >= position.x) {
				positiveTargetX = true;
			}
			else {
				positiveTargetX = false;
			}
			if (pos.y >= position.y) {
				positiveTargetY = true;
			}
			else {
				positiveTargetY = false;
			}
			startInterp = false;

		}

		float distance = speed * *deltaT;
		float angle = atan(abs(position.x - pos.x)/ abs(position.y - pos.y)) * 57.295779513082320876798154814105;

		if (pos.x >= position.x) {
			if (pos.y >= position.y) {
				angle = angle + 270;
			}
		}
		else if (pos.x < position.x) {
			if (pos.y >= position.y) {
				angle = angle + 180;
			}
			else {
				angle = angle + 90;
			}
		}
		move(angle, distance);


		if (positiveTargetX) {
			if (position.x > pos.x) {
				position = pos;
				startInterp = true;
				return true;
			}
		}
		else {
			if (position.x < pos.x) {
				position = pos;
				startInterp = true;
				return true;
			}
		}
		if (positiveTargetY) {
			if (position.y > pos.y) {
				position = pos;
				startInterp = true;
				return true;
			}
		}
		else {
			if (position.y < pos.y) {
				position = pos;
				startInterp = true;
				return true;
			}
		}
		return false;
	}

private:
	Vector2f calcXY(float angle, float dist) {
		angle = angle / 57.295779513082320876798154814105;
		float adjacent = cos(angle) * dist;
		float opposite = sin(angle) * dist;

		Vector2f newPos = Vector2f(position.x + adjacent, position.y + opposite);

		return newPos;
	}

public:

	void move(float angle, float* since, float speed) {

		float last = *since;

		float thisFrameD = speed * last;

		Vector2f amount = Vector2f(calcXY(angle, thisFrameD));
		setPosition(amount);
	}
	void move(float angle, float dist) {

		float thisFrameD = dist;

		Vector2f amount = Vector2f(calcXY(angle, thisFrameD));
		setPosition(amount);
	}

//The following relates to when the object the camera is following is flipped horizontally.

public:bool checkFlip(){
	return following->getFlipped();
}

public: objectSprite* getFollowing() {
	return following;
}
public: int getFlipInc() {
	return following->getRectX();
}
public:float getFAmount() {
	return following->getFAmount();
}
public:void flipCamera() {
	if (following->getFlipped()){
		this->position = Vector2f(position.x + fAmount, position.y);
	}
	else {
		this->position = Vector2f(position.x - fAmount, position.y);
	}

}
};