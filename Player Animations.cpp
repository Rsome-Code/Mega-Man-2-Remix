#include "Physics Object.cpp"
#include "Animate.cpp"
#include "Animation Timer.cpp"
#pragma once

class playerAnimation {
	animation* idle;
	animation* shoot;
	animation* throwIdle;

	animation* hurt;
	
	animation* jump;
	animation* jumpShoot;
	animation* jumpThrow;
	animation* toe;

	animation* walk;
	animTimer* walkTimer;
	animation* shootWalk;
	animTimer* shootWalkTimer;

	animation* ladder;
	animation* ladderShoot;
	animTimer* ladderTimer;
	animation* ladderThrow;

	animation* ladderTop;


	float shootTime = 0.3;
	float shootTemp = 0.3;
	

	physicsObject* sprite;
	float idleTime = 1.5;
	float blinkTime = 0.2;
	float untilRun = 0.1;
	bool running = false;
	bool runIdle = false;
	bool facingRight = false;
	bool shooting = false;
	bool metalBlade = false;
	bool leafShield = false;

	//0 = idle, 1 = tip toe, 2 = walk, 3 = jump, 4 = climb
	int currentAnim = 0;

public:
	playerAnimation(physicsObject* s) {
		sprite = s;
		idle = new animation(list<IntRect> {IntRect(Vector2i(3, 20), Vector2i(21, 25)), IntRect(Vector2i(54, 20), Vector2i(21, 25)) }, sprite);
		toe = new animation(list<IntRect> {IntRect(Vector2i(100, 20), Vector2i(22, 25)) }, sprite);
		jump = new animation(list<IntRect> {IntRect(Vector2i(276, 12), Vector2i(26, 31)) }, sprite);
		walk = new animation(list<IntRect> {IntRect(Vector2i(148, 22), Vector2i(26, 26)), IntRect(Vector2i(183, 20), Vector2i(24, 26)), IntRect(Vector2i(210, 22), Vector2i(22, 26)), IntRect(Vector2i(183, 20), Vector2i(24, 26)) }, sprite);
		walkTimer = new animTimer(walk, 8, true);
		shootWalk = new animation(list<IntRect> {IntRect(Vector2i(35, 79), Vector2i(33, 23)), IntRect(Vector2i(70, 77), Vector2i(28, 26)), IntRect(Vector2i(103, 79), Vector2i(32, 24)), IntRect(Vector2i(70, 77), Vector2i(28, 26))}, sprite);
		shootWalkTimer = new animTimer(shootWalk, 8, true);
		shoot = new animation(list<IntRect>{IntRect(Vector2i(1, 77), Vector2i(31, 25))}, sprite);
		jumpShoot = new animation(list<IntRect>{IntRect(Vector2i(140, 69), Vector2i(30, 32))}, sprite);
		jumpShoot->setOffsetList(list<Vector2f> { Vector2f( - 4 * sprite->getScale().x, 2 * sprite->getScale().y)});
		shooting = false;
		shootWalk->setDisplay(false);
		jump->setDisplay(true);
		toe->setDisplay(true);
		walk->setDisplay(true);
		jumpShoot->setDisplay(false);
		throwIdle = new animation(list<IntRect>{IntRect(Vector2i(214, 77), Vector2i(30, 25))}, sprite);

		jumpThrow= new animation(list<IntRect>{IntRect(Vector2i(248, 69), Vector2i(29, 31))}, sprite);
		jumpThrow->setDisplay(false);

		throwIdle->setOffsetList(list<Vector2f> { Vector2f(-7 * sprite->getScale().x, 0)});
		shoot->setOffsetList(list<Vector2f> { Vector2f(-10 * sprite->getScale().x, 0)});
		jumpShoot->setOffsetList(list<Vector2f> { Vector2f(-4 * sprite->getScale().x, 0)});
		jumpThrow->setOffsetList(list<Vector2f> { Vector2f(-3 * sprite->getScale().x, 0)});

		ladder = new animation(list<IntRect>{IntRect(Vector2i(310, 14), Vector2i(18, 31)), IntRect(Vector2i(337, 14), Vector2i(17, 31))}, sprite);

		ladderTop = new animation(list<IntRect>{IntRect(Vector2i(364, 18), Vector2i(18, 23))}, sprite);

		ladderTimer = new animTimer(ladder, 8, true);
		ladderShoot = new animation(list<IntRect>{IntRect(Vector2i(177, 69), Vector2i(26, 30))}, sprite);
		ladderShoot->setOffsetList(list<Vector2f> { Vector2f(-7 * sprite->getScale().x, 0)});
		ladderThrow = new animation(list<IntRect>{IntRect(Vector2i(248, 69), Vector2i(30, 30))}, sprite);

		hurt = new animation(list<IntRect>{IntRect(Vector2i(387, 12), Vector2i(26, 28))}, sprite);
	}
public:

	void ladderAnim(float* deltaT) {
		currentAnim = 4;
		ladderTimer->run(deltaT);
	}

	void ladderGetUp() {
		currentAnim = 4;
		if (!shooting) {
			ladderTop->thisFrame();
		}
	}

	

	void ladderStart() {
		ladder->thisFrame();
	}


	void setMetalBlade(bool b) {
		metalBlade = b;
	}

	void runJump() {
		jump->thisFrame();
		if (metalBlade) {
			jumpThrow->thisFrame();
		}
		else {
			jumpShoot->thisFrame();
		}
		currentAnim = 3;
	}

	void toeAnim(float* deltaT, bool right) {
		if (right != facingRight) {
			resetRun();
		}

		untilRun = untilRun - *deltaT;
		if (untilRun <= 0) {
			if (running == false) {
				//walk->thisFrame();
				//shootWalk->thisFrame();
				running = true;
			}
			walkTimer->run(deltaT);
			shootWalkTimer->run(deltaT);
			currentAnim = 2;
		}
		else {
			walk->reset();
			shootWalk->reset();
			toe->thisFrame();
			currentAnim = 1;
			
		}
	}

	bool getFacingRight() {
		return facingRight;
	}

	animation* getIdle(){
		return idle;
	}


	void idleAnim(float* deltaT) {
		
		idleTime = idleTime - *deltaT;
		if (idleTime <= 0) {
			runIdle = true;
			idle->nextFrame(false);
		}

		if (runIdle) {
			//idleTimer->run(deltaT);
			blinkTime = blinkTime - *deltaT;
			if (blinkTime <= 0) {
				resetIdle();
			}
		}
	}
	void resetIdle() {
		runIdle = false;
		idleTime = 1.5;
		blinkTime = 0.2;
		idle->reset();
		idle->thisFrame();
		currentAnim = 0;
	}


	void swapDirection(bool right) {
		bool swapped;
		if (right != facingRight) {
			swapped = true;
			
			idle->swapAll();
			walk->swapAll();
			shootWalk->swapAll();
			toe->swapAll();
			jump->swapAll();
			shoot->swapAll();
			jumpShoot->swapAll();
			throwIdle->swapAll();
			jumpThrow->swapAll();
			ladder->swapAll();
			ladderShoot->swapAll();
			ladderThrow->swapAll();
			hurt->swapAll();
			
		}
		else {
			swapped = false;
		}
		facingRight = right;

		if (swapped) {
			if (facingRight) {
				throwIdle->setOffsetList(list<Vector2f> { Vector2f(-2 * sprite->getScale().x, 0)});
				shoot->setOffsetList(list<Vector2f> { Vector2f(0, 0)});
				jumpThrow->setOffsetList(list<Vector2f> { Vector2f(-4 * sprite->getScale().x, 0)});
				ladderShoot->setOffsetList(list<Vector2f> { Vector2f(-1 * sprite->getScale().x, 0)});
				//ladder->setOffsetList(list<Vector2f> { Vector2f(-6 * sprite->getScale().x, 0), Vector2f(-6 * sprite->getScale().x, 0)});
			}
			else {
				throwIdle->setOffsetList(list<Vector2f> { Vector2f(-7 * sprite->getScale().x, 0)});
				shoot->setOffsetList(list<Vector2f> { Vector2f(-10 * sprite->getScale().x, 0)});
				jumpShoot->setOffsetList(list<Vector2f> { Vector2f(-4 * sprite->getScale().x, 0)});
				jumpThrow->setOffsetList(list<Vector2f> { Vector2f(-3 * sprite->getScale().x, 0)});
				ladderShoot->setOffsetList(list<Vector2f> { Vector2f(-7 * sprite->getScale().x, 0)});

			}
		}
		if (currentAnim == 2 && swapped) {
			resetIdle();
		}
	}


	void shootOffset(int index) {
		if (!facingRight) {
			throwIdle->setOffsetList(list<Vector2f> { Vector2f(-7 * sprite->getScale().x, 0)});
		}
		/*if (currentAnim == 0 && !facingRight) {
			if (!metalBlade) {
				sprite->setVisualOffset(Vector2f(-10 * sprite->getScale().x, 0));
			}
			else {
				sprite->setVisualOffset(Vector2f(-7 * sprite->getScale().x, 0));
			}
		}
		if(currentAnim == 0 && facingRight) {
			if (!metalBlade) {
				sprite->setVisualOffset(Vector2f(0, 0));
			}
			else {
				sprite->setVisualOffset(Vector2f(-2 * sprite->getScale().x, 0));
			}
		}*/
		if (currentAnim == 2 && !metalBlade) {
			float x = 0;
			float y = 0;

			if (index == 1 || index == 0) {
				y = 2 * sprite->getScale().y;

				if (facingRight) {
					x = -3 * sprite->getScale().x;
				}
				else {
					x = -9 * sprite->getScale().x;
				}
				sprite->setVisualOffset(Vector2f(x, y));
			}

			else if (index == 2 || index == 4) {
				if (facingRight) {
					x = 0 * sprite->getScale().x;
				}
				else {
					x = -7 * sprite->getScale().x;
				}
				sprite->setVisualOffset(Vector2f(x, y));
			}

			else if (index == 3) {
				y = 2 * sprite->getScale().y;
				if (facingRight) {
					x = -4 * sprite->getScale().x;
				}
				else {
						x = -7 * sprite->getScale().x;
				}
				sprite->setVisualOffset(Vector2f(x, y));
			}
			sprite->setVisualOffset(Vector2f(x, y));

		}

		/*else if (currentAnim == 3) {
			if (!facingRight) {
				if (!metalBlade) {
					sprite->setVisualOffset(Vector2f(-4 * sprite->getScale().x, 0));
				}
				else {
					sprite->setVisualOffset(Vector2f(-3 * sprite->getScale().x, 0));
				}
			}
		}*/
	}
	void offsetDecide(int index) {
		
		if (currentAnim == 1 && !shooting && facingRight) {
			sprite->setVisualOffset(Vector2f(-1 * sprite->getScale().x, 0));
		}

		else if (currentAnim == 2) {
			float x = 0;
			float y = 0;
			
			

			if (index == 1) {
				y = 2 * sprite->getScale().y;
				
				if (facingRight) {

						x = -3 * sprite->getScale().x;
					

				}
				else {
				
						x = -2 * sprite->getScale().x;
					
			
					
				}
				sprite->setVisualOffset(Vector2f(x, y));
			}
			else if (index == 2 || index == 4) {
				if (facingRight) {
				
						x = -6 * sprite->getScale().x;
					

				}
				else {
					x = 3 * sprite->getScale().x;
				}
				sprite->setVisualOffset(Vector2f(x, y));
			}



			else if (index == 3) {
				y = 2 * sprite->getScale().y;
				if (facingRight) {
					if (!shooting) {
						x = -3 * sprite->getScale().x;
					}
					else {
						x = -4 * sprite->getScale().x;
					}
				}
				else {
					if (!shooting) {
						x = 2 * sprite->getScale().x;
					}
					else {
						x = -7 * sprite->getScale().x;
					}
				}
				sprite->setVisualOffset(Vector2f(x, y));
			}



			
		}

		else if (currentAnim == 3 && facingRight) {
			sprite->setVisualOffset(Vector2f(-4 * sprite->getScale().x, 0));
		}


		else {
			sprite->setVisualOffset(Vector2f(0, 0));
		}
	}

	//////////////////////////////////////////

	void shootStart() {
		if (!leafShield) {
			shooting = true;
			if (!metalBlade) {
				shootWalk->setDisplay(true);
			}
			else {
				shootWalk->setDisplay(false);
			}
			jump->setDisplay(false);
			jumpShoot->setDisplay(true);

			jumpThrow->setDisplay(true);
			toe->setDisplay(false);
			walk->setDisplay(false);


			shootTemp = shootTime;

			if (currentAnim == 2 && !metalBlade) {
				shootWalk->prevFrame();

			}
			else if (currentAnim == 3) {
				if (!metalBlade) {
					jumpShoot->thisFrame();
				}
				else {
					jumpThrow->thisFrame();
				}
			}
			shootOffset(walk->getCurrentIndex());
		}
	}

	//Run every frame
	void shootDecide(float* deltaT) {
		
		if (shooting) {
			shootOffset(shootWalk->getCurrentIndex());
			//idle->setDisplay(false);
			//shoot->setDisplay(true);


			shootTemp = shootTemp - *deltaT;

			if (shootTemp > 0) {
				if (currentAnim == 0 || currentAnim == 1) {
					shootIdle();
				}
			}
			else {
				shootEnd();

			}
			
		}

		else {
			offsetDecide(walk->getCurrentIndex());
			//idle->setDisplay(true);
			//shoot->setDisplay(false);

		}

		
		
	}
	
	void climbShoot() {
		if (!leafShield) {
			if (metalBlade) {
				ladderThrow->thisFrame();
			}
			else {
				ladderShoot->thisFrame();
			}
		}
	}


	void shootIdle() {
		//resetIdle();
		if (metalBlade) {
			throwIdle->thisFrame();
		}
		else {
			shoot->thisFrame();
		}

	}

	void shootEnd() {
		shooting = false;
		shootWalk->setDisplay(false);
		jump->setDisplay(true);
		jumpShoot->setDisplay(false);
		jumpThrow->setDisplay(false);
		toe->setDisplay(true);
		walk->setDisplay(true);
		if (currentAnim == 0) {
			resetIdle();
		}
		if (currentAnim == 2) {
			walk->prevFrame();
		}
		else if (currentAnim == 3) {
			jump->thisFrame();
		}
		else if (currentAnim == 4) {
			ladder->thisFrame();
		}
		offsetDecide(shootWalk->getCurrentIndex());
	}

	//////////////////////////////////////////////

	void resetRun() {
		untilRun = 0.1;
		running = false;
	}

	void stopped() {
		if (currentAnim != 3) {
			currentAnim = 0;
			if (shooting) {
				shootOffset(0);
			}
		}
	}

	void landed(bool b) {
		resetRun();
		resetIdle();
		walk->reset();
		shootWalk->reset();
		if (b) {
			currentAnim = 2;
			untilRun = 0;
			
			if (!shooting) {
				walk->thisFrame();
				offsetDecide(1);
			}
			else {
				shootWalk->thisFrame();
				shootOffset(1);
			}

		}

	}

	bool getShooting() {
		return shooting;
	}


	void hurtAnim() {
		hurt->thisFrame();
	}
};