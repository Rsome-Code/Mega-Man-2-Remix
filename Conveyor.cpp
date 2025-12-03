#include "tile with object.cpp"
#pragma once

class ConveyorTile : public TileWithObject {

	list<movable*> yellows;
	list<movable*> reds;

	list<objectSprite*> copies;

	int speed = 100;

	int angle = 0;

	float size;

	movable* reserveYellow;
	movable* reserveRed;

	float dist = 9;
	float startDist = 1.5;
	bool moveRight;

protected:
	objectHitbox* groundHitbox;
	objectHitbox* leftHitbox;
	objectHitbox* rightHitbox;
	objectHitbox* ceilingHitbox;

public:
	void reset() {

		copies.clear();

		int i = 0;
	
		for (objectSprite* sp : yellows) {
			sp->setPosition(Vector2f(sprite->getPosition().x + (((i * dist) + startDist) * 4), sprite->getPosition().y + 3 * 4));

			i++;

			copies.push_back(new objectSprite(sp));
		}
		i = 0;
		for (objectSprite* sp : reds) {
			sp->setPosition(Vector2f(sprite->getPosition().x + ((((i * dist) + (dist / 2)) + startDist) * 4), sprite->getPosition().y + 3 * 4));

			i++;
			copies.push_back(new objectSprite(sp));
		}
		
		/*else {
			for (objectSprite* sp : yellows) {
				sp->setPosition(Vector2f(sprite->getPosition().x + (((i * dist) - startDist) * 4), sprite->getPosition().y + 3 * 4));

				i++;

			}
			i = 0;
			for (objectSprite* sp : reds) {
				sp->setPosition(Vector2f(sprite->getPosition().x + ((((i * dist) - (dist / 2)) - startDist) * 4), sprite->getPosition().y + 3 * 4));

				i++;

			}
		}*/


	}
	ConveyorTile(Vector2f loc, Texture* t, float z, bool right) {
		this->z = z;

		location = loc;

		moveRight = right;

		sprite = new objectSprite("Tile", t, Vector2i(204, 121), Vector2i(16, 16), Vector2f(loc.x * (16*4), loc.y * (16 * 4)), Vector2f(4, 4), 1);
		if (right) {
			type = "conveyor anim-right";
		}
		else {
			type = "conveyor anim-left";
		}
		
		for (int i = 0; i < 3; i++) {
			movable* temp = new movable(t, IntRect(129, 85, 3, 9), Vector2f(sprite->getPosition().x + (((i*dist) + startDist)*4), sprite->getPosition().y + 3*4), Vector2f(4, 4));
			yellows.push_back(temp);
			objects.push_back(temp);
			size = temp->getSize().x;
			if (i == 2) {
				reserveYellow = temp;
			}
			copies.push_back(new objectSprite(temp));
		}
		for (int i = 0; i < 3; i++) {
			movable* temp = new movable(t, IntRect(133, 85, 3, 9), Vector2f(sprite->getPosition().x + ((((i * dist) + (dist/2)) + startDist) * 4), sprite->getPosition().y + 3 * 4), Vector2f(4, 4));
			reds.push_back(temp);
			objects.push_back(temp);
			if (i == 2) {
				reserveRed = temp;

			}
			copies.push_back(new objectSprite(temp));
		}


		groundHitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(16, 1)), true, sprite);
		ceilingHitbox = new objectHitbox(IntRect(Vector2i(0, sprite->getSize().y), Vector2i(16, 1)), true, sprite);
		leftHitbox = new objectHitbox(IntRect(Vector2i(0, 4), Vector2i(1, 16)), true, sprite);
		rightHitbox = new objectHitbox(IntRect(Vector2i(sprite->getSize().x, 4), Vector2i(1, 16)), true, sprite);

		//if (right) {
		angle = 0;
		//}
		//else {
		//	angle = 180;
		//}
	}

	objectHitbox* getGround() {
		return groundHitbox;
	}
	objectHitbox* getCeiling() {
		return ceilingHitbox;
	}
	objectHitbox* getLeft() {
		return leftHitbox;
	}
	objectHitbox* getRight() {
		return rightHitbox;
	}

	void update() {
		groundHitbox->updatePos();
		leftHitbox->updatePos();
		rightHitbox->updatePos();
		ceilingHitbox->updatePos();
	}

	void animate(float* deltaT) {
		reserveYellow->setPosition(Vector2f(-9999, -9999));
		reserveRed->setPosition(Vector2f(-9999, -9999));

		//if (moveRight) {
		rightLoop(deltaT);
		//}
		//else {
		//	leftLoop(deltaT);
		//}
	}

	/*void leftLoop(float* deltaT) {
		for (movable* mov : yellows) {
			mov->move(angle, deltaT, speed);
			if (mov->getEndPosition().x < sprite->getPosition().x ) {
				reserveYellow->setPosition(Vector2f(sprite->getEndPosition().x , mov->getPosition().y));
				reserveYellow->setScale(Vector2f(0, 0));
				reserveYellow = mov;
				reserveYellow->setScale(Vector2f(4, 4));
			}
			leftScaling(mov, sprite->getPosition().x, sprite->getEndPosition().x, true);
		}
		for (movable* mov : reds) {
			mov->move(angle, deltaT, speed);
			if (mov->getEndPosition().x < sprite->getPosition().x ) {
				reserveRed->setPosition(Vector2f(sprite->getEndPosition().x , mov->getPosition().y));
				reserveRed->setScale(Vector2f(0, 0));
				reserveRed = mov;
				reserveRed->setScale(Vector2f(4, 4));
			}
			leftScaling(mov, sprite->getPosition().x, sprite->getEndPosition().x, false);
		}
	}*/

	void rightLoop(float* deltaT) {
		for (movable* mov : yellows) {
			mov->move(angle, deltaT, speed);
			if (mov->getPosition().x > sprite->getEndPosition().x - (1 * 4)) {
				reserveYellow->setPosition(Vector2f(sprite->getPosition().x - mov->getSize().x - (3 * 4), mov->getPosition().y));
				reserveYellow->setScale(Vector2f(0, 0));
				reserveYellow = mov;
				reserveYellow->setScale(Vector2f(4, 4));
			}
			scaling(mov, sprite->getPosition().x, sprite->getEndPosition().x, true);
		}
		for (movable* mov : reds) {
			mov->move(angle, deltaT, speed);
			if (mov->getPosition().x > sprite->getEndPosition().x - (1 * 4)) {
				reserveRed->setPosition(Vector2f(sprite->getPosition().x - mov->getSize().x - (3 * 4), mov->getPosition().y));
				reserveRed->setScale(Vector2f(0, 0));
				reserveRed = mov;
				reserveRed->setScale(Vector2f(4, 4));
			}
			scaling(mov, sprite->getPosition().x, sprite->getEndPosition().x, false);
		}
	}


	/*void leftScaling(movable* s, float leftTarget, float rightTarget, bool yell) {
	//s->setScale(Vector2f(4, 4));
	//Diff between right of sprite and right of tile
		float xDif = s->getEndPosition().x - (rightTarget - (3 * 4));

		//Diff between right of sprite and left of sprite
		xDif = size - xDif;

		//Decimal of difference
		xDif = xDif / size;
		if (xDif >= 1) {

			xDif = leftTarget - s->getPosition().x;

			xDif = size - xDif;

			xDif = xDif / size;
			if (xDif >= 1) {
				xDif = 1;
			}
			s->setVisualOffset(Vector2f(size - (size * xDif), 0));
		}
		else {
			//cout << "\neh";
		}
		xDif = 4 * xDif;

		if (xDif < 0) {
			xDif = 0;
			if (yell) {
				reserveYellow->setPosition(Vector2f(sprite->getEndPosition().x, s->getPosition().y));

				reserveYellow = s;
			}
			else {
				reserveRed->setPosition(Vector2f(sprite->getEndPosition().x, s->getPosition().y));

				reserveRed = s;
			}
		}

		s->setScale(Vector2f(xDif, s->getScale().y));

		if (s->getScale().x == 4) {
			//cout << "\nhuh";
		}
	}*/

	void scaling(movable* s, float leftTarget, float rightTarget, bool yell) {
		//s->setScale(Vector2f(4, 4));
		//Diff between right of sprite and right of tile
		float xDif = s->getEndPosition().x - (rightTarget - (3*4));

		//Diff between right of sprite and left of sprite
		xDif = size - xDif;

		//Decimal of difference
		xDif = xDif / size;
		if (xDif >= 1) {
			
			xDif = leftTarget - s->getPosition().x ;

			xDif = size - xDif;

			xDif = xDif / size;
			if (xDif >= 1) {
				xDif = 1;
			}
			s->setVisualOffset(Vector2f(size - (size * xDif), 0));
		}
		else {
			//cout << "\neh";
		}
		xDif = 4 * xDif;

		if (xDif < 0) {
			xDif = 0;
			if (yell) {
				reserveYellow->setPosition(Vector2f(sprite->getPosition().x - s->getSize().x - (3 * 4), s->getPosition().y));

				reserveYellow = s;
			}
			else {
				reserveRed->setPosition(Vector2f(sprite->getPosition().x - s->getSize().x - (3 * 4), s->getPosition().y));

				reserveRed = s;
			}
		}

		s->setScale(Vector2f(xDif, s->getScale().y));

		if (s->getScale().x == 4) {
			//cout << "\nhuh";
		}
	}


	list<objectSprite*>::iterator spriteIt;

	list<objectSprite*> getInternalSprites() {
		if (moveRight) {
			return objects;
		}
		
		//list<objectSprite*> tempList;

		spriteIt = objects.begin();

		for (objectSprite* temp : copies) {
			
			objectSprite* trueOb = *spriteIt;

			//objectSprite* temp = new objectSprite(ob);
			temp->setVisualOffset(Vector2f(-trueOb->getVisualOffset().x, trueOb->getVisualOffset().y));


			float middleDif = trueOb->getMiddlePos().x - sprite->getMiddlePos().x;
			float newPos = trueOb->getMiddlePos().x - (2 * middleDif);

			temp->setMiddlePos(Vector2f(newPos, temp->getMiddlePos().y));
			//delete& temp;
			//tempList.push_back(temp);

			spriteIt = next(spriteIt);

			
		}
		return copies;
	}

	float getMovement() {
		if (moveRight) {
			return 300;
		}
		return -300;
	}

	void setMoveRight(bool right) {
		moveRight = right;
	}
};