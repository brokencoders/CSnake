#include "entities.h"

#include <iostream>

extern int world_size;
extern int window_size;

Snake::Snake()
    :direction(RIGHT)
{
    body.push_front(std::make_tuple(world_size / 2, world_size / 2));
}

void Snake::move(Direction d)
{
    if(2 != std::abs(direction - d))
        direction = d;
}

bool Snake::collide(Apple apl)
{
    std::tuple<int, int> tail = body.front();
    int x = std::get<0>(tail);
    int y = std::get<1>(tail);

    if (x == apl.x && y == apl.y)
        return true;
    return false;
}

bool Snake::headCollision(Apple apl)
{
    int xx = apl.x;
    int yy = apl.y;

    auto it = body.begin();
    for (; it != body.end(); ++it) {
        int x = std::get<0>(*it);
        int y = std::get<1>(*it);

        if(xx == x && yy == y)
            return true;
    }
    return false;
}

bool Snake::bodyCollision()
{
    auto it = body.begin();
    int head_x = std::get<0>(*it);
    int head_y = std::get<1>(*it);

    for (it++; it != body.end(); ++it) {
        int x = std::get<0>(*it);
        int y = std::get<1>(*it);

        if(head_x == x && head_y == y)
            return true;
    }
    return false;
}

void Snake::grow()
{
    std::tuple<int, int> tail = body.front();
    int x = std::get<0>(tail);
    int y = std::get<1>(tail);

    if (direction == UP) y --;
    else if(direction == DOWN) y ++;
    else if(direction == RIGHT) x ++;
    else if(direction == LEFT) x --;

    body.push_front(std::make_tuple(x, y));
}

void Snake::draw(SDL_Renderer* renderer)
{
    std::tuple<int, int> tail = body.front();
    body.pop_back();
    
    int x = std::get<0>(tail);
    int y = std::get<1>(tail);
    
    // Move
    if (direction == UP) y --;
    else if(direction == DOWN) y ++;
    else if(direction == RIGHT) x ++;
    else if(direction == LEFT) x --;

    // Check if gose outside the world
    if (x < 0) x = world_size; 
    else if (y < 0) y = world_size;
    else if (x > world_size) x = 0;
    else if (y > world_size) y = 0;

    body.push_front(std::make_tuple(x, y));

    static double block_size = window_size / world_size;

    for (auto const& i : body) {
        int xx = std::get<0>(i);
        int yy = std::get<1>(i);

        
        // Color : ED2B2A
        SDL_Rect rect = {xx * block_size, yy * block_size, block_size, block_size}; // x, y, width, height
        SDL_SetRenderDrawColor(renderer, 0xed, 0x2b, 0x2a , 0xFF);
        SDL_RenderFillRect(renderer, &rect);
    }
}

Apple::Apple()
{
    std::random_device rd;                                  // obtain a random number from hardware
    std::uniform_int_distribution<> distr(0, world_size);   // define the range
    std::mt19937 gen(rd());                                 // seed the generator

    x = distr(gen);
    y = distr(gen);
}

void Apple::draw(SDL_Renderer* renderer)
{
    static double block_size = window_size / world_size;
    
    // Color : DDFFBB
    SDL_Rect rect = {x * block_size, y * block_size, block_size, block_size}; // x, y, width, height
    SDL_SetRenderDrawColor(renderer, 0xdd, 0xff, 0xbb, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}