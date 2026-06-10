#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <iterator>
#include <list>
#pragma once

using namespace sf;
using namespace std;

class Maths {

public:
	static float getDistance(Vector2f pos1, Vector2f pos2) {

		float xDistance = getDistance(pos1.x, pos2.x);
		float yDistance = getDistance(pos1.y, pos2.y);

		float total = sqrt(pow(xDistance, 2) + pow(yDistance, 2));
		return total;
	}

	static float getDistance(float pos1, float pos2) {
		float distance = pos1 - pos2;

		distance = fabs(distance);
		return distance;
	}

	static float map(float startMin, float startMax, float endMin, float endMax, float number) {
		if (endMin == endMax) {
			return endMin;
		}
		float startDiff = startMax - startMin;
		float endDiff = endMax - endMin;
		float numDiff = number - startMin;
		float decimal = numDiff / startDiff;

		float endNumDiff = decimal * endDiff;
		return (endNumDiff + endMin);
		
	}

	static float getAngle(Vector2f pos, Vector2f pos1) {
		float xDiff = fabs(pos.x - pos1.x);
		float yDiff = fabs(pos.y - pos1.y);

		float angle = (atan(yDiff / xDiff)) * 57.295779513082320876798154814105;

		if (pos1.x > pos.x) {
			
			if (pos1.y < pos.y) {
				angle -= (angle * 2);
				angle += 0;
			}
		}
		if (pos1.x < pos.x) {
			if (pos1.y < pos.y) {
				angle += 180;
			}
			else if (pos1.y > pos.y) {
				angle -= (angle * 2);
				angle += 180;
			}
		}

		return angle;
	}


	//a,b,c are for lengths
	static float getMissingTriangleLength(float angle, float a, float b) {
		float c;
		//c = sqrt(cos(angle) * (2 * b * a) - pow(b, 2) - pow(a, 2));

		//c = cos(angle) * (2 * b * a) - pow(b, 2) - pow(a, 2);

		float ang = angle / 57.295779513082320876798154814105;

		c = cos(ang);

		c = c * (2 * b * a);

		c = c - pow(b, 2);
		c = c - pow(a, 2);

		c = sqrt(-c);

		//c = c;

		return c;
	}

	static float getAngleInTriagnle(float a, float b, float oppositeLen) {

		double ang;

		//ang = acos((pow(a, 2) + pow(b, 2) - oppositeLen) / 2 * a * b);

		ang = pow(a, 2) + pow(b, 2) - pow(oppositeLen, 2);

		//ang = ang / (2 * a * b);

		float temp = 2 * a * b;

		if (temp != 0) {
			ang = ang / temp;
		}
		else {
			ang = ang / 0.0000000000000000000000000000000000000000000000000001;
		}

		ang = ang / 57.295779513082320876798154814105;

		ang = acos(ang);


		ang = ang * 57.295779513082320876798154814105;

		return ang;

	}


	static float getEndPoint(Vector2f startPos, float angle, float distance) {
		 
	}

	static bool inProximity(Vector2f pos, Vector2f pos1, float range) {

		bool xTrue = false;

		if (pos.x < pos1.x + range) {
			if (pos.x > pos1.x - range) {
				xTrue = true;
			}
		}

		bool yTrue = false;

		if (pos.y < pos1.y + range) {
			if (pos.y > pos1.y - range) {
				yTrue = true;
			}
		}

		return (xTrue && yTrue);

	}

};