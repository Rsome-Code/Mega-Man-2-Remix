#include "wily boss.cpp"
#include "dragon fire.cpp"

#pragma once

class Dragon : public WilyBoss {

	using WilyBoss::WilyBoss;

	shared_ptr<objectSprite> mouth;
	IntRect mouthClose;
	IntRect mouthOpen;


	shared_ptr<objectSprite> tail;
	shared_ptr<objectSprite> wings;
	shared_ptr<animation> wingAnim;
	shared_ptr<animTimer> wingTimer;

	shared_ptr<animation> tailAnim;
	shared_ptr<animTimer> tailTimer;

	shared_ptr<objectSprite> darkness;

	float shootTime = 1.4;
	float shootTime_left = 0;

	float columnTime = 2;
	float columnTime_left = columnTime;

	list<shared_ptr<objectSprite>> explos;
	bool exploAnimFin = false;
	shared_ptr<objectSprite> currentEx = NULL;

	shared_ptr<animation> exploAnim;
	shared_ptr<animTimer> exploTimer;

	Vector2f startPos;

	void initial() {
		currentTransparency = 255;

		phys->setRect(IntRect(0, 224, 94, 112));

		shared_ptr<Texture> tex = shared_ptr<Texture>(new Texture());

		tex->loadFromFile("assets\\Wily Bosses.png");

		phys->setTexture(tex);

		mouth = shared_ptr<objectSprite>(new objectSprite(phys->getTexture(), IntRect(143, 368, 20, 17), Vector2f(0, 0), Vector2f(4, 4)));
		mouthOpen = IntRect(122, 368, 16, 23);
		mouthClose = IntRect(143, 368, 20, 17);

		tail = shared_ptr<objectSprite>(new objectSprite(phys->getTexture(), IntRect(22, 359, 28, 42), Vector2f(0, 0), Vector2f(4, 4)));
		tailAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(22, 359, 28, 42), IntRect(55, 359, 47, 24)}, tail));
		tailTimer = shared_ptr<animTimer>(new animTimer(tailAnim, 1, true));

		wings = shared_ptr<objectSprite>(new objectSprite(phys->getTexture(), IntRect(198, 360, 28, 28), Vector2f(0, 0), Vector2f(4, 4)));
		wingAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(198, 360, 28, 28), IntRect(164, 369, 32, 21)}, wings));
		wingAnim->setOffsetList(list<Vector2f>{Vector2f(0, 0), Vector2f(2 * 4, 16 * 4)});
		wingTimer = shared_ptr<animTimer>(new animTimer(wingAnim, 4, true));

		code = "dragon";
		bossInitial(code);
		musicSetup();

		darkness = shared_ptr<objectSprite>(new objectSprite(shared_ptr<Texture>(new Texture())));
		darkness->setRect(IntRect(Vector2i(0,0), Vector2i(150*4, 200*4)));
		darkness->setColour(Colour::Black());
		darkness->setPosition(phys->getPosition());


		startPos = phys->getPosition();

		exploSetup();

		exploAnimFin = false;
		if (explos.empty()) {
			exploAnim->reset();
			
			for (int i = 0; i < 8; i++) {
				Vector2i posi = Vector2i(rand() % int(phys->getSize().x), rand() % int(phys->getSize().y));
				posi = Vector2i(posi.x + phys->getPosition().x, posi.y + phys->getPosition().y);
				shared_ptr<objectSprite> exp = shared_ptr<objectSprite>(new objectSprite(phys->getTexture(), exploAnim->getCurrentRect(), Vector2f(posi), Vector2f(4, 4)));
				explos.push_back(exp);
			}
			exploAnim->setSprite(*explos.begin());
		}

		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 0, 94, 112), phys));

		hurt = shared_ptr<objectHitbox>(new objectHitbox(IntRect(0, 13*4, 30, 30), phys));

		updateSpritePositions();
	}

	void exploSetup() {
		exploAnim = shared_ptr<animation>(new animation(list<IntRect>{IntRect(Vector2i(191, 0), Vector2i(24, 24)), IntRect(Vector2i(216, 4), Vector2i(16, 16)), IntRect(Vector2i(235, 6), Vector2i(12, 12)), IntRect(Vector2i(253, 7), Vector2i(10, 10)), IntRect(Vector2i(269, 10), Vector2i(4, 4))}, sprite));
		
		list<Vector2f> temp = list<Vector2f>{ Vector2f(0, 0), Vector2f(4 * 4, 4 * 4), Vector2f(6 * 4, 6 * 4), Vector2f(7 * 4, 7 * 4), Vector2f(10 * 4, 10 * 4) };
		exploAnim->setOffsetList(temp);
		
		exploTimer = shared_ptr<animTimer>(new animTimer(exploAnim, 16, false));
		offSetList();
	}


	list<shared_ptr<objectSprite>> getExtraSprites() {

		list<shared_ptr<objectSprite>> temp;

	
		if (!exploAnimFin) {

			temp.push_back(mouth);
			temp.push_back(wings);
			temp.push_back(tail);

			//if (!introDone) {
			temp.push_back(darkness);
			//}
			if (hp <= 0) {
				//temp.push_back(phys);
				temp.push_back(currentEx);
			}
		}
		else {
			temp.push_back(darkness);
		}

		return temp;
	}


	float introSpeed = 150;
	float currentTransparency = 255;
	bool introAnim(float* deltaT) {
		currentTransparency -= (*deltaT * introSpeed);
		darkness->setTransparency(currentTransparency);

		if (darkness->getTransparency() <= 0) {
			return true;
		}
		return false;
	}

	void wingLoop(float* deltaT) {
		wingTimer->run(deltaT);
	}
	void tailLoop(float* deltaT) {
		tailTimer->run(deltaT);
	}

	int quickDam() {
		return 200;
	}

	virtual bool death(float* deltaT, list<shared_ptr<enemy>>* tempEList) {
		bossMusic->stop();
		if (exploAnimFin) {
			if (!dead) {
				sprite->setPosition(Vector2f(-1000, 0));
				dead = true;
				
			}
			hit->setPosition(Vector2f(-1000, 0));
			hurt->setPosition(Vector2f(-1000, 0));





			return false;
		}

		else {
			healthBar->update(0);
			//sprite->setRect(IntRect(0, 224, 94, 112));
			explosion(deltaT);
			return false;
		}
	}

	void explosion(float* deltaT) {

		currentTransparency += (*deltaT * introSpeed);
		darkness->setTransparency(currentTransparency);
		darkness->setPosition(phys->getPosition());
		if (currentTransparency >= 255) {
			darkness->setTransparency(255);
		}


		if (currentEx == NULL) {
			if (explos.empty()) {
				exploAnimFin = true;
				exploAnim->setSprite(mov);
				exploAnim->reset();
				exploTimer->reset();
			}
			else {
				currentEx = *explos.begin();
				exploAnim->reset();
				exploTimer->reset();
				exploAnim->setSprite(currentEx);
			}
		}

		else {
			exploTimer->run(deltaT);
			if (exploTimer->isFinished()) {
				hitSound->play();
				currentEx = NULL;
				explos.pop_front();
			}
		}

		for (shared_ptr<objectSprite> ex : explos) {
			float dist = Maths::getDistance(prevPos, phys->getPosition());

			ex->setPosition(Vector2f(ex->getPosition().x, ex->getPosition().y + dist));
		}
	}


	int pattern = 0;
	int startAngle = 230;
	int maxSprayFire = 10;

	
	void sprayShoot(list<shared_ptr<EnemyBullet>>* bList) {
		openMouth();
		
		
		int angle = startAngle;
		

		for (int i = 0; i < maxSprayFire; i += 2) {
			angle = startAngle - ((100 / maxSprayFire) * (i + pattern));
			shared_ptr<DragonFire> temp = shared_ptr<DragonFire>(new DragonFire(phys->getTexture(), getFirePos(), angle));
			
			bList->push_back(temp);
		}

		if (pattern == 0) {
			pattern = 1;
		}
		else if (pattern == 1) {
			pattern = 0;
		}
	}

	Vector2f getFirePos() {
		float x = phys->getPosition().x;
		float y = phys->getPosition().y + (45 * 4);

		return Vector2f(x, y);
	}


	int maxColumnFire = 8;

	void columnShoot(list<shared_ptr<EnemyBullet>>* bList, shared_ptr<camera> cam) {
		openMouth();

		int exclude = rand() % maxColumnFire;

		for (int i = 0; i < maxColumnFire; i += 1) {
			if (exclude != i) {
				shared_ptr<DragonFire> temp = shared_ptr<DragonFire>(new DragonFire(phys->getTexture(), getFirePos()));
				temp->setStartMove(Vector2f(phys->getPosition().x - (temp->getSprite()->getSize().x * 1.2), (5 * 4) + cam->getPosition().y + (i * temp->getSprite()->getSize().y)));
				bList->push_back(temp);
			}
		}
	}

	float mouthTime = 0.2;
	float mouthTime_left = 0;
	void openMouth() {
		mouthTime_left = mouthTime;
	}

	void mouthLoop(float* deltaT) {
		mouthTime_left -= *deltaT;
		if (mouthTime_left > 0) {
			
			mouth->setRect(mouthOpen);
		}
		else {
			mouth->setRect(mouthClose);
		}
	}

	bool moveDown = true;
	int moveSpeed = 75;
	void moveLoop(float* deltaT, shared_ptr<camera> cam) {
		
		if (moveDown) {
			phys->move(Angle::down, deltaT, moveSpeed);
			if (phys->getCameraPosition().y + phys->getSize().y > 1080) {
				moveDown = false;
			}
		}
		else {
			phys->move(Angle::up, deltaT, moveSpeed);
			if (phys->getCameraPosition().y < 0) {
				moveDown = true;
			}
		}
	}

	void updateSpritePositions() {
		if (mouthTime_left > 0) {
			mouth->setPosition(Vector2f(phys->getPosition().x + (8 * 4), phys->getPosition().y + (34 * 4)));
		}
		else {
			mouth->setPosition(Vector2f(phys->getPosition().x + (4 * 4), phys->getPosition().y + (34 * 4)));
		}

		tail->setPosition(Vector2f(phys->getPosition().x + (64 * 4), phys->getPosition().y + (112 * 4)));
		wings->setPosition(Vector2f(phys->getPosition().x + (66 * 4), phys->getPosition().y + (9 * 4)));
	}

	void alive(shared_ptr<player> p, float* deltaT, list<shared_ptr<tile>>* tileList, list<shared_ptr<enemy>>* objectList, list<shared_ptr<GameObject>>* obList, list<shared_ptr<EnemyBullet>>* bList, shared_ptr<SoundCollection> soundCol, shared_ptr<camera> cam) {


		wingLoop(deltaT);

		tailLoop(deltaT);

		if (hp > 14) {

			shootTime_left -= *deltaT;


			if (shootTime_left <= 0) {
				shootTime_left = shootTime;

				
					sprayShoot(bList);
				
				
			}
		}
		else {
			columnTime_left -= *deltaT;


			if (columnTime_left <= 0) {
				columnTime_left = columnTime;

				columnShoot(bList, cam);
				


			}
			
		}

		mouthLoop(deltaT);

		moveLoop(deltaT, cam);

		updateSpritePositions();

		hit->updatePos();
		hurt->updatePos();


		for (shared_ptr<objectSprite> ex : explos) {
			float dist = Maths::getDistance(prevPos, phys->getPosition());

			ex->setPosition(Vector2f(ex->getPosition().x, ex->getPosition().y + dist));
		}
	}


};