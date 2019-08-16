#include "Platforms.h"
Platforms::Platforms(SDL_Renderer *r)
{
	//Apply texture for the object
	this->PlatformTexture = SpriteLoader(r, PlatformSprite);
	int x, y;
	//Set its position to the mouse
	SDL_GetMouseState(&x, &y);
	//set midpoint for circle to be mousepos
	x -= P_WIDTH / 2;
	y -= P_HEIGHT / 2;
	this->PlatformRect = { x, y, P_WIDTH, P_HEIGHT }; //and this will define the position for the object
}

Platforms::Platforms() {
	//do nothing
}

Platforms::~Platforms()
{
}

void Platforms::PlatformRender(SDL_Renderer* r, SDL_Event& ev, bool flags) {
	SDL_RenderCopy(r, this->PlatformTexture, NULL, &this->PlatformRect);

	//also handle other things such as physics and grabbing here
	bool grab = this->Grab();
	if (grab)
	{
		int x(0), y(0);
		//Update objects position to the mouse pos
		SDL_GetMouseState(&x, &y);
		this->PlatformRect.x = (x - this->PlatformRect.w / 2);
		this->PlatformRect.y = (y - this->PlatformRect.h / 2);
		printf("Platform(%i) POS: X:%d Y:%d\n", this->PlatformID, this->PlatformRect.x, this->PlatformRect.y);
	}

	Resize(ev, flags);
}

void Platforms::Resize(SDL_Event& ev,bool flags) {
	if (MouseInsideBoundsCheck(this->PlatformRect) && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		if (flags)
		{
			if (ev.type == SDL_MOUSEWHEEL)
			{
				if (ev.wheel.y > 0)
					this->PlatformRect.w += 5;
				if (ev.wheel.y < 0)
					this->PlatformRect.w -= 5;
				if (ev.wheel.y = 0)
					this->PlatformRect.w = this->PlatformRect.w;
			}
		}

		else if (!flags)
		{
			if (ev.type == SDL_MOUSEWHEEL)
			{
				if (ev.wheel.y > 0)
					this->PlatformRect.h += 10;
				if (ev.wheel.y < 0)
					this->PlatformRect.h -= 10;
				if (ev.wheel.y = 0)
					this->PlatformRect.h = this->PlatformRect.h;
			}
		}
	}

	if (this->PlatformRect.w < 5)
		this->PlatformRect.w = 5;

	if (this->PlatformRect.h < 5)
		this->PlatformRect.h = 5;
}

bool Platforms::Grab() {
	if (MouseInsideBoundsCheck(this->PlatformRect) && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		return true;
	}
	else return false;
}

void Platforms::DestroyAllPlatforms() {
	if (!m_Platforms.empty()) {
		for (auto &i : m_Platforms) {
			SDL_DestroyTexture(i->PlatformTexture);
			i = nullptr;
		}
		printf("Destroyed %i platforms!\n", m_Platforms.size());
		m_Platforms.clear();
	}
	else
		printf("There are no platforms to destroy\n");
}

void Platforms::CreateNewPlatform(SDL_Renderer* r) {
	Platforms* p = new Platforms(r);
	
	//assign an id
	for (int i = 0; i < m_Platforms.size(); i++) {
		if (m_Platforms.empty())
			p->PlatformID += 1;
		else if (m_Platforms.size() > 0) {
			p->ObjectID = m_Platforms[i]->PlatformID += 1;
		}
	}

	m_Platforms.push_back(p);
	printf("Created a new platform!\n");
}