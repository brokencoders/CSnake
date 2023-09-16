#include "game.h"

// Global  Variables
extern int world_size;
extern int window_size;

Game::Game() 
:window(nullptr), screenSurface(nullptr), running(true),
    points(0), speed(150)
{ 
    bool error = false;

    // Init SDL and check for errors and SDL_tff
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could't initialize! Error: " << SDL_GetError() << std::endl;
		error = true;
	}

    if (TTF_Init() < 0) {
        std::cout << "TTF initialization failed: " << TTF_GetError() << std::endl;
		error = true;
    }


	// Create the window and check for errors 
	window = SDL_CreateWindow(
		"Snake",			        // Title
		SDL_WINDOWPOS_CENTERED,      // x
		SDL_WINDOWPOS_CENTERED,		// y
		window_size,			        // width 
		window_size,			        // height
		0				        // flags
	);

    if (window == NULL )
    {
	    std::cout << "Window could't be created! Error: " << SDL_GetError() << "\n";
		error = true;
    }

    // Get window surface
    screenSurface = SDL_GetWindowSurface(window);

    renderer = SDL_GetRenderer(window);
    if (renderer == nullptr) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
		error = true;
    }

    font = TTF_OpenFont("../font/Debrosee-ALPnL.ttf", 40);
    if(font == nullptr) {
        std::cout << "Unable to load font: Error " << TTF_GetError() << "\n";
		error = true;
    }

    if(!error)
        start();
}

Game::~Game() 
{ 
    delete screenSurface;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::start() 
{
    running = true;
    loop();
}

void Game::stop() 
{ 
    running = false;
}

void Game::drawPoints()
{
    static SDL_Color White = { 255, 255, 255 };
    static SDL_Rect Message_rect = {10, 0, 200, 40}; //create a rect

    std::string text = "Points " + std::to_string(points);

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), White); 
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

}

void Game::drawGameover()
{
    static SDL_Color Red = { 255, 0, 0 };
    static SDL_Rect Message_rect = {50, window_size / 2 - 50, window_size - 100, 100}; //create a rect

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "GAME OVER", Red); 
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}

void Game::drawMenu()
{
    static SDL_Color Red = { 255, 0, 0 };
    static SDL_Rect Message_rect = {50, window_size / 2 - 50, window_size - 100, 100}; //create a rect

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "Press Enter to start", Red); 
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}


void Game::loop()
{
    while(running)
    {
        handleEvents();

        // Draw a Green rectangle on the background
        SDL_SetRenderDrawColor(renderer, 0x41, 0xBC, 0x66, 0xFF);
        
        // Draw filled square
        SDL_Rect squareRect = { 0, 0, window_size, window_size}; // x, y, width, height
        SDL_RenderFillRect(renderer, &squareRect);
        // SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x41, 0xBC, 0x66 ) );
        
        if (game_running)
        {
            // Draw here the game
            snake.draw(renderer);
            apple.draw(renderer);

            if (snake.collide(apple))
            {
                apple = Apple();
                while(snake.headCollision(apple))
                    apple = Apple();
                snake.grow();
                points++;
                speed--;
            }

            if (snake.bodyCollision())
            {
                game_running = false;
                snake = Snake();
                drawGameover();
                SDL_RenderPresent(renderer);
                SDL_Delay(5000);
                continue;
            }
            
            drawPoints();
        
        } else 
            drawMenu();

        SDL_RenderPresent(renderer);
        SDL_Delay(speed);
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
        
        if (event.type == SDL_KEYDOWN)
            // A key was pressed
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    snake.move(UP);
                    break;
                case SDLK_DOWN:
                    snake.move(DOWN);
                    break;
                case SDLK_LEFT:
                    snake.move(LEFT);
                    break;
                case SDLK_RIGHT:
                    snake.move(RIGHT);
                    break;
                case SDLK_RETURN:
                    if (!game_running) game_running = true;
                    break;
            }
    }
}
