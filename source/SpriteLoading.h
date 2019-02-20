#pragma once
#include "SDL.h"
#include "SDL_image.h"

#ifndef SPRITELOADING_H
#define SPRITELOADING_H

class SpriteLoading {
public:
	SDL_Texture* SpriteLoader(SDL_Renderer* r, const char* path);
};
#endif
