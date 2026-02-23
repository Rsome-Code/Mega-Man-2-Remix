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
	list<shared_ptr<Text>> text;

	list<shared_ptr<Text>>::iterator current;

public:
	TextBox(list<shared_ptr<Text>> texts) {
		text = texts;
		current = text.begin();
	}

	virtual ~TextBox() {

	}



	void nextText() {
		current = next(current);
	}

	shared_ptr<Text> getCurrent() {
		return *current;
	}

	virtual void transitionTo() = 0;

	virtual void transitionFrom() = 0;


};