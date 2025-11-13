#include "gobject.cpp"
#include "ob tile.cpp"
#include "hitbox detector.cpp"
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

	physicsObject* phys;

	bool done = false;

	float lastGrav = 9999;
	float gravTimer = 0;

	bool firstF = true;

	int floatGrav = 2000;

public:

	FallPlatform(Texture* tex, Vector2f pos) {
		tile1 = new ObTile(Vector2f(pos.x / 16, pos.y / 16), tex, 0);
		tile1->getSprite()->setRect(IntRect(638, 115, 16, 16));

		tile2 = new ObTile(Vector2f((pos.x / 16) + 1, pos.y / 16), tex, 0);
		tile2->getSprite()->setRect(IntRect(638, 115, 16, 16));

		initialPos = pos;

		setCode("fall platform");

		sprite = tile1->getSprite();

		phys = new physicsObject();
		//phys->setPosition(tile1->getSprite()->getPosition());
	}

	void initial() {
		tile1->setPosition(initialPos);
		phys->setPosition(tile1->getSprite()->getPosition());
		phys->setVerticalFriction(1000);
		tile2->setPosition(Vector2f(initialPos.x + (16*4), initialPos.y));
		state = still;

		tile1->setAct(true);
		tile2->setAct(true);
		tile1->setDisplay(true);
		tile2->setDisplay(true);
		firstF = true;
		
	}

	void eachFrame(float* deltaT, player* player, camera* cam, list<tile*>* tileList) {

		tile1->update();
		tile2->update();

		sprite->setCameraPosition(Vector2f(sprite->getPosition().x - cam->getPosition().x, sprite->getPosition().y - cam->getPosition().y));


		camCheck(cam);

		float thisFall = fallSpeed * *deltaT;

		if (hitboxDetect::hitboxDetection(player->getFoot(), tile1->getGround()) || hitboxDetect::hitboxDetection(player->getFoot(), tile2->getGround())) {
			if (!done) {
				state = fall;
			}
		}

		if (state == fall) {
			gravTimer += *deltaT;
			
			if (lastGrav < 0.2) {
				if (phys->getGravity() > 0) {
					if (phys->getVVelocity() < 0) {
						state = still;
						done = true;
					}
				}
				else if (phys->getGravity() < 0) {
					if (phys->getVVelocity() > 0) {
						state = still;
						done = true;
					}
				}
			}
			phys->enableGravity(true);
			phys->eachFrame(deltaT);
			
			tile1->setPosition(Vector2f(tile1->getPosition().x, phys->getPosition().y));
			tile2->setPosition(Vector2f(tile2->getPosition().x, phys->getPosition().y));
			//tile1->getSprite()->setPosition(Vector2f(tile1->getSprite()->getPosition().x, tile1->getSprite()->getPosition().y + thisFall));
			//tile2->getSprite()->setPosition(Vector2f(tile2->getSprite()->getPosition().x, tile1->getSprite()->getPosition().y));

			if (waterCheck(tileList)) {
				if (phys->getGravity() == -3000 && !firstF) {
					lastGrav = gravTimer;
					gravTimer = 0;
				}
				phys->setGravity(floatGrav);
			}
			else {
				if (phys->getGravity() == floatGrav && !firstF) {
					lastGrav = gravTimer;
					gravTimer = 0;
				}
				phys->setGravity(-3000);
			}
			firstF = false;
		}

		
	}

	bool waterCheck(list<tile*>* tileList) {
		for (tile* t : *tileList) {
			if (t->getWaterBox() != NULL) {
				if (hitboxDetect::hitboxDetection(t->getWaterBox(), tile1->getGround())) {
					return true;
				}
			}
		}
		return false;
	}

	void camCheck(camera* cam) {
		if (sprite->getCameraPosition().x < 0 || sprite->getCameraPosition().x > 1920) {
			initial();
		}
		if (sprite->getCameraPosition().y < 0 || sprite->getCameraPosition().y > 1200) {
			tile1->getSprite()->setPosition(Vector2f(0, 0));
			tile2->getSprite()->setPosition(Vector2f(0, 0));
		}
	}


	list<tile*> getTiles() { return list<tile*> {tile1, tile2}; }
};