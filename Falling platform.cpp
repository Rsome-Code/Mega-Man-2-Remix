#include "gobject.cpp"
#include "ob tile.cpp"
#include "player.cpp"
#pragma once

class FallPlatform : public GameObject {

	ObTile* tile1;
	ObTile* tile2;
	Vector2f initialPos;

	float fallSpeed = 500;

	enum State {
		still, fall
	};

	State state = still;

public:

	FallPlatform(Texture* tex, Vector2f pos) {
		tile1 = new ObTile(Vector2f(pos.x / 16, pos.y / 16), tex, 0);
		tile1->getSprite()->setRect(IntRect(638, 115, 16, 16));

		tile2 = new ObTile(Vector2f((pos.x / 16) + 1, pos.y / 16), tex, 0);
		tile2->getSprite()->setRect(IntRect(638, 115, 16, 16));

		initialPos = pos;

		setCode("fall platform");

		sprite = tile1->getSprite();
	}

	void initial() {
		tile1->setPosition(initialPos);
		tile2->setPosition(Vector2f(initialPos.x + (16*4), initialPos.y));
		state = still;

		tile1->setAct(true);
		tile2->setAct(true);
		tile1->setDisplay(true);
		tile2->setDisplay(true);
	}

	void eachFrame(float* deltaT, player* player, camera* cam) {

		tile1->update();
		tile2->update();

		camCheck(cam);

		float thisFall = fallSpeed * *deltaT;

		if (hitboxDetect::hitboxDetection(player->getFoot(), tile1->getGround()) || hitboxDetect::hitboxDetection(player->getFoot(), tile2->getGround())) {
			state = fall;
		}

		if (state == fall) {
			tile1->getSprite()->setPosition(Vector2f(tile1->getSprite()->getPosition().x, tile1->getSprite()->getPosition().y + thisFall));
			tile2->getSprite()->setPosition(Vector2f(tile2->getSprite()->getPosition().x, tile1->getSprite()->getPosition().y));
		}
	}

	void camCheck(camera* cam) {
		if (sprite->getCameraPosition().x < 0 || sprite->getCameraPosition().x > 1920) {
			initial();
		}
	}


	list<tile*> getTiles() { return list<tile*> {tile1, tile2}; }
};