#include "movable object.cpp"
#include "text.cpp"
#include "render logic.cpp"
#pragma once

class File {
	Vector2f position;

	shared_ptr<movable> backdrop;

	list<shared_ptr<movable>> checkmarkList;

	float angle = 0;

	Vector2f startSize;

	shared_ptr<text> name;

	int standardTextSize = 20;
	Vector2f textPosFromBottomLeft = Vector2f(10, 40);


	list<shared_ptr<objectSprite>> winIcons;

	bool empty = false;

public:
	File(shared_ptr<Texture> t) {

		position = Vector2f(1920 / 2, 1080 / 2);

		backdrop = shared_ptr<movable>(new movable(t, IntRect(0,0, 202, 266), position, Vector2f(2,2)));

		startSize = backdrop->getSize();

		this->name = shared_ptr<text>(new text());
		

		shared_ptr<Font> font = shared_ptr<Font>(new Font());
		font->loadFromFile("assets//font.otf");

		name->setFont(font);
		name->setFillColour(Colour::White());

		name->setPosition(Vector2f(position.x, position.y + backdrop->getSize().y));

		name->setScale(backdrop->getScale());

		empty = false;
		
	}

	bool isEmpty() {
		return empty;
	}

	shared_ptr<Font> getFont() {
		return name->getFont();
	}

	void setupIcons(bool bubble, bool heat, bool metal, bool wood, bool air, bool quick, bool flash, bool crash) {
		winIconSetup(bubble, heat, metal, wood, air, quick, flash, crash);
	}

	int lives = 2;
	void setLives(int l) {
		lives = l;
	}

	int eTanks = 0;
	void setETanks(int e) {
		eTanks = e;
	}

	void setName(string nam) {
		name->setString(nam);
	}

	string getName() {
		return name->getString();
	}

	void setNewFile() {
		shared_ptr<objectSprite> sprite = shared_ptr<objectSprite>(new objectSprite());

		sprite->setTexture(shared_ptr<Texture> (backdrop->getTexture()));
		sprite->setRect(IntRect(0, 0, 185, 198));
		sprite->setScale(Vector2f(2, 2));
		sprite->setPosition(Vector2f(8, 8));
		sprite->setColour(Colour::Black());
		winIcons.push_back(sprite);

		name->setString("New Game");

		empty = true;
	}

	bool bubble = false;
	bool heat = false;
	bool metal = false;
	bool wood = false;
	bool air = false;
	bool quick = false;
	bool flash = false;
	bool crash = false;

	

	void winIconSetup(bool bubble, bool heat, bool metal, bool wood, bool air, bool quick, bool flash, bool crash) {

		shared_ptr<objectSprite> templateS = shared_ptr<objectSprite>(new objectSprite());

		templateS->setTexture(backdrop->getTexture());
		templateS->setRect(IntRect(208, 0, 45, 47));
		templateS->setScale(Vector2f(2,2));

		if (bubble) {
			shared_ptr<objectSprite> temp = shared_ptr<objectSprite>(new objectSprite(templateS));
			temp->setPosition(Vector2f(14, 9));
			winIcons.push_back(temp);
			//won.push_back(0);
			this->bubble = true;
		}
		if (heat) {
			shared_ptr<objectSprite> temp = shared_ptr<objectSprite>(new objectSprite(templateS));
			temp->setPosition(Vector2f(14, 73));
			winIcons.push_back(temp);
			//won.push_back(3);
			this->heat = true;
		}
		if (metal) {
			shared_ptr<objectSprite> temp = shared_ptr<objectSprite>(new objectSprite(templateS));
			temp->setPosition(Vector2f(14, 137));
			winIcons.push_back(temp);
			//won.push_back(6);
			this->metal = true;
		}
		if (wood) {
			shared_ptr<objectSprite> temp = shared_ptr<objectSprite>(new objectSprite(templateS));
			temp->setPosition(Vector2f(142, 73));
			winIcons.push_back(temp);
			//won.push_back(5);
			this->wood = true;
		}
		if (air) {
			shared_ptr<objectSprite> temp = shared_ptr<objectSprite>(new objectSprite(templateS));
			temp->setPosition(Vector2f(78, 9));
			winIcons.push_back(temp);
			//won.push_back(1);
			this->air = true;
		}
		if (quick) {
			shared_ptr<objectSprite> temp = shared_ptr<objectSprite>(new objectSprite(templateS));
			temp->setPosition(Vector2f(142, 9));
			winIcons.push_back(temp);
			//won.push_back(2);
			this->quick = true;
		}
		if (flash) {
			shared_ptr<objectSprite> temp = shared_ptr<objectSprite>(new objectSprite(templateS));
			temp->setPosition(Vector2f(78, 137));
			winIcons.push_back(temp);
			//won.push_back(7);
			this->flash = true;
		}
		if (crash) {
			shared_ptr<objectSprite> temp = shared_ptr<objectSprite>(new objectSprite(templateS));
			temp->setPosition(Vector2f(142, 137));
			winIcons.push_back(temp);
			//won.push_back(8);
			this->crash = true;
		}

		if (wood && crash && flash && air && quick && metal && bubble && heat) {
			shared_ptr<objectSprite> temp = shared_ptr<objectSprite>(new objectSprite(templateS));
			temp->setTexture(backdrop->getTexture());
			temp->setRect(IntRect(255, 0, 45, 45));
			temp->setPosition(Vector2f(78, 73));
			temp->setScale(Vector2f(2, 2));
			winIcons.push_back(temp);

		}
		else {
			//won.push_back(4);
		}

	}

	list<shared_ptr<objectSprite>> getSprites() {

		list<shared_ptr<objectSprite>> sprites;

		for (shared_ptr<objectSprite> s : winIcons) {
			sprites.push_back(s);
		}

		sprites.push_front(backdrop);

		return sprites;
	}

	shared_ptr<text> getText() {
		return name;
	}

	void setPosition(Vector2f pos) {
		backdrop->setPosition(pos);
		position = pos;
		//name->setPosition(Vector2f(position.x, position.y + backdrop->getSize().y));
	}

	Vector2f getPosition() {
		return position;
	}

	Vector2f getSize() {
		return backdrop->getSize();
	}

	

	void setZ(float z, shared_ptr<renderer> instance, shared_ptr<camera> cam) {
		backdrop->setZ(z);

		instance->bObjectCalc(backdrop, cam);

		name->setSize(standardTextSize * backdrop->getCameraScale().x);
		name->setPosition(Vector2f(backdrop->getCameraPosition().x + (textPosFromBottomLeft.x * backdrop->getCameraScale().x), (backdrop->getCameraPosition().y + backdrop->getCameraSize().y) - (textPosFromBottomLeft.y * backdrop->getCameraScale().y)));

		for (shared_ptr<objectSprite> s : winIcons) {
			s->setCameraPosition(Vector2f(backdrop->getCameraPosition().x + (s->getPosition().x * backdrop->getCameraScale().x), backdrop->getCameraPosition().y + (s->getPosition().y * backdrop->getCameraScale().y)));
			s->setCameraScale(backdrop->getCameraScale());
		}
		//backdrop->setPosition(Vector2f(backdrop->getPosition().x + backdrop->getCameraSize().x, backdrop->getPosition().y + backdrop->getCameraSize().y));
	}

	float getZ() {
		return backdrop->getZ();
	}

	float getAngle() {
		return angle;
	}

	void setAngle(float ang) {
		angle = ang;
	}
};