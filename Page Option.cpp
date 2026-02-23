#include "pause option.cpp"
#pragma once

class PageOption : public Option {
	shared_ptr<UISprite> word;

public:
	PageOption(shared_ptr<Texture> t, Vector2f pos) {
		position = pos;

		icon = shared_ptr<UISprite>(new UISprite("Opt", t, IntRect(1675, 11, 8, 8), pos, Vector2f(4, 4)));
		word = shared_ptr<UISprite>(new UISprite("Opt", t, IntRect(1684, 11, 29, 8), Vector2f(pos.x + (13*4), pos.y), Vector2f(4, 4)));
	}

	list<shared_ptr<UISprite>> getSprites() {
		list<shared_ptr<UISprite>> temp;
		if (displayIcon) {
			temp.push_back(icon);
		}
		temp.push_back(word);
		
		return temp;
	}

	shared_ptr<UISprite> getWordSprite() {
		return word;
	}
};