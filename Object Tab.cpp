#include "Tile.cpp"
#include "UI Sprite.cpp"
#include "Object Sprite.cpp"
#include "Time.cpp"
#include "Render Logic.cpp"
#include "Menu Selection.cpp"
#include "Object Select.cpp"
#include "Right tile.cpp"
#include "Left tile.cpp"
#include "Ceiling tile.cpp"
#include "Top Tile.cpp"
#include "Solid Tile.cpp"
#include "ladder tile.cpp"
#include "top ladder tile.cpp"
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "GObject.cpp"
#include "Mouse.cpp"

using namespace std;
using namespace sf;

class Tab {
	shared_ptr<UISprite> background;
	list<shared_ptr<GameObject>> objects;
	list<shared_ptr<UISprite>> obSprites;
	list<shared_ptr<GameObject>>::iterator obIt;
	list<shared_ptr<UISprite>> spriteIt;
	shared_ptr<GameObject> selectedObject = NULL;

public:
	Tab(list<shared_ptr<GameObject>> o, Vector2f position) {
		objects = o;
		shared_ptr<Texture> t = shared_ptr<Texture> (new Texture());
		t->loadFromFile("Assets/tab.png");
		background = shared_ptr<UISprite>(new UISprite("BG", t, Vector2i(0, 0), Vector2i(414, 1080), position));

		int plusX = 0;
		int plusY = 0;
		list<shared_ptr<GameObject>>::iterator i = objects.begin();
		int greatestY = 0;
		bool nextLine = false;
		bool initial = true;

		for (shared_ptr<object> o : objects) {
			shared_ptr<UISprite> s = o->getSprite();
			if (s->getSize().y > greatestY) {
				greatestY = s->getSize().y;
			}

			if (!initial) {
				shared_ptr<object> ob = *prev(i);
				if (ob->getSprite()->getSize().x + plusX > 414) {
					nextLine = true;

					plusX = 0;
					plusY = plusY + greatestY + 5;
					nextLine = false;
				}
			}
			else {
				initial = false;
			}
			

			s->setCameraPosition(Vector2f(background->getCameraPosition().x + 10 + plusX, background->getCameraPosition().y + 10 + plusY));
			
			obSprites.push_back(o->getSprite());

			if (!nextLine) {
				plusX = plusX + s->getSize().x + 5;
			}

			i++;
			
		}

		
	}

	bool checkActive(Vector2i mousePos) {
		IntRect debug = IntRect(Vector2i(background->getCameraPosition()), Vector2i(background->getSize()));
		if (mousePos.x > background->getCameraPosition().x && mousePos.x < (background->getSize().x + background->getCameraPosition().x)) {
			if (mousePos.y > background->getCameraPosition().y && mousePos.y < (background->getSize().y + background->getCameraPosition().y)) {
				return true;
			}
		}
		return false;
	}

	void checkMouse(Vector2i mousePos) {
		obIt = objects.begin();
		for (shared_ptr<UISprite> sprite : obSprites) {
			if (checkCollision(mousePos, sprite->getCameraPosition(), sprite->getSize())) {
				selectedObject = *obIt;
			}
			else {
				obIt = next(obIt);
			}
		}
	}

	shared_ptr<GameObject> getSelected() {
		return selectedObject;
	}

	bool checkCollision(Vector2i mouse, Vector2f sPos, Vector2f sSize) {
		if (mouse.x > sPos.x && mouse.x < sPos.x + sSize.x) {
			if (mouse.y > sPos.y && mouse.y < sPos.y + sSize.y) {
				return true;
			}
		}
		return false;
	}

	list<shared_ptr<UISprite>> getSprites() {
		list<shared_ptr<UISprite>> temp;
		temp = obSprites;
		temp.push_front(background);
		return temp;
	}
};