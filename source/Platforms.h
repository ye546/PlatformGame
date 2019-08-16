#pragma once
#include "Object.h"
#define PlatformSprite "greenbox.png"
#define P_WIDTH  100
#define P_HEIGHT 10

class Platforms : public Object
{
public:
	Platforms(SDL_Renderer *r);
	Platforms();
	~Platforms();

	void PlatformRender(SDL_Renderer* r, SDL_Event& ev, bool flags);
	virtual void Resize(SDL_Event& ev, bool flags);
	virtual bool Grab();
	void DestroyAllPlatforms();
	void CreateNewPlatform(SDL_Renderer* r);
	int PlatformID;

	SDL_Texture *PlatformTexture;
	SDL_Rect PlatformRect;

	//Vector to store all platforms
	std::vector<Platforms*> m_Platforms;
};
