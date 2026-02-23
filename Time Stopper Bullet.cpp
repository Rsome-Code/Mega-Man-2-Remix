#include "bullet.cpp"
#include "animation timer.cpp"
#include <random>
#pragma once

class TimeStopBullet:public bullet {

	vector<shared_ptr<UISprite>> sprites;
	vector<shared_ptr<animation>> anims;
	vector<shared_ptr<animTimer>> timers;

	list<shared_ptr<UISprite>> spriteList;



public:

	TimeStopBullet(shared_ptr<Texture> t) {
		for (int i = 0; i < 9; i++) {
			sprites.push_back(shared_ptr<UISprite>(new UISprite(t, IntRect(542, 12, 1, 6), Vector2f(0, 0), Vector2f(4, 4))));
			shared_ptr<animation> anim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(542, 12, 1, 6), IntRect(532, 14, 3, 3), IntRect(521, 11, 8, 8), IntRect(532, 14, 3, 3)}, sprites[i]));
			anim->setOffsetList(list<Vector2f>{ Vector2f(0, 0), Vector2f(-1 * 4, 2 * 4), Vector2f(-3 * 4, -1 * 4), Vector2f(-1 * 4, 2 * 4)});
			anims.push_back(anim);
			
			timers.push_back(shared_ptr<animTimer> (new animTimer(anims[i], 6, true)));

			spriteList.push_back(sprites[i]);
		}



		hitbox = shared_ptr<objectHitbox>(new objectHitbox());
		
	}

	bool eachFrame(float* deltaT) {


		for (shared_ptr<animTimer> timer : timers) {
			timer->run(deltaT);
		}

		*deltaT = 0.00000000000000000000000001;

		return false;
	}


	void stop() {
		shooting = false;
		for (shared_ptr<UISprite> sprite : spriteList) {
			sprite->setCameraPosition(Vector2f(-10000, 0));
		}
	}

	void start(bool r) {
		shooting = true;
		for (shared_ptr<UISprite> sprite : spriteList) {
			int randomX = rand() % (1920);
			int randomY = rand() % (1080);
			sprite->setCameraPosition(Vector2f(randomX, randomY));
		}
	}

	void uniqueReset() {
		stop();
	}

	list<shared_ptr<UISprite>> getUISprites() {
		return spriteList;
	}


};