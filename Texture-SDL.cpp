#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Texture-abstract.cpp"
#pragma once

class Texture : public textureControl {

	SDL_Surface* texture;


	SDL_Surface* getTexture() {
		return texture;
	}

	void loadTexture() {
		texture = SDL_LoadPNG(this->path.c_str());
	}

	 void initSize() {
		 size = Vector2i(texture->w, texture->h);
	}
};