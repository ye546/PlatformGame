#pragma once
#include "Object.h"

class Platforms : public Object
{
public:
	Platforms(SDL_Renderer *r);
	~Platforms();

	SDL_Texture *texture;
	SDL_Rect p_rect;
};
