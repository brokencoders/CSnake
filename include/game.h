#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>

#include "entities.h"

class Game 
{
public:
    Game();             // Create the window and setup the game 
    ~Game();            // Free memory

    void start();   
    void stop();

    void drawMenu();
    void drawPoints();
    void drawGameover();

private:
    void loop();        // Game loop managed internaly
    void handleEvents();// Close window and user key inputs

    bool running;       // Window Status
    bool game_running;  // Game Status

    // Game info 
    int points;
    int speed;

    Snake snake;
    Apple apple;

    // SDL
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Renderer* renderer;

    // TFF 
    TTF_Font* font;
};

#endif
