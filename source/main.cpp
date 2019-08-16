#include <SDL.h>
#include <chrono>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "Object.h"
#include "Player.h"
#include "SpriteLoading.h"
#include "Platforms.h"
#include "Vector2d.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

static Platforms platform;
static Object object;

//Timer
static struct timer {
	typedef std::chrono::steady_clock clock;
	typedef std::chrono::seconds seconds;

	void reset() {
		//sets start to current time
		start = clock::now();
	}

	unsigned long long seconds_elapsed()const {
		//check elapsed time
		//basically this function > x
		return std::chrono::duration_cast<seconds>(clock::now() - start).count();
	}

private:
	clock::time_point start = clock::now();
}t;


void Movement(Player* player,	// the player
	std::vector<Object*> &ob,	//objects player could collide with
	SDL_Event& ev				//event to handle the keypressing
);
void handleEvents(
	SDL_Event &event,
	bool &isRunning,
	bool &handle,
	Player* player,
	SDL_Renderer* Renderer
);
void Collision(SDL_Rect &rect1, SDL_Rect &rect2);
void MonsterThinkingFunction(Object* monsters, Player* player);

static bool falling;

int main(int agrc, char** argv) {

	engine->Initiate();

	bool handle = 0;
	int frame = 0;
	const int fps = 60;
	const int frameDelay = (1000 / fps); //= 0.0060
	Uint32 start; //this will initialize the counter for SDL_Delay()
	int frameTime;

	SDL_Event event;

	Player* player = new Player(engine->Renderer, 50, 50);

	while (engine->IsRunning)
	{
		//this is for mouse event, not sure if I need this
		SDL_PumpEvents();

		//initiate counting to later use SDL_Delay to achieve desired fps
		start = SDL_GetTicks();

		//handling all the key events
		while (SDL_PollEvent(&event))
		{
			handleEvents(event, engine->IsRunning, handle, player, engine->Renderer);
		}
		
		//set background to white
		SDL_SetRenderDrawColor(engine->Renderer, 255, 255, 255, 255);

		player->PlayerRender(engine->Renderer);

		//draw all platforms
		for (int i = 0; i < platform.m_Platforms.size(); i++) {
			platform.m_Platforms[i]->PlatformRender(engine->Renderer, event, handle);
		}	

		//Draw all the monsters(objects)
		for (int i = 0; i < object.m_Objects.size(); i++) {
			object.m_Objects[i]->Render(engine->Renderer, event, handle);
			object.m_Objects[i]->Collision(object.m_Objects[i]);
			MonsterThinkingFunction(object.m_Objects[i], player);
		}
		

		//present the stuff to render
		SDL_RenderPresent(engine->Renderer);

		//clear engine->Renderer
		SDL_RenderClear(engine->Renderer);

		//get framtime
		frameTime = SDL_GetTicks() - start;
		//delay if frameDelay > time
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}

	//destroy all created textures
	for (int i = 0; i < object.m_Objects.size(); i++)
	{
		SDL_DestroyTexture(object.m_Objects[i]->ObjectTexture);
	}

	for (int i = 0; i < platform.m_Platforms.size(); i++) {
		SDL_DestroyTexture(platform.m_Platforms[i]->PlatformTexture);
	}
	
	SDL_DestroyRenderer(engine->Renderer);
	SDL_DestroyWindow(engine->Window);
	printf("\nERROR:%s\n", SDL_GetError());
	engine = nullptr;
	SDL_Quit();
	return 0;
}

void Movement(Player* player, std::vector<Object*> &ob, SDL_Event &ev)
{
	bool flags[4] = { 0 };

	switch (ev.type)
	{
		SpriteLoading sp;
	case SDL_KEYDOWN:
		switch (ev.key.keysym.sym) {
		case SDLK_w:
			flags[0] = 1;
			break;
		case SDLK_d:
			flags[1] = 1;
			break;
		case SDLK_s:
			flags[2] = 1;
			break;
		case SDLK_a:
			flags[3] = 1;
			break;
		}
		break;
	case SDL_KEYUP:
		switch (ev.key.keysym.sym) {
		case SDLK_w:
			flags[0] = 0;
			break;
		case SDLK_d:
			flags[1] = 0;
			break;
		case SDLK_s:
			flags[2] = 0;
			break;
		case SDLK_a:
			flags[3] = 0;
			break;
		case SDLK_SPACE:
			//falling = false;
			//player->jumping();
			//t.reset();
			//printf("the player jumped!\n");
			break;
		}
		break;
	}

	if (flags[0])
	{
		//walk up
		player->PlayerRect.y -= 5;
		for (int i = 0; i < ob.size(); i++) {
			Collision(player->PlayerRect, ob[i]->ObjectRect);
		}
	}

	if (flags[1])
	{
		//walk right
		player->PlayerRect.x += 5;
		for (int i = 0; i < ob.size(); i++) {
			Collision(player->PlayerRect, ob[i]->ObjectRect);
		}
	}

	if (flags[2])
	{
		//walk down
		player->PlayerRect.y += 5;
		for (int i = 0; i < ob.size(); i++) {
			Collision(player->PlayerRect, ob[i]->ObjectRect);
		}
	}

	if (flags[3])
	{
		//walk left
		player->PlayerRect.x -= 5;
		for (int i = 0; i < ob.size(); i++) {
			Collision(player->PlayerRect, ob[i]->ObjectRect);
		}
	}
}

void Collision(SDL_Rect &rect1, SDL_Rect &rect2) {
	if (SDL_HasIntersection(&rect1, &rect2))
	{
		//if (rect1.x >= (rect2.x + rect2.w))
		//{
		//	//return false;
		//	rect1.x += 5;
		//}

		//if ((rect1.x + rect1.w) <= rect2.x)
		//{
		////return false;
		//rect1.x -= 5;
		//}

		if (rect1.y >= (rect2.y + rect2.h))
		{
			//return false;
			rect1.y += 5;
		}

		if ((rect1.y + rect1.h) <= rect2.y)
		{
			//return false;
			rect1.y -= 5;
		}
	}
}

void MonsterThinkingFunction(Object* monsters, Player* player)
{
	//have the object's middlepoint move towards the players middlepoint
	if ((monsters->ObjectRect.x + monsters->ObjectRect.w / 2) > (player->PlayerRect.x + player->PlayerRect.w / 2))
		monsters->ObjectRect.x--;
	if ((monsters->ObjectRect.x + monsters->ObjectRect.w / 2) < (player->PlayerRect.x + player->PlayerRect.w / 2))
		monsters->ObjectRect.x++;
	if ((monsters->ObjectRect.y + monsters->ObjectRect.h / 2) > (player->PlayerRect.y + player->PlayerRect.h / 2))
		monsters->ObjectRect.y--;
	if ((monsters->ObjectRect.y + monsters->ObjectRect.h / 2) < (player->PlayerRect.y + player->PlayerRect.h / 2))
		monsters->ObjectRect.y++;
}

void handleEvents
(
	SDL_Event &event,
	bool &isRunning,
	bool &handle,
	Player* player,
	SDL_Renderer* Renderer
)
{
	if (event.type == SDL_QUIT) 
		engine->IsRunning = 0;

	Movement(player, object.m_Objects, event);
	switch (event.type)
	{
	case SDL_MOUSEBUTTONUP:
		switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:
			//Create a new monster
			object.CreateNewObject(engine->Renderer);
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym) {
		case SDLK_LCTRL:
			//Create new platform
			platform.CreateNewPlatform(engine->Renderer);
			break;
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_1:
			handle ^= 1;
			printf("Rescaling height\n", handle);
			break;
		case SDLK_2:
			handle ^= 0;
			printf("Rescaling width\n", handle);
			break;
		case SDLK_4:
			//töm Object vectorn
			object.KillAllMonsters();
			break;
		case SDLK_5:
			platform.DestroyAllPlatforms();
			break;
		}
	}
}