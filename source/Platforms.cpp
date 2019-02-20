#include "Platforms.h"



Platforms::Platforms(SDL_Renderer *r)
{
	Object ob;
	texture = ob.SpriteLoader(r, "greenbox.png");
	p_rect = ob.rect(p_rect);
}


Platforms::~Platforms()
{
}
