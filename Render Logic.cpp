/*This entire class' responsibility is to handly the render logic.
Essentially, it interpret's the current state of the game to figure out exactly what it should show the player.*/

#include <SFML/Graphics.hpp>
#include <typeinfo>
#include <windows.h>
#include <list>
#include "Object Sprite.cpp"
#include "Camera.cpp"
#include "UI Sprite.cpp"
#include "UI Hitbox.cpp"
#include "Object Hitbox.cpp"
#include "text.cpp"
#include <iostream>
#include <list>
#include "Object.cpp"

#pragma once
using namespace sf;
using namespace std;


class renderer{

	RenderWindow* w;
	float ratio;

public: 
	renderer(RenderWindow* wi) {
		this->w = wi;
		ratio = getRatio();

	}


public:
	void textDisplay(list<text*> textList){
		//list<text*> temp = resolutionTextScale(textList);
		for (text* text : textList) {
			w->draw(*text->getRender());
		}
	}

	void objectAccess(object* object, camera* cam) {
		if (object->getDisplay()) {
			objectDisplay(object->getSprite(), cam);
		}
	}



	void objectDisplay(objectSprite* object, camera* cam) {
		objectSetup(object, cam);

		Sprite* s = object->getSprite();

		w->draw(*s);

		lightingDisplay(object);
	}

	void lightingDisplay(objectSprite* object) {
		list<RectangleShape*> pix = object->getPixels();

		for (RectangleShape* rect : pix) {
			RectangleShape* temp = new RectangleShape();
			temp->setPosition(rect->getPosition() + object->getCameraPosition());
			temp->setSize(rect->getSize());
			temp->setFillColor(rect->getFillColor());
			w->draw(*temp);
		}
	}

	void screenLightingDisplay(list<RectangleShape*> rectangles) {
		for (RectangleShape* rect : rectangles) {
			w->draw(*rect);
		}
		cout << "ok!";
	}

	void objectSetup(objectSprite* object, camera* cam) {
		Vector2f cPosition = cam->getPosition();
		float zoom = cam->getZoom();
		Vector2f sPosition = object->getPosition();
		Vector2f newP = sPosition - cPosition;
		Vector2f plusZ = Vector2f((newP.x * pow(object->getZ(), -1)), (newP.y * pow(object->getZ(), -1)));

		Vector2f newPos = plusZ + object->getVisualOffset();

		object->setCameraScale(Vector2f(object->getScale().x * zoom, object->getScale().y * zoom));

		object->setCameraPosition(Vector2f((newPos.x * zoom), (newPos.y * zoom)));
	}

	void bObjectDisplay(objectSprite* object, camera* cam) {

		bObjectCalc(object, cam);
		Sprite* s = object->getSprite();

		w->draw(*s);
	}

	void bObjectCalc(objectSprite* object, camera* cam) {
		float mult = object->getZ() - 1;
		//Vector2f cPosition = Vector2f(cam->getPosition().x - (960 * mult), cam->getPosition().y - (540* mult));
		//Vector2f cPosition = Vector2f(cam->getPosition().x - (960 * mult), cam->getPosition().y);
		float zoom = cam->getZoom();
		Vector2f cPosition = Vector2f(cam->getPosition().x - ((960 * mult) * (pow(zoom, -1))), cam->getPosition().y - ((540 * mult)) * pow(zoom, -1));
		Vector2f sPosition = object->getPosition();
		Vector2f newP = sPosition - cPosition;
		Vector2f plusZ = Vector2f((newP.x * pow(object->getZ(), -1)), (newP.y * pow(object->getZ(), -1)));
		//Vector2f plusZ = Vector2f((newP.x * pow(object->getZ(), -1)), newP.y);

		Vector2f newPos = plusZ + object->getVisualOffset();

		object->setCameraScale(Vector2f((object->getScale().x * zoom) * pow(object->getZ(), -1), (object->getScale().y * zoom) * pow(object->getZ(), -1)));

		object->setCameraPosition(Vector2f((newPos.x * zoom), (newPos.y * zoom)));

	}

	void bObjectDisplay(list<objectSprite*> objects, camera* cam) {
		for (objectSprite* ob : objects) {
			bObjectDisplay(ob, cam);
		}
	}


	void objectDisplay(list<objectSprite*> objectList, camera* cam) {
		 //This check if the sprite the camera is following has been flipped
		
		//resolutionObjectScale(objectList);
		//if (cam->getFollowing() != NULL) {
		//	bool flipped = cam->checkFlip();
		//}

		//This number is how much the camera needs to be moved if said sprite is flipped
		//float flipAmount = cam->getFAmount();

		for (objectSprite* sprite : objectList) {
			objectDisplay(sprite, cam);

		}
	}

	void UIDisplay(list<UISprite*> spriteList) {
		//resolutionScale(spriteList);
		for (UISprite* sprite : spriteList) {


			string id = typeid(objectSprite).name();

			Sprite* s = sprite->getSprite();

			w->draw(*s);
		}
	}

	void UIDisplay(UISprite* sprite) {
		//resolutionScale(spriteList);


		string id = typeid(objectSprite).name();

		Sprite* s = sprite->getSprite();

		w->draw(*s);
		
	}

	void hitboxDisplay(list<UIHitbox*> hitList) {


		for (UIHitbox* hit : hitList) {
			hitboxDisplay(hit);
		}
	}

	void hitboxDisplay(UIHitbox* hit) {


		if (hit->isVisible()) {
			sf::RectangleShape rectangle(Vector2f(hit->getSize().x * ratio, hit->getSize().y * ratio));
			rectangle.setPosition(hit->getCameraPos());
			rectangle.setFillColor(sf::Color(0, 255, 0, 255));
			w->draw(rectangle);
		}
		
	}

	void objectHitboxSetup(list<objectHitbox*> hitList, camera* cam) {

		Vector2f camPosition = cam->getPosition();
		for (objectHitbox* hit : hitList) {
			objectHitboxSetup(hit, cam);

		}
	}

	void objectHitboxSetup(objectHitbox* hit, camera* cam) {

		Vector2f camPosition = cam->getPosition();
		
		hit->setCameraPos(Vector2f((hit->getPosition().x - camPosition.x) * cam->getZoom(), (hit->getPosition().y - camPosition.y) * cam->getZoom()));

		hitboxDisplay(hit);
	}




	float getRatio() {
		float currentRes = w->getSize().y;
		return currentRes / 1080;
	}

	/*void resolutionObjectScale(list<objectSprite*> sprites) {
		list<UISprite*> temp;
		for (UISprite* s : sprites) {
			temp.push_back(s);
		}
		resolutionScale(temp);
	}

	void resolutionScale(list<UISprite*> sprites) {
		
		for (UISprite* sprite : sprites) {
			Vector2f currentS = sprite->getScale();
			Sprite* s = sprite->getSprite();
			
			s->setScale((Vector2f(currentS.x * ratio, currentS.y * ratio)));
		}
	}

	list<text*> resolutionTextScale(list<text*> texts) {
		list<text*> temp;
		for (text* t : texts) {
			temp.push_back(t);
			float currentS = t->getSize();
			t->setRenderSize(currentS);
		}
	}*/

	RenderWindow* getWindow() {
		return w;
	}
};
