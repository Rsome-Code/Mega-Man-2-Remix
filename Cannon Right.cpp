#include "crazy cannon.cpp"
#pragma once


//This entire class only exists for debugging

class CannonRight : public CrazyCannon {
	using CrazyCannon::CrazyCannon;

public:
	void initial() {
		faceRight = true;
		sprite->setPosition(initialPos);

		sprite->setRect(IntRect(374, 581, 31, 22));

		downAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(374, 581, 31, 22), IntRect(407, 580, 31, 23), IntRect(440, 581, 31, 22), IntRect(473, 580, 31, 23), IntRect(508, 577, 29, 26), IntRect(541, 577, 29, 26)}, sprite));

		upAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(541, 577, 29, 26), IntRect(508, 577, 29, 26), IntRect(473, 580, 31, 23), IntRect(440, 581, 31, 22), IntRect(407, 580, 31, 23), IntRect(374, 581, 31, 22)}, sprite));

		downAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(2 * 4, -4 * 4), Vector2f(2 * 4, -4 * 4)});
		upAnim->setOffsetList(list<Vector2f>{Vector2f(2 * 4, -4 * 4), Vector2f(2 * 4, -4 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0, 0)});

		if (faceRight) {
			upAnim->swapAll();
			downAnim->swapAll();

			downAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, -4 * 4), Vector2f(0 * 4, -4 * 4)});
			upAnim->setOffsetList(list<Vector2f>{Vector2f(0 * 4, -4 * 4), Vector2f(0 * 4, -4 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0 * 4, 0 * 4), Vector2f(0 * 4, -1 * 4), Vector2f(0, 0)});
		}

		if (up) {
			timer = shared_ptr<animTimer> (new animTimer(downAnim, 8, false));

			while (downAnim->getCurrentIndex() != downAnim->getSize()) {
				downAnim->nextFrame(false);
				downAnim->thisFrame();
			}

		}
		else {
			timer = shared_ptr<animTimer> (new animTimer(upAnim, 8, false));
			while (upAnim->getCurrentIndex() != upAnim->getSize()) {

				upAnim->nextFrame(false);
				upAnim->thisFrame();
			}
		}


		code = "crazy cannon-y";

		hp = 5;
		damage = 3;

		offSetList();

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 31, 22), sprite));
		hurt = hit;
	}
};