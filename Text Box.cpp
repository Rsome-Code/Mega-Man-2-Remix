#include <chrono>
#include <thread>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <Xinput.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <list>

#pragma once
#pragma comment(lib,"winmm.lib")
using namespace std;

using namespace sf;

class TextBox {
	list<Text*> text;

	list<Text*>::iterator current;

	TextBox(list<Text*> texts) {
		text = texts;
		current = text.begin();
	}

public:
	void nextText() {
		current = next(current);
	}

	Text* getCurrent() {
		return *current;
	}

	virtual void transitionTo() = 0;

	virtual void transitionFrom() = 0;


};