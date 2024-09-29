#ifndef SNAKE_H
#define SNAKE_H

#include "SDL.h"
#include <list>
#include <tuple>
#include <random>

#include "apple.h"

enum Direction
{
    UP = 0,
    RIGHT, 
    DOWN,
    LEFT
};

class Snake 
{
public:
    Snake();
    void update();
    void draw(SDL_Renderer* renderer);
    void move(Direction);

    void moveAi(int move);

    bool wallCollision();
    bool bodyCollision();
    bool appleCollision(Apple apple);

    std::list<std::tuple<int, int>> get_body();

    int get_size();

    static void init(int window_size, int world_size);

    static int window_size;
    static int world_size;
public:
    std::list<std::tuple<int, int>> body;
    Direction direction;
};


#endif