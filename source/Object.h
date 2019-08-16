#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SpriteLoading.h"
#include "Engine.h"
#include <stdio.h>
#include <vector>
#include "EngineHeader.h"
#define ObjectSprite "redbox.png"

//looking for engine
extern Engine* engine;

constexpr auto OB_WIDTH = 50;
constexpr auto OB_HEIGHT = 50;
//const int ANIMATION_FRAMES = 5;
//SDL_Rect gSpriteClips[ANIMATION_FRAMES];

class Object : public SpriteLoading{
public:
	Object(SDL_Renderer* r);
	Object();
	~Object();
	
	virtual void Render(SDL_Renderer* r, SDL_Event& ev, bool flags);
	virtual void Resize(SDL_Event& ev, bool flags);
	virtual bool Grab();
	virtual void Collision(Object* ob);
	void KillAllMonsters();
	void CreateNewObject(SDL_Renderer* r);
	bool Check(Object *o, Object *l);
	bool MouseInsideBoundsCheck(SDL_Rect &box);

	int ObjectID;

	//SDL_Rect img_coords[WALK_ANIMATION];
	//SDL_Rect Rect(SDL_Rect rect); //predefine the rect position
	SDL_Rect ObjectRect;
	SDL_Texture *ObjectTexture;
	
	//Vector to store all objects
	std::vector<Object*> m_Objects;
};