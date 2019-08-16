#pragma once
#include "Object.h"
#include "Vector2d.h"

#define PlayerSprite "bluebox.png"

constexpr auto WALK_ANIMATION = 4;

class Player : public Object, public Vector2d
{
public:
	Player(SDL_Renderer* r, int x, int y);
	~Player();
	
	bool Collision(Player* p, SDL_Rect* rect);
	bool Grab();
	void PlayerRender(SDL_Renderer* r);
	void jumping();


	SDL_Texture *PlayerTexture;
	SDL_Rect PlayerRect;
	SDL_Rect crop[WALK_ANIMATION];
};
