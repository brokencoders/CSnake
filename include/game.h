#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include <iostream>

class Game 
{
public:
    Game();             // Create the window and setup the game 
    ~Game();            // Free memory

    void start();   
    void stop();

private:
    void loop();        // Game loop managed internaly
    void handleEvents();// Close window and user key inputs

    bool running;       // Game status

    // Game info 
    int points;
    int speed;

    // SDL
    SDL_Window* window;
    SDL_Surface* screenSurface;
};

#endif
