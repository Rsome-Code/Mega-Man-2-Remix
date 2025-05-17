//This stores the rectangles in a sprite sheet to be used for the stored sprite.
//This will not determine timing, only the frames themselves.
//The animation must be present on the spritesheet loaded into the sprite being animated.

#include <SFML/Graphics.hpp>
#include "Object Sprite.cpp"
#include <list>
using namespace std;
using namespace sf;
#pragma once

class animation {
	UISprite* sprite;
	list<IntRect> frameIndexes;
	//bool repeat;
	list<IntRect>::iterator current;
	int size;
	bool display = true;
	list<Vector2f> offsetList;
	list<Vector2f>::iterator offsetI;
	int currentIndex = 0;
	int assoSprites = 1;


public: 
	animation(list<IntRect> indexes, UISprite* sprite) {
	this->frameIndexes = indexes;
	this->sprite = sprite;
	//this->repeat = repeat;
	this->current = frameIndexes.begin();
	
	size = indexes.size();

	for (IntRect f : frameIndexes) {
		offsetList.push_back(Vector2f(0, 0));
	}
	offsetI = offsetList.begin();

	}

public:
	animation() {
		cout << "AY";
	}

	void setOffsetList(list<Vector2f> l) {
		offsetList = l;
		offsetI = offsetList.begin();
	}

	int getCurrentIndex() {
		return currentIndex;
	}

public:
	void setFrame(IntRect rect, Vector2f off) {
		sprite->applyRect(rect);
		sprite->setVisualOffset(off);
	}
	void setFrame() {
		sprite->applyRect(*current);
		sprite->setVisualOffset(*offsetI);
	}

	void thisFrame() {
		if (display) {
			if (current != frameIndexes.end()) {
				this->setFrame(*current, *offsetI);
			}
			else {
				this->setFrame(*prev(current), *prev(offsetI));
			}
		}
	}
	void prevFrame() {
		if (display) {
			if (current != frameIndexes.begin()) {
				this->setFrame(*prev(current), *prev(offsetI));
			}
			else {
				this->setFrame(*prev(frameIndexes.end()), *prev(offsetList.end()));
			}
		}
	}
	void displayNextFrame() {
		if (next(current) == frameIndexes.end() || current == frameIndexes.end()) {
			this->setFrame(*next(current), *next(offsetI));
		}
		else {
			this->setFrame(*frameIndexes.begin(), *offsetList.begin());
		}
	}

	//Returns true when animation finishes/loops
	bool nextFrame(bool loop) {
		if (current == frameIndexes.end()) {
			if (loop) {
				reset();
				if (display) {
					this->setFrame(*current, *offsetI);
				}

				this->current++;
				offsetI++;
				currentIndex++;

			
			}
			return true;
		}
		else {
			if (display) {
				this->setFrame(*current, *offsetI);
			}

			this->current++;
			offsetI++;
			currentIndex++;

			

			return false;
		}
	}


	bool isFinishedLoop() {
		if (current == next(frameIndexes.begin())) {
			return true;
		}
		return false;
	}
	bool isFinishedNotLoop() {
		if (current == frameIndexes.end()) {
			return true;
		}
		return false;
	}

public:void reset() {
	current = frameIndexes.begin();
	offsetI = offsetList.begin();
	currentIndex = 0;
}
	  void swapAll() {
		  int change = sprite->getTexture()->getSize().x;
		 

		  list<IntRect>::iterator j = frameIndexes.begin();
		  //frameIndexes.clear();
		  for (IntRect FI : frameIndexes) {
			  int changeSize = FI.getSize().x;

			  *j = IntRect(Vector2i((change - FI.getPosition().x) - changeSize, FI.getPosition().y) , FI.getSize());
			  j = next(j);
		  }
	  }

	  void setDisplay(bool b) {
		  display = b;
	  }

	  void setSprite(UISprite* s) {
		  sprite = s;
	  }
	  int getFrameSize() {
		  return frameIndexes.size();
	  }

	  list<IntRect>::iterator getCurrent() {
		  return current;
	  }
	  list<Vector2f>::iterator getOffsetI(){
		  return offsetI;
	  }

};