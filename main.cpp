#include "SDL.h"
#include "Object.h"
#include "functions.h"
#include "Player.h"

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
	std::vector<Object*> *objects = new std::vector < Object* >;

	//store the platforms in this vector
	std::vector<Platforms*> *platforms = new std::vector<Platforms*>;
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