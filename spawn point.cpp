#include "enemy.cpp"
#pragma once

enum SpawnType {
	START, END
};

class SpawnPoint : public object{
protected:
	
	string obName;

public:
	SpawnPoint(){}
	SpawnPoint(string objectName) {

		

		Texture* tex = new Texture();
		tex->loadFromFile("assets\\" + objectName + " spawn icon.png");

		sprite = new objectSprite(tex);
		
		obName = objectName;


		setCode(objectName + "-spawn");
		


	}
	
};