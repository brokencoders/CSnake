#ifndef GAME_H
#define GAME_H

#include <iostream>

#include "SDL.h"
#include "SDL2/SDL_ttf.h"

#include "apple.h"
#include "snake.h"
#include "agent.h"

class Game
{
public:
	Game(int window_size, int world_size, bool AI);
	~Game();

	bool init();
	void initGame();
	void start();
	void stop();
	void loop();
	void handleEvents();

	void deadMenu();
	void drawPoints();

	std::vector<int> get_state();

private:
	int world_size;
	int window_size;
	
	bool AI;
	bool running;
	bool render;

	int speed;
	int points;
	int best_score;

	Apple apple1;
	Apple apple2;
	Apple apple3;
	Apple apple4;
	Apple apple5;
	
	Snake snake;

	Agent agent;

	SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Renderer* renderer;

    TTF_Font* font;
};

#endif