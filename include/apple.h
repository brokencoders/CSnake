#ifndef APPLE_H
#define APPLE_H

#include "SDL.h"

#include <list>
#include <tuple>
#include <random>

class Apple 
{
public:
	Apple();
	void draw(SDL_Renderer* renderer);

	static void init(int window_size, int world_size);

public:
	int x, y;

	static std::random_device rd;                                  
    static std::uniform_int_distribution<> distr; 
    static std::mt19937 gen;                                 
    static double block_size;
};

#endif