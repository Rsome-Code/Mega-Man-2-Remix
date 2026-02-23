#include "enemy.cpp"
#pragma once

enum SpawnType {
	START, END
};

class SpawnPoint : public GameObject{
protected:
	
	string obName;

public:
	SpawnPoint(){}
	SpawnPoint(string objectName) {

		

		shared_ptr<Texture> tex = shared_ptr<Texture> (new Texture());
		tex->loadFromFile("assets\\" + objectName + " spawn icon.png");

		sprite = shared_ptr<objectSprite>(new objectSprite(tex));
		
		obName = objectName;


		setCode(objectName + "-spawn");
		


	}
	
};