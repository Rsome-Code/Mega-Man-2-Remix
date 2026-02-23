#include "object hitbox.cpp"
#include "animation timer.cpp"
#include "sound collection.cpp"
#pragma once

class CrashExplosion {
	vector<shared_ptr<objectSprite>> sprites;

	vector<shared_ptr<animation>> anims;
	vector<shared_ptr<animTimer>> timers;

	int count = 10;
	int count_left = count;

	Vector2f position;

	shared_ptr<Sound> explodeSound;

public:


	list<shared_ptr<objectSprite>> getSprites() {
		return list<shared_ptr<objectSprite>>(sprites.begin(), sprites.end());
	}


	CrashExplosion(shared_ptr<Texture> t, Vector2f startPos, shared_ptr<Sound> sound) {
		position = startPos;

		for (int i = 0; i < 4; i++) {
			sprites.push_back(shared_ptr<objectSprite>(new objectSprite("crash", t, IntRect(565, 241, 16, 16), Vector2f(0, 0), Vector2f(4, 4))));

			anims.push_back(shared_ptr<animation>(new animation(list<IntRect>{IntRect(565, 241, 16, 16), IntRect(584, 243, 12, 12), IntRect(602, 244, 10, 10), IntRect(618, 247, 4, 4)}, sprites[i])));

			anims[i]->setOffsetList(list<Vector2f>{Vector2f(-6 * 4, -6 * 4), Vector2f(-4 * 4, -4 * 4), Vector2f(-3 * 4, -3 * 4), Vector2f(0 * 4, 0 * 4)});

			timers.push_back(shared_ptr<animTimer>(new animTimer(anims[i], 38, false)));
		}

		explodeSound = sound;

		startExplo();
	}

	void startExplo() {
		for (shared_ptr<animTimer> t : timers) {
			t->reset();
		}
		for (shared_ptr<animation> a : anims) {
			a->reset();	
			a->thisFrame();
		}
		
		for (shared_ptr<objectSprite> s : sprites) {
			s->setPosition(Vector2f (position.x + (rand() % (32  *4)), position.y + (rand() % (32 * 4))));
		}

		explodeSound->play();

	}

	bool eachFrame(float* deltaT) {

		for (shared_ptr<animTimer> t : timers) {
			t->run(deltaT);
		}

		if (timers[0]->isFinished(deltaT)) {
			count_left--;
			
			if (count_left <= 0) {
				count_left = count;
				return true;
			}
			startExplo();
		}
		return false;
	}
};