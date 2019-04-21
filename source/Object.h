#ifndef OBJECT_H
#define OBJECT_H

#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SpriteLoading.h"
#include <stdlib.h>


constexpr auto OB_WIDTH = 50;
constexpr auto OB_HEIGHT = 50;
//const int ANIMATION_FRAMES = 5;
//SDL_Rect gSpriteClips[ANIMATION_FRAMES];

class Object : public SpriteLoading{
public:
	Object(SDL_Renderer* r, const char* path);
	Object();
	~Object();
	
	void Render(SDL_Renderer* rend, SDL_Texture* t, SDL_Rect* crop, SDL_Rect* rect);
	void resize(SDL_Rect& rect, SDL_Event& ev, bool one);
	void occupied_spot(Object *o, Object *l);
	bool Grab(SDL_Rect &r);
	bool check(Object *o, Object *l);
	bool mouse_inside_bounds_check(SDL_Rect &box);

	//SDL_Rect img_coords[WALK_ANIMATION];
	SDL_Rect rect(SDL_Rect rect); //predefine the rect position
	SDL_Rect _objectRect;
	SDL_Texture *_objectTexture;

};
#endif
