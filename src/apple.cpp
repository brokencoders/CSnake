#include "apple.h"

std::random_device Apple::rd;                                  
std::uniform_int_distribution<> Apple::distr; 
std::mt19937 Apple::gen;                                 
double Apple::block_size;

Apple::Apple()
{
    x = distr(gen);
    y = distr(gen);
}

void Apple::draw(SDL_Renderer* renderer)
{   
    SDL_Rect rect = {x * block_size, y * block_size, block_size, block_size};
    SDL_SetRenderDrawColor(renderer, 0xdd, 0xff, 0xbb, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}

void Apple::init(int window_size, int world_size)
{
	block_size = window_size / world_size;
    distr = std::uniform_int_distribution<>(0, world_size-1);
    gen = std::mt19937(rd());   
}