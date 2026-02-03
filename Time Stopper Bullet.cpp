#include "bullet.cpp"
#include "animation timer.cpp"
#include <random>
#pragma once

class TimeStopBullet:public bullet {

	vector<UISprite*> sprites;
	vector<animation*> anims;
	vector<animTimer*> timers;

	list<UISprite*> spriteList;



public:

	TimeStopBullet(Texture* t) {
		for (int i = 0; i < 9; i++) {
			sprites.push_back(new UISprite(t, IntRect(542, 12, 1, 6), Vector2f(0, 0), Vector2f(4, 4)));
			animation* anim = new animation(list<IntRect>{IntRect(542, 12, 1, 6), IntRect(532, 14, 3, 3), IntRect(521, 11, 8, 8), IntRect(532, 14, 3, 3)}, sprites[i]);
			anim->setOffsetList(list<Vector2f>{ Vector2f(0, 0), Vector2f(-1 * 4, 2 * 4), Vector2f(-3 * 4, -1 * 4), Vector2f(-1 * 4, 2 * 4)});
			anims.push_back(anim);
			
			timers.push_back(new animTimer(anims[i], 6, true));

			spriteList.push_back(sprites[i]);
		}

		hitbox = new objectHitbox(IntRect(0, 0, 0, 0), sprites[0]);
		
	}

	bool eachFrame(float* deltaT) {


		for (animTimer* timer : timers) {
			timer->run(deltaT);
		}

		*deltaT = 0.00000000000000000000000001;

		return false;
	}


	void stop() {
		shooting = false;
		for (UISprite* sprite : spriteList) {
			sprite->setCameraPosition(Vector2f(-10000, 0));
		}
	}

	void start(bool r) {
		shooting = true;
		for (UISprite* sprite : spriteList) {
			int randomX = rand() % (1920);
			int randomY = rand() % (1080);
			sprite->setCameraPosition(Vector2f(randomX, randomY));
		}
	}

	void uniqueReset() {
		stop();
	}

	list<UISprite*> getUISprites() {
		return spriteList;
	}


};