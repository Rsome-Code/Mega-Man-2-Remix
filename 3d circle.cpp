#include "movable object.cpp"
#pragma once


class MenuCircle {

	Vector3f centrePoint = Vector3f(1920/2, 300, 100);

	float furthest = 0;
	float closest = 1000;

	float radius = 500;
	float circumference = 1000;



public:

	MenuCircle() {

	}


	Vector3f getCentrePoint() {
		return centrePoint;
	}

	float getZDistance(float angle){

		float hypot = Maths::getMissingTriangleLength(angle, radius, radius);
		
		float antiAngle = Maths::getAngleInTriagnle(hypot, radius, radius);

		float newAngle = 90 - antiAngle;

		newAngle = newAngle / 57.295779513082320876798154814105;

		float zDistance = sin(newAngle) * hypot;

		

		zDistance = Maths::map(0, 20, 1, 4, zDistance);

		return zDistance;
	}

	float getXDistance(float ang, Vector2f size) {

		float angle = ang + ang;

		float hypot = Maths::getMissingTriangleLength(angle, radius, radius);

		float antiAngle = Maths::getAngleInTriagnle(hypot, radius, radius);

		float newAngle = 90 - antiAngle;

		newAngle = newAngle / 57.295779513082320876798154814105;

		float xDistance = cos(newAngle);

		//xDistance = xDistance * 57.295779513082320876798154814105;

		xDistance = xDistance * hypot;

		//xDistance = Maths::map(0, 20, 0, radius, xDistance);




		if (ang > 180) {
			xDistance = -xDistance;
		}

		xDistance += (centrePoint.x - (size.x/2));

		return xDistance;
	}

	/*float getYDistance(float ang) {

		float angle = ang + 0;

		float hypot = Maths::getMissingTriangleLength(angle, radius, radius);

		float antiAngle = Maths::getAngleInTriagnle(hypot, radius, radius);

		float newAngle = 90 - antiAngle;

		newAngle = newAngle / 57.295779513082320876798154814105;

		float yDistance = sin(newAngle);

		//xDistance = xDistance * 57.295779513082320876798154814105;

		yDistance = yDistance * hypot;

		yDistance = Maths::map(0, maxYDist, -radius, radius, yDistance);

		yDistance += centrePoint.y / 2;



		return yDistance;
	}*/
};