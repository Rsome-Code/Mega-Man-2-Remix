#include "menu selection.cpp"
#include "UI Sprite.cpp"
#pragma once

class menuTab {
	list<shared_ptr<menuSelect>> optionList;
	list<shared_ptr<menuSelect>>::iterator optionI;

	shared_ptr<UISprite> tab;
	Vector2f position;

public:
	virtual ~menuTab() {

	}

	menuTab(Vector2f pos) {
		shared_ptr<Texture> tabT = shared_ptr<Texture> (new Texture);
		tabT->loadFromFile("Assets\\Tab.png");
		position = pos;
		tab = shared_ptr<UISprite>(new UISprite("UI", tabT, Vector2i(0, 0), Vector2i(414, 1080), pos, Vector2f(1, 1)));
	}

	

};