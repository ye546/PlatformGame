#include "SpriteLoading.h"
#include <stdio.h>

//the purpose of this function is to load the needed texture
SDL_Texture* SpriteLoading::SpriteLoader(SDL_Renderer* r, const char* path) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(path);
	
	//check if the path does not exist
	if (!surface)
		printf("IMG_Load Error %s\n", SDL_GetError());

	//create texture
	texture = SDL_CreateTextureFromSurface(r, surface);

	//if it failed
	if (!texture)
		printf("Failed to load texture: %s\n", SDL_GetError());

	//free the surface since it isnt needed any longer
	SDL_FreeSurface(surface);

	return texture;
}