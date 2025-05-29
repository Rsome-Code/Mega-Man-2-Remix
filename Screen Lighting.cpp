#include "light source.cpp"
#include <vector>
#include "Maths.cpp"
#include "camera.cpp"

#include <list>
#pragma once

using namespace sf;
using namespace std;

class ScreenLighting {
	list<RectangleShape*> rectangles;
	float defaultTransparency;

public:
	ScreenLighting() {
		for (int y = 0; y < 1080/4; y++) {
			for (int x = 0; x < 1920/4; x++) {
				RectangleShape* temp = new RectangleShape(Vector2f(4,4));
				temp->setPosition(Vector2f(x*4, y*4));
				rectangles.push_back(temp);
			}
		}

		defaultTransparency = 200;
	}

	list<RectangleShape*> getRectangles() {
		return rectangles;
	}
	void setDefaultTransparency(float t) {
		defaultTransparency = t;
	}
	float getDefaultTransparency() {
		return defaultTransparency;
	}


	void lightingCheck(LightSource* light, camera* cam) {
		bool done = false;
		for (RectangleShape* pixel : rectangles) {
			Vector2f check = pixelPosition(pixel);
			float distance = Maths::getDistance(pixelPosition(pixel), light->getPosition());
			if (!done) {
				done = true;
			}

			Color colour = light->getColour();
			float red = colour.r;
			float green = colour.g;
			float blue = colour.b;

			if (distance < light->getRange()) {


				float percent = Maths::map(0, light->getRange(), 0, 100, light->getRange() - distance);

				float newRed = Maths::map(0, 100, 0, red, percent);
				float newGreen = Maths::map(0, 100, 0, green, percent);
				float newBlue = Maths::map(0, 100, 0, blue, percent);
				float newTransparency = 255 - Maths::map(0, 100, 0, light->getBrightness(), percent);
				if (newTransparency > defaultTransparency) {
					newTransparency = defaultTransparency;
				}
				pixel->setFillColor(Color(newRed, newGreen, newBlue, newTransparency));
			}
			else {
				pixel->setFillColor(Color(0, 0, 0, defaultTransparency));
			}


		}
	}

	Vector2f pixelPosition(RectangleShape* pixel) {
		Vector2f relPosition = pixel->getPosition();
		Vector2f camPos = relPosition;
		return (camPos);
	}
};