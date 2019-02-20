#pragma once
#include "Object.h"

constexpr auto WALK_ANIMATION = 4;

class Player : public Object
{
public:
	Player(SDL_Renderer* r, int x, int y);
	~Player();
	SDL_Texture *texture;
	SDL_Rect p_rect;
	SDL_Rect crop[WALK_ANIMATION];
	void jumping();
};