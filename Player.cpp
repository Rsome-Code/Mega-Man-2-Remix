#include "Physics Object.cpp"
#include "Player Controls.cpp"
#include "Object Hitbox.cpp"
#include "animate.cpp"
#include "animation Timer.cpp"
#include "Player Animations.cpp"
#include "teleport.cpp"
#include "bullet.cpp";
#include "mega buster.cpp"
#include "Energy Bar.cpp"
#include "Damage Effect.cpp"
#pragma once

class player {
	physicsObject* sprite;
	Texture* texture;
	pControls* controls;
	bool grounded = true;
	teleport* tele;

	objectHitbox* hit;
	objectHitbox* foot;
	objectHitbox* head;
	objectHitbox* ladderHit;
	objectHitbox* ladderBelow;
	objectHitbox* ladderAbove;

	playerAnimation* pAnim;

	energyBar* health;

	bool damage = false;
	float damageTime = 0.5;
	float tempDTime = 0.5;

	damageEffect* dam;
	

public:
	player(pController* p1) {
		texture = new Texture();
		if (!texture->loadFromFile("Assets\\NES - Mega Man 2 - Mega Man.png")) {
			cout << "error";
		}

		Image im = texture->copyToImage();
		Image* image = &im;
		sprite = new physicsObject("player", texture, im, IntRect(2, 21, 22, 24), Vector2f(1000, 2000), Vector2f(4, 4), 1, 200);

		sprite->setFullColour(&Color::Red);

		pAnim = new playerAnimation(sprite);
		controls = new pControls(p1, sprite, pAnim);
		loadPallete();

		hit = new objectHitbox(IntRect(Vector2i(5 * sprite->getScale().x, 2 * sprite->getScale().y), Vector2i(11, 22)), true, sprite);
		foot = new objectHitbox(IntRect(Vector2i(9 * sprite->getScale().x, 7 * sprite->getScale().y), Vector2i(4, 20)), true, sprite);
		head = new objectHitbox(IntRect(Vector2i(7 * sprite->getScale().x, 1 * sprite->getScale().y), Vector2i(7, 2)), true, sprite);
		ladderHit = new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 24 * sprite->getScale().y), Vector2i(10, 2)), true, sprite);
		ladderBelow = new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 26 * sprite->getScale().y), Vector2i(10, 2)), true, sprite);
		ladderAbove = new objectHitbox(IntRect(Vector2i(6 * sprite->getScale().x, 21 * sprite->getScale().y), Vector2i(10, 2)), true, sprite);

		Texture* hbT = new Texture();
		hbT->loadFromFile("Assets\\Health.png");
		health = new energyBar(hbT, Vector2f(20,20));

		dam = new damageEffect(sprite);

	}

	objectHitbox* getFoot() {
		return foot;
	}

	bool getGrounded() {
		return grounded;
	}

	objectHitbox* getBelow() {
		return ladderBelow;
	}
	objectHitbox* getAbove() {
		return ladderAbove;
	}

	void start(Vector2f pos) {
		tele = new teleport(sprite, pos, 0);
		setPosition(Vector2f(pos.x, pos.y));
	}

	void setPosition(Vector2f pos) {
		sprite->setPosition(pos);
		hit->updatePos();
		ladderHit->updatePos();
		foot->updatePos();
		head->updatePos();
		ladderBelow->updatePos();
		ladderAbove->updatePos();
	}

	void eachFrame(float* deltaT) {

		if (!damage) {

			if (tele == NULL) {
				controls->checkControls(deltaT);
				controls->shootEachFrame(deltaT);

				pAnim->shootDecide(deltaT);


				sprite->eachFrame(deltaT);
				

				if (controls->checkTeleport()) {
					tele = new teleport(sprite, sprite->getPosition());
				}

			}
			else {
				if (tele->eachFrame(deltaT)) {
					delete tele;
					tele = NULL;
					pAnim->resetIdle();
					pAnim->getIdle()->thisFrame();
				}
			}
		}
		else {
			dam->flicker(deltaT);
			takingDamage(deltaT);
			
		}

		if (sprite->getSpeed() > 3000) {

			//foot->setSize(Vector2i(4, 25));
		}
		else {

			//foot->setSize(Vector2i(4, 20));
		}

		updateHitbox();

	}

	void updateLighting() {
		sprite->updateLighting();
	}

	list<RectangleShape*> getLightPixels() {
		return sprite->getPixels();
	}

	void lightingCheck(LightSource* light) {

		sprite->lightingCheck(light);

	}

	bool getDamage() {
		return damage;
	}

	void updateHitbox() {
		hit->updatePos();
		ladderHit->updatePos();
		foot->updatePos();
		head->updatePos();
		ladderBelow->updatePos();
		ladderAbove->updatePos();
	}

	bool isTeleporting() {
		return tele != NULL;
	}

	objectHitbox* getHitbox() {
		return hit;
	}
	objectHitbox* getLadderHitbox() {
		return ladderHit;
	}

	objectHitbox* getHead() {
		return head;
	}

	physicsObject* getSprite() {
		return sprite;
	}

	list<objectSprite*> getSprites() {
		return list<objectSprite*> {dam->getSprite(), sprite};
	}

	object* getDamEffect() {
		return dam;
	}

	list<UISprite*> getUI() {
		return list<UISprite*> {health->getSprite()};
	}

	list<objectSprite*> getBullets() {
		return controls->getBullets();
	}
	list<objectHitbox*> getBulletHitboxes() {
		return controls->getBulletHitboxes();
	}

	void setGrounded(bool b) {
		if (!controls->getOnLadder()) {
			grounded = b;
			this->controls->setGrounded(b);
		}
	}
	playerAnimation* getAnimation() {
		return pAnim;
	}
	pControls* getControls() {
		return controls;
	}

	void takeDamage(int dam) {
		health->increaseAmount(-dam);
		sprite->setMovable(false);
		damage = true;
		pAnim->hurtAnim();
		sprite->setHVelocity(0);
		sprite->setVVelocity(0);
	}

	string getBulletType() {
		bullet* b = controls->getBulletObject();
		return b->getSprite()->getType();
	}


	void loadPallete() {
		Weapon* temp = getWeapon();
		this->texture = temp->getTexture();
		sprite->setTexture(texture);
	}

	Weapon* getWeapon() {
		return getControls()->getWeapon();
	}

private:
	void takingDamage(float* deltaT) {
		tempDTime = tempDTime - *deltaT;
		if (tempDTime <= 0) {
			tempDTime = damageTime;
			damage = false;
			sprite->setMovable(true);
			dam->reset();
			
		}

		if (pAnim->getFacingRight()) {
			sprite->move(180, deltaT, 200);
		}
		else {
			sprite->move(0, deltaT, 200);
		}
		
	}

	
};