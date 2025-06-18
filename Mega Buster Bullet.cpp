#include "Movable Object.cpp"
#include "Object Hitbox.cpp"
#include "Bullet.cpp"
#include "enemy.cpp"
#pragma once
class megaBuster : public bullet{

	

public:
	megaBuster(objectSprite* o, Texture* t) {
		origin = o;
		sprite = new movable ("buster", t, Vector2i(220, 4), Vector2i(8, 6), Vector2f(0, 0), Vector2f(4, 4), 1);
		
		hitbox = new objectHitbox(IntRect(Vector2i(0, 0), Vector2i(8, 6)), true, sprite);
		shootTime = 0.5;
		shootTemp = 0.5;
	}

	int checkDamage(object* en) {
		return en->busterDam();
	}

	void start(bool r) {
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
		shootTemp = shootTime;
	}

	bool eachFrame(float* deltaT) {
		shootTemp = shootTemp - *deltaT;
		if (shootTemp <= 0) {
			shootReset();
			return true;
		}

		if (shooting) {
			if (right) {
				sprite->move(0, deltaT, speed);
			}
			else {
				sprite->move(180, deltaT, speed);
			}
			hitbox->updatePos();
			return false;
		}
	}

	objectHitbox* getHitbox() {
		return hitbox;
	}

	void onHit(){
		shootReset();
	}

};