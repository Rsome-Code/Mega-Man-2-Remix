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

};