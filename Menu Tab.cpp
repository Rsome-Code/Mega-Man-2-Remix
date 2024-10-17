#include "menu selection.cpp"
#include "UI Sprite.cpp"
#pragma once

class menuTab {
	list<menuSelect*> optionList;
	list<menuSelect*>::iterator optionI;

	UISprite* tab;
	Vector2f position;

	menuTab(Vector2f pos) {
		Texture* tabT = new Texture;
		tabT->loadFromFile("Assets\\Tab.png");
		position = pos;
		tab = new UISprite("UI", tabT, Vector2i(0, 0), Vector2i(414, 1080), pos, Vector2f(1, 1));
	}

	

};