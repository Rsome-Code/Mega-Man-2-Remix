#include <string>
#include "vector2i.cpp"

using namespace std;

class textureControl {
protected:
	string path;

	Vector2i size;
public:


	textureControl() {
		//cout << "HUH?";
	}
	textureControl(string path) {
		this->path = path;
		loadTexture();
		initSize();
	}

	virtual void loadTexture() = 0;

	void setPath(string text) {
		this->path = text;
		loadTexture();
		initSize();
	}

	void loadFromFile(string text) {
		this->path = text;
		loadTexture();
		initSize();
	}

	virtual void initSize() = 0;


	Vector2i getSize() {
		return this->size;
	}



public:string getPath() {
	return this->path;
}

};