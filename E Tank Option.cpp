#include "pause option.cpp"
#pragma once

class ETankOption : public Option {
	list<UISprite*> tankSprites;
	int eTanks;
	Texture* texture;


public:
	ETankOption(Texture* t, Vector2f pos, int eTankNum) {
		position = pos;
		texture = t;
		update(eTankNum);
		icon = new UISprite("Ion", t, IntRect(73, 56, 8, 8), pos, Vector2f(4, 4));
	}

	void update(int eTankNum) {
		tankSprites.clear();
		eTanks = eTankNum;
		for (int i = 0; i < eTanks; i++) {
			UISprite* temp = new UISprite("Symbol", texture, IntRect(63, 56, 8, 8), Vector2f(((position.x + (16*4)) + (i * 16)*4), position.y), Vector2f(4, 4));
			tankSprites.push_back(temp);
		}
	}

	list<UISprite*> getSprites() {
		list<UISprite*> temp = tankSprites;
		if (displayIcon) {
			temp.push_back(icon);
		}
		

		return temp;

	}
};