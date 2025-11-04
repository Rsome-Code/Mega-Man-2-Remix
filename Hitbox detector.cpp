#include "Object hitbox.cpp"
#pragma once
class hitboxDetect {
public:
	static bool hitboxDetection(objectHitbox* hit, objectHitbox* hit1) {
		if (hit->getPosition().x > hit1->getPosition().x + hit1->getSize().x) {

		}
		else if (hit->getPosition().x + hit->getSize().x < hit1->getPosition().x) {

		}
		else if (hit->getPosition().y > hit1->getPosition().y + hit1->getSize().y) {

		}
		else if (hit->getPosition().y + hit->getSize().y < hit1->getPosition().y) {

		}
		else {
			return true;
		}
		return false;
	
	}

	static bool hitboxDetection(Vector2f point, objectHitbox* hit1) {
		if (point.x > hit1->getPosition().x + hit1->getSize().x) {

		}
		else if (point.x < hit1->getPosition().x) {

		}
		else if (point.y > hit1->getPosition().y + hit1->getSize().y) {

		}
		else if (point.y < hit1->getPosition().y) {

		}
		else {
			return true;
		}
		return false;
	}

	static bool hitboxYDetection(Vector2f point, objectHitbox* hit1) {

		if (point.y > hit1->getPosition().y + hit1->getSize().y) {

		}
		else if (point.y < hit1->getPosition().y) {

		}
		else {
			return true;
		}
		return false;
	}

	static bool hitboxXDetection(Vector2f point, objectHitbox* hit1) {
		if (point.x > hit1->getPosition().x + hit1->getSize().x) {

		}
		else if (point.x < hit1->getPosition().x) {

		}

		else {
			return true;
		}
		return false;
	}
};