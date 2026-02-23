#include "enemy bullet.cpp"
#pragma once

class LeafShieldBullet : public EnemyBullet {

	float angleSpeed = 400;
	float speed = 500;
	float angle;

	bool shot = false;
	bool right = false;

	float shotSpeed = 500;

public:
	LeafShieldBullet(shared_ptr<Texture> t, Vector2f pos) {
		
		mov = shared_ptr<movable>(new movable(string("eBullet"), t, IntRect(337, 335, 15, 15),pos, Vector2f(4, 4)));
		Vector2f position = Vector2f(pos.x - (mov->getSize().x / 2), pos.y - (mov->getSize().y / 2));
		mov->setPosition(position);
		sprite = mov;
		hit = shared_ptr<objectHitbox>(new objectHitbox(IntRect(-5, -5, 25, 25), mov));

		damage = 4;
		collType = DESTROY;
		
	}

	void setCorner(int i) {
		if (i == 2) {
			angle = 225;
		}
		else if (i == 0) {
			angle = 315;
		}
		else if (i == 3) {
			
			angle = 135;
		}
		else if (i == 1){
			angle = 45;
		}
		//angle = 0;
	}
	

	void eachFrame(float* deltaT, list<shared_ptr<tile>>* tileList) {
		
		angle = angle + (angleSpeed * *deltaT);
		mov->move(angle, deltaT, speed);

		if (shot) {
			if (right) {
				mov->move(0, deltaT, shotSpeed);
			}
			else {
				mov->move(180, deltaT, shotSpeed);
			}
		}
		hit->updatePos();
	}

	void shoot() {
		shot = true;
	}

	void setRight(bool b) {
		right = b;
	}
};