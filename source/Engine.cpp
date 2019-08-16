#include "Engine.h"

Engine::Engine()
{
	
}

Engine::~Engine()
{
}

void Engine::Initiate() {
	//Initiate SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		printf("Failed to load SDL. Error: %s", SDL_GetError());
		getchar();
		return;
	}

	Window = SDL_CreateWindow("Engine", 150, 150, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (Window && Renderer)
		printf("Window and Renderer successfully created!\n");
}
//
//void Engine::GameLoop() {
//	
//}
