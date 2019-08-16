#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Engine
{
public:
	Engine();
	~Engine();

	void Initiate();
	//void GameLoop();

	bool IsRunning = true;

	SDL_Window*		Window;
	SDL_Renderer*	Renderer;
};

