#include "SDL.h"
#include "Object.h"
#include "Player.h"
#include "SpriteLoading.h"
#include "functions.h"
#include "Platforms.h"
#include <chrono>
#include <vector>
#include <cstdio>

#define VEC_OBJ std::vector<Object*>
#define VEC_PLAT std::vector<Platforms*>

static bool falling;
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

void Movement(Player* player,				// the player
			VEC_OBJ& ob,	//objects player could collide with
			SDL_Event& ev				//event to handle the keypressing
);

Object* NewObject(SDL_Renderer* r);
Platforms *NewPlatform(SDL_Renderer *r);

//Gravity
void Gravity(SDL_Rect *x, bool &falling);

//returns true is there is a collision
bool Collision(SDL_Rect &rect1, SDL_Rect &rect2);

//crop rigging for specific image, not ideal because it only works for 1 image
void crops(SDL_Rect* crop);

//Monster
void MonsterThinkingFunction(Object* monsters, Player* player);



//Events
void handleEvents(SDL_Event &event,
	bool &isRunning,
	bool &handle,
	Player* player,
	VEC_OBJ &objects,
	VEC_PLAT &platforms,
	SDL_Renderer* renderer);

void create_and_place_platform(SDL_Renderer* r, VEC_PLAT& pt);

void DrawThePlayer(Player *player, SDL_Event &event, SDL_Renderer *renderer, bool &falling);

void DrawMonsters(VEC_OBJ &v, Player *player, SDL_Renderer *r, SDL_Event &event, bool &handle);

void DrawPlatforms(VEC_PLAT &v, Player *player, SDL_Renderer *r, SDL_Event &event, bool &handle);



bool is_touching_platform(VEC_PLAT &v, SDL_Rect *player);

void isOnPlatform(SDL_Rect *player, SDL_Rect *platform);

//Move
void MovingAnObject(Player &p, SDL_Rect &ob);

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(int agrc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("failed to load SDL %s", SDL_GetError());
		getchar();
		return 1;
	}

	bool isRunning = true;
	bool handle = 0;
	//bool falling = true;

	int frame = 0;

	const int fps = 60;
	const int frameDelay = (1000 / fps); //= 0.0060
	Uint32 start; //this will initialize the counter for SDL_Delay()
	int frameTime;

	//create a window
	SDL_Window* window = SDL_CreateWindow("Engine", 150, 150, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	//create a renderer which we'll use to display everything with
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |SDL_RENDERER_PRESENTVSYNC);

	//the player
	Player* player = new Player(renderer, 50, 50);
	//crops(player->crop);
	//^the player

	//store the objects in this vector
	VEC_OBJ *objects = new VEC_OBJ;

	//store the platforms in this vector
	VEC_PLAT *platforms = new VEC_PLAT;
	SDL_Event event;

	//&vector to our Object vector
	auto &vRef = (*objects);
	auto &pRef = (*platforms);

	create_and_place_platform(renderer, pRef);

	//turn on gravity at the beginning
	falling = true;

	while (isRunning)
	{
		//this is for mouse event, not sure if I need this
		SDL_PumpEvents();

		//initiate counting to later use SDL_Delay 
		//to achieve desired fps
		start = SDL_GetTicks();


		//handling all the key events
		while (SDL_PollEvent(&event))
		{
			handleEvents(event, isRunning, handle, player, vRef, pRef, renderer);
			
			//förbättra hoppfunktionen, för den suger verkligen cancerdase.

			//check if spacebar is pressed
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_SPACE) {
					
				}
			}
		}
		
		//set background to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		DrawThePlayer(player, event, renderer, falling);
		
		//xd(vRef);

		//Draw all the monsters
		DrawMonsters(vRef, player, renderer, event, handle);

		//draw all platforms
		DrawPlatforms(pRef, player, renderer, event, handle);

		//retrive status of the player, etc is he falling or not?
		falling = is_touching_platform(pRef, &player->p_rect);

		////increase frame of player
		//frame++;
		//if (frame == 4)
		//	frame = 0;
		
		//present the stuff to render
		SDL_RenderPresent(renderer);

		//clear renderer
		SDL_RenderClear(renderer);

		//get framtime
		frameTime = SDL_GetTicks() - start;

		//delay if frameDelay > time
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}

	//destroy all created textures
	for (int i = 0; i < vRef.size(); i++)
	{
		SDL_DestroyTexture(vRef[i]->_objectTexture);
	}
	//delete[] objects;
	//delete[] platforms;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	printf("\n%s\n", SDL_GetError());
	SDL_Quit();
	return 0;
}


void Movement(Player* player, VEC_OBJ &ob, SDL_Event &ev)
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
			falling = false;
			player->jumping();
			t.reset();
			printf("the player jumped!\n");
			break;
		}
		break;
	}

	if (flags[0])
	{
		//walk up
		player->p_rect.y -= 5;
		for (int i = 0; i < ob.size(); i++) {
			if (Collision(player->p_rect, ob[i]->_objectRect))
				ob[i]->_objectRect.y -= 5;
		}
	}

	if (flags[1])
	{
		//walk right
		player->p_rect.x += 5;
		for (int i = 0; i < ob.size(); i++) {
			if (Collision(player->p_rect, ob[i]->_objectRect))
				ob[i]->_objectRect.x += 5;
		}
	}

	if (flags[2])
	{
		//walk down
		player->p_rect.y += 5;
		for (int i = 0; i < ob.size(); i++) {
			if (Collision(player->p_rect, ob[i]->_objectRect))
				ob[i]->_objectRect.y += 5;
		}
	}

	if (flags[3])
	{
		//walk left
		player->p_rect.x -= 5;
		for (int i = 0; i < ob.size(); i++) {
			if (Collision(player->p_rect, ob[i]->_objectRect))
				ob[i]->_objectRect.x -= 5;//push the block
		}
	}
}

void Gravity(SDL_Rect *x, bool& falling) {

	if (falling) {
		x->y += 5; //just keep falling...

		//if the player falls outside the screen height, reset his position on the Y axis
		if ((x->y + x->h) >= 720) //50 in this case is the default height and witdh for the rect
			x->y = 0;
	}
}

bool Collision(SDL_Rect& rect1, SDL_Rect& rect2) {
	//if all of these are false 
	//then there cannot be a collision
	if (rect1.x >= (rect2.x + rect2.w))
		return false;
	if ((rect1.x + rect1.w) <= rect2.x)
		return false;
	if (rect1.y >= (rect2.y + rect2.h))
		return false;
	if ((rect1.y + rect1.h) <= rect2.y)
		return false;

	return true;
}

//void crops(SDL_Rect* crop) {
//	crop[0] = { 0 , 0, 64, 205 };
//	crop[1] = { 64, 0, 64, 205 };
//	crop[2] = { 128, 0, 64, 205 };
//	crop[3] = { 196, 0, 64, 205 };
//}

void MonsterThinkingFunction(Object* monsters, Player* player)
{
	//have the object's middlepoint move towards the players middlepoint
	if ((monsters->_objectRect.x + monsters->_objectRect.w / 2) > (player->p_rect.x + player->p_rect.w / 2))
		monsters->_objectRect.x--;
	if ((monsters->_objectRect.x + monsters->_objectRect.w / 2) < (player->p_rect.x + player->p_rect.w / 2))
		monsters->_objectRect.x++;
	if ((monsters->_objectRect.y + monsters->_objectRect.h / 2) > (player->p_rect.y + player->p_rect.h / 2))
		monsters->_objectRect.y--;
	if ((monsters->_objectRect.y + monsters->_objectRect.h / 2) < (player->p_rect.y + player->p_rect.h / 2))
		monsters->_objectRect.y++;
}

Object* NewObject(SDL_Renderer* r)
{
	Object* ob = new Object;
	ob->_objectRect = ob->rect(ob->_objectRect);
	ob->_objectTexture = ob->SpriteLoader(r, "redbox.png");

	return ob;
}

void handleEvents
(
	SDL_Event &event,
	bool &isRunning,
	bool &handle,
	Player* player,
	VEC_OBJ &objects,
	VEC_PLAT &platforms,
	SDL_Renderer* renderer)
{
	if (event.type == SDL_QUIT) isRunning = 0;

	Movement(player, objects, event);

	switch (event.type)
	{
	case SDL_MOUSEBUTTONUP:
		//create new wall here
		switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:
			objects.push_back(NewObject(renderer));
			printf("created an Object: sizeof objects (%i)\n", objects.size());
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym) {
		case SDLK_LCTRL:
			platforms.push_back(NewPlatform(renderer));
			printf("New platform created\n");
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
			if (!objects.empty()) {
				for (int i = 0; i < objects.size(); i++) {
					SDL_DestroyTexture(objects[i]->_objectTexture);
					objects[i] = nullptr;
				}
				printf("Destroyed %i blocks\n", objects.size());
				objects.clear();
			}
			else
				printf("No blocks to destroy!\n");
			break;
		case SDLK_5:
			if (!platforms.empty()) {
				for (auto &i : platforms) {
					SDL_DestroyTexture(i->texture);
					i = nullptr;
				}
				int i = platforms.size();
				printf("Deleted %i platforms", i);
				platforms.clear();
			}
			else
				printf("no platforms to destroy\n");
			break;
		}
	}
}

void create_and_place_platform(SDL_Renderer* r, VEC_PLAT &pt) {

	for (int i = 0; i < 4; i++) {
		Platforms* newOb = NewPlatform(r);
		newOb->_objectRect.x = (i + 50);
		newOb->_objectRect.y = (i + 50);
		pt.push_back(newOb);
	}

}

void DrawThePlayer(Player *player, SDL_Event &event, SDL_Renderer *renderer, bool &falling) {
	//do the rendering here

	player->Render(renderer, player->texture, NULL, &player->p_rect);

	//allow for grabbing
	MovingAnObject(*player, player->p_rect);

	if (falling == false) {
		if (t.seconds_elapsed() > 2)
			falling = true;
	}

	//if the player is not touching a platform, 
	//then we call gravity and apply it on him
	if (falling) {
		Gravity(&player->p_rect, falling);
	}
}

//my attmept of deleting an object if they're inside each other
//void xd(std::vector<Object*> &v) {
//	for (int i = 0; i < v.size(); i++) {
//		for (int j = 0; j < v.size(); j++) {
//			if (&v[i]->_objectRect.x == &v[j]->_objectRect.x &&
//				&v[i]->_objectRect.y == &v[j]->_objectRect.y
//				) 
//			{
//				SDL_DestroyTexture(v[i]->_objectTexture);
//				v[i] = nullptr;
//				v.erase(v.begin() + i);
//				printf("deleted Object at same which were at same coord!\n");
//			}
//		}
//	}
//}

//draw all monsters from the vector of monsters
void DrawMonsters(VEC_OBJ &v, Player *player, SDL_Renderer *r, SDL_Event &event, bool &handle) {
	for (int i = 0; i < v.size(); i++) {
		//render all the objects
		v[i]->Render(r, v[i]->_objectTexture, NULL, &v[i]->_objectRect);
		//call thinking for monsters
		//MonsterThinkingFunction(v[i], player);

		//allow grabbing for all the objects
		MovingAnObject(*player, v[i]->_objectRect);

		//allow re-scaling
		v[i]->resize(v[i]->_objectRect, event, handle);

		//check if the object collides with the player
		//if it does we simply destroy the object
		if (SDL_HasIntersection(&player->p_rect, &v[i]->_objectRect)) {
			SDL_DestroyTexture(v[i]->_objectTexture);

			v[i] = nullptr;

			v.erase(v.begin() + i);

			printf("destroyed 1 monster\n");
		}
	}
}

//draw all the platforms
void DrawPlatforms(VEC_PLAT &v, Player *player, SDL_Renderer *r, SDL_Event &event, bool &handle) {
	for (int i = 0; i < v.size(); i++) {
		//call render func
		v[i]->Render(r, v[i]->texture, NULL, &v[i]->p_rect);

		//allow grabbing
		MovingAnObject(*player, v[i]->p_rect);

		//allow re-scaling
		v[i]->resize(v[i]->p_rect, event, handle);

		//check if the player is colliding with the platform
		if (SDL_HasIntersection(&player->p_rect, &v[i]->p_rect)) {
			isOnPlatform(&player->p_rect, &v[i]->p_rect);
		}

	}
}

Platforms *NewPlatform(SDL_Renderer *r) {
	Platforms *p = new Platforms(r);
	return p;
}

//this functions checks if the player is touching a paltform, wether it's its sides or top or bottom
bool is_touching_platform(VEC_PLAT &v, SDL_Rect *player) {
	//just to check if he is touching the platform, I

	for (auto &i : v) {
		if (SDL_HasIntersection(player, &i->p_rect)) {
			return false;
		}
	}
	return true;
}

//this function makes sure that if the player is above and he touches the platform
//if he does he stays afloat
void isOnPlatform(SDL_Rect *player, SDL_Rect *platform) {
	//if the player is ontop of platform
	//push him upwards I suppose
	if ((player->y + player->h) >= platform->y) {
		player->y -= 5;
	}
}

void MovingAnObject(Player &p, SDL_Rect &ob) {
	int x(0), y(0);

	if (p.Grab(ob) == true)
	{
		//Update objects position to the mouse pos
		SDL_GetMouseState(&x, &y);
		ob.x = (x - ob.w / 2);
		ob.y = (y - ob.h / 2);
		printf("POS: X:%d Y:%d", ob.x, ob.y);
	}
}
