#include "Vector2i.cpp"
class AbstractWindow {

public:
	enum Type {
		windowed, boarderlessFullscreen, fullscreen
	};

	virtual Vector2i getSize() = 0;

	virtual void close() = 0;
};