#include "pause option.cpp"
#pragma once

class ETankOption : public Option {
	list<shared_ptr<UISprite>> tankSprites;
	int eTanks;
	shared_ptr<Texture> texture;


public:
	ETankOption(shared_ptr<Texture> t, Vector2f pos, int eTankNum) {
		position = pos;
		texture = t;
		update(eTankNum);
		icon = shared_ptr<UISprite>(new UISprite("Ion", t, IntRect(73, 56, 8, 8), pos, Vector2f(4, 4)));
		
	}

	String getName() {
		return "e tank";
	}

	void update(int eTankNum) {
		tankSprites.clear();
		eTanks = eTankNum;
		for (int i = 0; i < eTanks; i++) {
			shared_ptr<UISprite> temp = shared_ptr<UISprite>(new UISprite("Symbol", texture, IntRect(63, 56, 8, 8), Vector2f(((position.x + (16*4)) + (i * 16)*4), position.y), Vector2f(4, 4)));
			tankSprites.push_back(temp);
		}
	}

	list<shared_ptr<UISprite>> getSprites() {
		list<shared_ptr<UISprite>> temp = tankSprites;
		if (displayIcon) {
			temp.push_back(icon);
		}
		

		return temp;

	}
};