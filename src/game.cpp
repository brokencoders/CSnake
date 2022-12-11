#include "game.h"

// Global  Variables
extern int world_size;
extern int window_size;

Game::Game() 
:window(nullptr), screenSurface(nullptr), running(true),
    points(0), speed(150)
{ 

}

Game::~Game() 
{ 
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::start() 
{ 
    // Init SDL and check for errors
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could't initialize! Error: " << SDL_GetError() << "\n";
		return;
	}

	// Create the window and check for errors 
	window = SDL_CreateWindow(
		"Snake",			// Title
		SDL_WINDOWPOS_CENTERED,		// x
		SDL_WINDOWPOS_CENTERED,		// y
		window_size,			// width 
		window_size,			// height
		0				// flags
	);

    if (window == NULL )
    {
	std::cout << "Window could't be created! Error: " << SDL_GetError() << "\n";
	return;
    }

    // Get window surface
    screenSurface = SDL_GetWindowSurface(window);

    running = true;
    loop();
}

void Game::stop() 
{ 
    running = false;
}

void Game::loop()
{
    while(running)
    {
        handleEvents();

        // Draw a Green rectangle on the background
        SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x41, 0xBC, 0x66 ) );
        
        // Update window content!
        SDL_UpdateWindowSurface(window);
    }
}

void Game::handleEvents()
{
    static SDL_Event event;
    
    // Get Every Single Event
    while (SDL_PollEvent(&event))
    {
        // User requests quit
        if (event.type == SDL_QUIT)
            stop();
    }
}
