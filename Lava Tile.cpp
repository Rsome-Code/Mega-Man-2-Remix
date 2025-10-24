#include "death tile.cpp"
#include "animate.cpp"
#include "animation Timer.cpp"
#pragma once

class LavaTile :public DeathTile {

protected:
	animation* flowAnim;
	animTimer* flowTimer;

public:
	/*LavaTile(Vector2f loc, Texture* t, int tileNum, float z) {
		this->z = z;
		tileNumber = tileNum;
		location = loc;
		int tY = tileNum / 4;
		int tX = tileNum % 4;


		location = loc;
		sprite = new objectSprite("Tile", t, Vector2i(tX * 16, tY * 16), Vector2i(16, 16), Vector2f(loc.x * size, loc.y * size), Vector2f(4, 4), 1);

		deathBox = new objectHitbox(IntRect(0, 0, 16, 16), sprite);

		
	}*/

	void animSetup(int animFrames) {
		list <IntRect> temp;
		for (int i = 0; i < animFrames; i++) {
			temp.push_back(IntRect(Vector2i(sprite->getRect().getPosition().x, sprite->getRect().getPosition().y + (i*16)), Vector2i(sprite->getRect().getSize())));
		}

		flowAnim = new animation(temp, sprite);
		
		flowTimer = new animTimer(flowAnim, 4, true);
	}

	void animate(float* deltaT) {
		flowTimer->run(deltaT);
	}
};