//This class will control the timing for the animation assigned to it.
//It can animate at any fps you want, regardless of the game's fps, though it cannot exceed it.
//Multiple of these can be applied to one set of animation frames

#include "Animate.cpp"
#pragma once

class animTimer {
	animation* anim;
	float fps;
	float targetDelta;
	float countDown;
	bool loop;
	bool display;

public:animTimer(animation* anim, float fps, bool loop) {
		this->anim = anim;
		this->fps = fps;
		targetDelta = (1 / fps);
		countDown = targetDelta;
		this->loop = loop;
		display = true;
	}


public: 
	//Returns true when animation has finished/looped
	bool run(float* delta) {
		bool nextFrame = false;
		if (*delta >= countDown) {
			nextFrame = true;
			anim->nextFrame(loop);
			countDown = targetDelta;
		}
		else {
			countDown = countDown - *delta;
			
		}
		return nextFrame;
		
	}

	bool isFinished(float *deltaT) {
		if (loop) {
			return anim->isFinishedLoop() && *deltaT >= countDown;
		}
		else {
			return anim->isFinishedNotLoop() && *deltaT >=countDown;
		}
	}
	bool isFinished() {
		if (loop) {
			return anim->isFinishedLoop();
		}
		else {
			return anim->isFinishedNotLoop();
		}
	}

	void skip() {
		countDown = 0;
	}
	void reset() {
		countDown = targetDelta;
	}

	void setDisplay(bool b) {
		display = b;
		anim->setDisplay(b);
	}

	void setAnim(animation* a) {
		anim = a;
	}
};