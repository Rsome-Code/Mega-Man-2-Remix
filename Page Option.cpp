#include "pause option.cpp"
#pragma once

class PageOption : public Option {
	UISprite* word;

public:
	PageOption(Texture* t, Vector2f pos) {
		position = pos;

		icon = new UISprite("Opt", t, IntRect(1675, 11, 8, 8), pos, Vector2f(4, 4));
		word = new UISprite("Opt", t, IntRect(1684, 11, 29, 8), Vector2f(pos.x + (13*4), pos.y), Vector2f(4, 4));
	}

	list<UISprite*> getSprites() {
		list<UISprite*> temp;
		if (displayIcon) {
			temp.push_back(icon);
		}
		temp.push_back(word);
		
		return temp;
	}
};