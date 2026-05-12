/*This entire class' responsibility is to handly the render logic.
Essentially, it interprets the current state of the game to figure out exactly what it should show the player.*/

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
#include "Object.cpp"

#pragma once
using namespace sf;
using namespace std;


class renderer{

	shared_ptr<RenderWindow> w;
	float ratio;

	float resolutionFactor;

	int defaultResolution = 1080;

	bool fullscreen;

public:

	virtual ~renderer() {

	}

public: 
	renderer(shared_ptr<RenderWindow> wi, bool fullscreen) {
		this->w = wi;
		ratio = getRatio();

		resolutionFactor = float(wi->getSize().y) / float(defaultResolution);
		this->fullscreen = fullscreen;

		
	}

	bool getFullscreen() {
		return fullscreen;
	}

	void setSize(Vector2u size) {
		//delete w;
		w->close();
		
		if (fullscreen) {
			w = shared_ptr<RenderWindow>(new RenderWindow(VideoMode(size.x, size.y), "Executable", Style::Fullscreen));
		}
		else {
			w = shared_ptr<RenderWindow>(new RenderWindow(VideoMode(size.x, size.y), "Executable", Style::Default));
		}
		resolutionFactor = float(w->getSize().y) / float(defaultResolution);
	}

	void setFullscreen(bool full) {
		fullscreen = full;
		setSize(w->getSize());
	}

private:


	void resolutionFix(shared_ptr<UISprite> sprite) {

		int windowS = w->getSize().y;

		//float resolutionFactor = float(windowS) / float(defaultResolution);


		Sprite* s = sprite->getSprite();

		s->setScale(sprite->getSprite()->getScale() * resolutionFactor);

		s->setPosition(sprite->getSprite()->getPosition() * resolutionFactor);

	}

	void resolutionCorrection(shared_ptr<UISprite> sprite) {
		Sprite* s = sprite->getSprite();

		s->setScale(sprite->getSprite()->getScale() / resolutionFactor);

		s->setPosition(sprite->getSprite()->getPosition() / resolutionFactor);
	}

	void resolutionFix(shared_ptr<UIHitbox> hit) {
	}
	void resolutionFix(shared_ptr<objectHitbox> hit) {
	}

	void resolutionFix(shared_ptr<RectangleShape> rect) {
		int windowS = w->getSize().y;

		//float resolutionFactor = float(windowS) / float(defaultResolution);
		rect->setScale(rect->getScale() * resolutionFactor);
		rect->setPosition(rect->getPosition() * resolutionFactor);
	}

	void rectCorrection(shared_ptr<RectangleShape> rect) {
		int windowS = w->getSize().y;

		//float resolutionFactor = float(windowS) / float(defaultResolution);
		rect->setScale(rect->getScale() / resolutionFactor);
		rect->setPosition(rect->getPosition() / resolutionFactor);
	}

	void resolutionFix(shared_ptr<Text> t) {
		
		int windowS = w->getSize().y;

		//float resolutionFactor = float(windowS) / float(defaultResolution);


		t->setScale(t->getScale() * resolutionFactor);
		t->setPosition(t->getPosition() * resolutionFactor);
	}

	void resolutionFix(shared_ptr<text> te) {

		int windowS = w->getSize().y;

		//float resolutionFactor = float(windowS) / float(defaultResolution);

		shared_ptr<Text> t = te->getRender();

		t->setScale(Vector2f(1 * resolutionFactor, 1 * resolutionFactor));
		t->setPosition(te->getPosition() * resolutionFactor);
	}

	void textCorrection(shared_ptr<Text> t) {
		int windowS = w->getSize().y;

		//float resolutionFactor = float(windowS) / float(defaultResolution);

		t->setScale(t->getScale() / resolutionFactor);
		t->setPosition(t->getPosition() / resolutionFactor);
	}

public:

	void textDisplay(list<shared_ptr<text>> textList){
		//list<shared_ptr<text>> temp = resolutionTextScale(textList);
		for (shared_ptr<text> text : textList) {
			resolutionFix(text);
			w->draw(*text->getRender());
		}
	}

	void textDisplay(shared_ptr<text> text) {
		//list<shared_ptr<text>> temp = resolutionTextScale(textList);
		resolutionFix(text);
		w->draw(*text->getRender());
		
		
	}
	void textDisplay(shared_ptr<Text> text) {
		//list<shared_ptr<text>> temp = resolutionTextScale(textList);
		resolutionFix(text);
		w->draw(*text);
		textCorrection(text);
	}

	void textDisplay(vector<shared_ptr<Text>> text) {

		for (shared_ptr<Text> t : text) {
			textDisplay(t);
		}
	}

	void objectAccess(shared_ptr<object> object, shared_ptr<camera> cam) {
		if (object->getDisplay()) {
			objectDisplay(object->getSprite(), cam);
		}
	}

	void rectDisplay(shared_ptr<RectangleShape> rect) {
		resolutionFix(rect);
		w->draw(*rect);
		rectCorrection(rect);
	}

	void objectDisplay(shared_ptr<objectSprite> object, shared_ptr<camera> cam) {
	
		objectSetup(object, cam);
		resolutionFix(object);
		Sprite* s = object->getSprite();

		w->draw(*s);
		resolutionCorrection(object);

		lightingDisplay(object, cam);
	}

	void lightingDisplay(shared_ptr<objectSprite> object, shared_ptr<camera> cam) {
		list<shared_ptr<RectangleShape>> pix = object->getPixels();

		for (shared_ptr<RectangleShape> rect : pix) {
			shared_ptr<RectangleShape> temp = shared_ptr<RectangleShape>(new RectangleShape());
			temp->setPosition(rect->getPosition() + object->getCameraPosition());
			temp->setSize(rect->getSize());
			temp->setFillColor(rect->getFillColor());
			w->draw(*temp);
		}
	}

	void screenLightingDisplay(list <shared_ptr<RectangleShape>> rectangles) {
		for (shared_ptr<RectangleShape> rect : rectangles) {
			shared_ptr<RectangleShape> temp = shared_ptr<RectangleShape>(new RectangleShape());
			temp->setPosition(rect->getPosition());
			temp->setSize(rect->getSize());
			temp->setFillColor(rect->getFillColor());
			w->draw(*temp);
		}
	}

	void objectSetup(shared_ptr<objectSprite> object, shared_ptr<camera> cam) {
		Vector2f cPosition = cam->getPosition();
		float zoom = cam->getZoom();
		Vector2f sPosition = object->getPosition();
		Vector2f newP = sPosition - cPosition;
		Vector2f plusZ = Vector2f((newP.x * pow(object->getZ(), -1)), (newP.y * pow(object->getZ(), -1)));

		Vector2f newPos = plusZ + object->getVisualOffset();

		object->setCameraScale(Vector2f(object->getScale().x * zoom, object->getScale().y * zoom));

		object->setCameraPosition(Vector2f((newPos.x * zoom), (newPos.y * zoom)));


	}

	void bObjectDisplay(shared_ptr<objectSprite> object, shared_ptr<camera> cam) {

		

		bObjectCalc(object, cam);
		Sprite* s = object->getSprite();

		resolutionFix(object);
		w->draw(*s);
		resolutionCorrection(object);
	}

	void bObjectDisplay(shared_ptr<objectSprite> object, bool display, shared_ptr<camera> cam) {

		bObjectCalc(object, cam);

		if (display) {
			
			Sprite* s = object->getSprite();

			resolutionFix(object);
			w->draw(*s);
			resolutionCorrection(object);
		}
	}

	void bObjectCalc(shared_ptr<objectSprite> object, shared_ptr<camera> cam) {
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

	void bObjectDisplay(list<shared_ptr<objectSprite>> objects, shared_ptr<camera> cam) {
		for (shared_ptr<objectSprite> ob : objects) {
			bObjectDisplay(ob, cam);
		}
	}


	void objectDisplay(list<shared_ptr<objectSprite>> objectList, shared_ptr<camera> cam) {
		 //This check if the sprite the camera is following has been flipped
		
		//resolutionObjectScale(objectList);
		//if (cam->getFollowing() != NULL) {
		//	bool flipped = cam->checkFlip();
		//}

		//This number is how much the camera needs to be moved if said sprite is flipped
		//float flipAmount = cam->getFAmount();

		for (shared_ptr<objectSprite> sprite : objectList) {
			if (sprite != NULL) {
				objectDisplay(sprite, cam);
			}
		}
	}

	void UIDisplay(list<shared_ptr<UISprite>> spriteList) {
		//resolutionScale(spriteList);
		for (shared_ptr<UISprite> sprite : spriteList) {


			UIDisplay(sprite);
		}
	}

	void UIDisplay(shared_ptr<UISprite> sprite) {
		//resolutionScale(spriteList);


		string id = typeid(objectSprite).name();

		Sprite* s = sprite->getSprite();

		s->setPosition(sprite->getCameraPosition() + sprite->getVisualOffset());

		resolutionFix(sprite);
		w->draw(*s);
		resolutionCorrection(sprite);
		
	}

	void hitboxDisplay(list<shared_ptr<objectHitbox>> hitList) {


		for (shared_ptr<objectHitbox> hit : hitList) {
			hitboxDisplay(hit);
		}
	}

	void objectHitboxDisplay(shared_ptr<objectHitbox> hit, shared_ptr<camera> cam) {

		objectHitboxSetup(hit, cam);
		hitboxDisplay(hit);

	}

	void hitboxDisplay(shared_ptr<objectHitbox> hit) {


		//if (hit->isVisible()) {
		sf::RectangleShape rectangle(Vector2f(hit->getSize().x * ratio, hit->getSize().y * ratio));
		rectangle.setPosition(hit->getCameraPos());
		rectangle.setFillColor(Colour(0, 255, 0, 150).getColour());
		
		resolutionFix(hit);
		w->draw(rectangle);
		//}
		
	}

	void hitboxDisplay(shared_ptr<objectHitbox> hit, float zoom) {


		//if (hit->isVisible()) {
		sf::RectangleShape rectangle(Vector2f(hit->getSize().x * ratio, hit->getSize().y * ratio));
		rectangle.setPosition(hit->getCameraPos());
		rectangle.setFillColor(Colour(0, 255, 0, 255).getColour());
		rectangle.setScale(Vector2f(zoom, zoom));
		resolutionFix(hit);
		w->draw(rectangle);
		//}

	}

	void hitboxDisplay(shared_ptr<objectHitbox> hit, Colour col) {


		//if (hit->isVisible()) {
		sf::RectangleShape rectangle(Vector2f(hit->getSize().x * ratio, hit->getSize().y * ratio));
		rectangle.setPosition(hit->getCameraPos());
		rectangle.setFillColor(col.getColour());
		resolutionFix(hit);
		w->draw(rectangle);
		//}

	}

	void hitboxDisplay(shared_ptr<objectHitbox> hit, Colour col, float zoom) {


		//if (hit->isVisible()) {
		sf::RectangleShape rectangle(Vector2f(hit->getSize().x * ratio, hit->getSize().y * ratio));
		rectangle.setPosition(hit->getCameraPos());
		rectangle.setFillColor(col.getColour());
		rectangle.setScale(Vector2f(zoom, zoom));
		resolutionFix(hit);
		w->draw(rectangle);
		//}

	}

	void objectHitboxSetup(list<shared_ptr<objectHitbox>> hitList, shared_ptr<camera> cam) {

		
		for (shared_ptr<objectHitbox> hit : hitList) {
			objectHitboxSetup(hit, cam);

		}
	}

	void objectHitboxSetup(shared_ptr<objectHitbox> hit, shared_ptr<camera> cam) {

		Vector2f camPosition = cam->getPosition();
		
		hit->setCameraPos(Vector2f((hit->getPosition().x - camPosition.x) * cam->getZoom(), (hit->getPosition().y - camPosition.y) * cam->getZoom()));

		hitboxDisplay(hit, cam->getZoom());
	}

	void objectHitboxSetup(shared_ptr<objectHitbox> hit, shared_ptr<camera> cam, Colour col) {

		Vector2f camPosition = cam->getPosition();

		hit->setCameraPos(Vector2f((hit->getPosition().x - camPosition.x) * cam->getZoom(), (hit->getPosition().y - camPosition.y) * cam->getZoom()));

		hitboxDisplay(hit, col, cam->getZoom());
	}


	float getRatio() {
		float currentRes = w->getSize().y;
		return currentRes / 1080;
	}

	/*void resolutionObjectScale(list<shared_ptr<objectSprite>> sprites) {
		list<shared_ptr<UISprite>> temp;
		for (shared_ptr<UISprite> s : sprites) {
			temp.push_back(s);
		}
		resolutionScale(temp);
	}

	void resolutionScale(list<shared_ptr<UISprite>> sprites) {
		
		for (shared_ptr<UISprite> sprite : sprites) {
			Vector2f currentS = sprite->getScale();
			Sprite* s = sprite->getSprite();
			
			s->setScale((Vector2f(currentS.x * ratio, currentS.y * ratio)));
		}
	}

	list<shared_ptr<text>> resolutionTextScale(list<shared_ptr<text>> texts) {
		list<shared_ptr<text>> temp;
		for (shared_ptr<text> t : texts) {
			temp.push_back(t);
			float currentS = t->getSize();
			t->setRenderSize(currentS);
		}
	}*/

	shared_ptr<RenderWindow> getWindow() {
		return w;
	}
};
