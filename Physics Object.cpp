#include "Movable Object.cpp"
#pragma once

class physicsObject :public movable {
	using movable::movable;
	float horizontalAcc;
	float verticalAcc;
	float friction = 0;
	float gravity = -3000;
	float maxSpeed = 3000;
	bool grav = false;
	bool firstFrame = true;

	float speed = 0;

	bool movable = true;

	float HDistanceTraveled = 0;

public:

	void frictionCalc(Vector2f f) {
		if (horizontalAcc > 0) {
			horizontalAcc = horizontalAcc - f.x;
			if (horizontalAcc < 0) {
				horizontalAcc = 0;
			}
		}
		else if (horizontalAcc < 0) {
			horizontalAcc = horizontalAcc + f.x;
			if (horizontalAcc > 0) {
				horizontalAcc = 0;
			}
		}
		if (verticalAcc > 0) {
			verticalAcc = verticalAcc - f.y;
			if (verticalAcc < 0) {
				verticalAcc = 0;
			}
		}
		else if (verticalAcc < 0){
			verticalAcc = verticalAcc + f.y;
			if (verticalAcc > 0) {
				verticalAcc = 0;
			}
		}
		
	}

	void setGravity(float g) {
		gravity = g;
	}
	void enableGravity(bool b) {
		grav = b;
	}

	void eachFrame(float* deltaT) {

		if (movable) {

			float f = friction * *deltaT;

			//Early, must ammend for vertical friction
			frictionCalc(Vector2f(f, 0));
			if (firstFrame) {
				firstFrame = false;
			}
			else {
				if (grav) {
					addVerticalForce(gravity, deltaT);
				}
			}

			if (horizontalAcc == 0 && verticalAcc == 0) {}
			else {
				if (verticalAcc > maxSpeed) {
					verticalAcc = maxSpeed;
				}
				else if (verticalAcc < -maxSpeed) {
					verticalAcc = -maxSpeed;
				}
				if (horizontalAcc > maxSpeed) {
					horizontalAcc = maxSpeed;
				}
				else if (horizontalAcc < -maxSpeed) {
					horizontalAcc = -maxSpeed;
				}
				float thisH = horizontalAcc * *deltaT;
				float thisV = verticalAcc * *deltaT;
				float h2 = thisH * thisH;
				float v2 = thisV * thisV;
				float hyp = sqrt(h2 + v2);

				float tempX = abs(thisH);
				float tempY = abs(thisV);
				float angle = atan((tempY / tempX)) * 57.295779513082320876798154814105;


				if (thisH < 0) {
					if (thisV <= 0) {

						angle = 90 - angle;

						angle = angle + 90;
					}
					else if (thisV > 0) {
						angle = angle + 180;
					}
				}
				else if (thisH >= 0) {
					if (thisV > 0) {
						angle = 90 - angle;
						angle = angle + 270;
					}
				}

				speed = hyp;

				this->move(angle, hyp);
				HDistanceTraveled = thisH;

			}
		}

	}

	float getHDistanceTraveled() {
		return HDistanceTraveled;
	}


	void addHorizontalForce(float i, float* deltaT) {
		float increase = i * *deltaT;
		horizontalAcc = horizontalAcc + increase;

	}
	void addVerticalForce(float i, float* deltaT) {
		float increase = i * *deltaT;
		verticalAcc = verticalAcc + increase;
	}
	void addForce(Vector2f i, float* deltaT) {
		float increaseH = i.x * *deltaT;
		horizontalAcc = increaseH;
		float increaseV = i.y * *deltaT;
		verticalAcc = increaseV;
	}
	Vector2f getAcceleration() {
		return Vector2f(horizontalAcc, verticalAcc);
	}
	float getSpeed() {
		return speed;
	}
	void setHVelocity(float v) {
		this->horizontalAcc = v;
	}
	void setVVelocity(float v) {
		this->verticalAcc = v;
	}
	void setFriction(float f) {
		friction = f;
	}
	void setMovable(bool b) {
		movable = b;
	}
};