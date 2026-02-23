#include "Movable Object.cpp"
#include "Object Hitbox.cpp"
#include "Bullet.cpp"
#include "object.cpp"
#pragma once
class megaBuster : public bullet{

	

public:
	megaBuster(shared_ptr<objectSprite> o, shared_ptr<Texture> t, shared_ptr<SoundCollection> soundCol) {
		origin = o;
		sprite = shared_ptr<movable>(new movable ("buster", t, Vector2i(220, 4), Vector2i(8, 6), Vector2f(0, 0), Vector2f(4, 4), 1));
		
		hitbox = shared_ptr<objectHitbox>(new objectHitbox(IntRect(Vector2i(-2, -2), Vector2i(8, 10)), sprite));
		shootTime = 0.5;
		shootTemp = 0;
		dinkSetup(soundCol);

	}

	int checkDamage(shared_ptr<object> en) {
		return en->busterDam();
	}

	void start(bool r) {
		deflected = false;
		shooting = true;

		float offset;
		if (r) {
			offset = 80;
		}
		else {
			offset = -24;
		}
		sprite->setPosition(Vector2f(origin->getPosition().x + offset, origin->getPosition().y + 38));
		right = r;
		if (right) {
			direction = 0;
		}
		else {
			direction = 180;
		}
		shootTemp = shootTime;
	}

	bool eachFrame(float* deltaT) {
		if (shooting) {
		shootTemp = shootTemp - *deltaT;
		if (shootTemp <= 0) {
			shootReset();
			return true;
		}

			sprite->move(direction, deltaT, speed);

			hitbox->updatePos();
			return false;
		}
		return false;

	}

	

	shared_ptr<objectHitbox> getHitbox() {
		return hitbox;
	}

	void onHit(shared_ptr<object> e){
		shootReset();
	}

};