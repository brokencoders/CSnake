#ifndef ENTITIES_H
#define ENTITIES_H

#include "SDL.h"
#include <list>
#include <tuple>
#include <random>

enum Direction
{
    UP = 0,
    RIGHT, 
    DOWN,
    LEFT
};

struct Apple 
{
    Apple();
    void draw(SDL_Renderer* renderer);
    int x, y;
};


class Snake 
{
public:
    Snake();
    void draw(SDL_Renderer* renderer);
    void move(Direction);
    bool collide(Apple apl);
    bool headCollision(Apple apl);
    bool bodyCollision();
    void grow();
private:
    std::list<std::tuple<int, int>> body;
    Direction direction;
};
#endif