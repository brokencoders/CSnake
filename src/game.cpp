#include "game.h"
#include <cstdlib> 

Game::Game(int window_size, int world_size, bool AI)
	:window_size(window_size), world_size(world_size), 
	AI(AI), render(true), speed(50), points(0), agent(world_size * world_size), best_score(0)
{
	if(init()) 
	{
		Apple::init(window_size, world_size);
		snake.init(window_size, world_size);
		start();
	}
}

Game::~Game()
{
    if (screenSurface) SDL_FreeSurface(screenSurface);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();
}

bool Game::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could't initialize! Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() < 0) 
	{
        std::cout << "TTF initialization failed: " << TTF_GetError() << std::endl;
		return false;
    }

    window = SDL_CreateWindow(
		"Snake",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_size,
		window_size,
		0
	);


    if (window == NULL )
    {
	    std::cout << "Window could't be created! Error: " << SDL_GetError() << "\n";
		return false;
    }

    screenSurface = SDL_GetWindowSurface(window);

    renderer = SDL_GetRenderer(window);
    if (renderer == nullptr) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
		return false;
    }

    font = TTF_OpenFont("../font/BodoniFLF-Bold.ttf", 40);
    if(font == nullptr) {
        std::cout << "Unable to load font: Error " << TTF_GetError() << "\n";
		return false;
    }

    return true;
}

void Game::initGame()
{
	if(points > best_score)
		best_score = points;
	points = 0;
	apple1 = Apple();
	/*
	apple2 = Apple();
	apple3 = Apple();
	apple4 = Apple();
	apple5 = Apple();
	*/
	snake = Snake();
}

void Game::start()
{
	running = true;
	initGame();
	loop();
}

void Game::stop()
{
	running = false;
}

void Game::drawPoints()
{
	static SDL_Color White = { 255, 255, 255 };
    static SDL_Rect Message_rect = {10, 0, 200, 40};
    std::string text = "Points " + std::to_string(points);
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), White); 
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}

void Game::deadMenu()
{
	bool run = true;

	SDL_SetRenderDrawColor(renderer, 0x41, 0xBC, 0x66, 0xFF);
    SDL_Rect squareRect = { 0, 0, window_size, window_size};
    SDL_RenderFillRect(renderer, &squareRect);

	static SDL_Color Red = { 255, 0, 0 };
    static SDL_Rect Message_rect = {50, window_size / 2 - 50, window_size - 100, 100};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "Press Enter to start", Red); 
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    SDL_RenderPresent(renderer);

    static SDL_Event event;
    while(run)
    {
    	while (SDL_PollEvent(&event))
    	{
    		if (event.type == SDL_QUIT)
        	{
            	stop();
        		run = false;
        	}
        	if (event.type == SDL_KEYDOWN)
	            switch (event.key.keysym.sym)
	        	{
	                case SDLK_RETURN:
	                    run = false;
	                    break;
	        	}
    	}
	}
}

void Game::loop()
{
	int reward = 0;

	std::vector<int> state;
	std::vector<int> new_state = get_state();
	int action = 0;
	int count = 20;
	int t = 0;
	while(running)
	{
		handleEvents();

		if(AI)
		{
			state = new_state;
			action = agent.get_action(state);
			snake.moveAi(action);
		}
		
		snake.update();

		if(render)
		{
			SDL_SetRenderDrawColor(renderer, 0x41, 0xBC, 0x66, 0xFF);
            SDL_Rect squareRect = { 0, 0, window_size, window_size};
            SDL_RenderFillRect(renderer, &squareRect);

            apple1.draw(renderer);
            /*
            apple2.draw(renderer);
            apple3.draw(renderer);
            apple4.draw(renderer);
            apple5.draw(renderer);
            */
            snake.draw(renderer);
            drawPoints();
		}

		reward = -0.3;
		if(snake.appleCollision(apple1))
		{
			points++;
			if(AI) reward = 2;
			apple1 = Apple();
		}

		/*
		if(snake.appleCollision(apple2))
		{
			points++;
			if(AI) reward = 2;
			apple2 = Apple();
		}

		if(snake.appleCollision(apple3))
		{
			points++;
			if(AI) reward = 2;
			apple3 = Apple();
		}

		if(snake.appleCollision(apple4))
		{
			points++;
			if(AI) reward = 2;
			apple4 = Apple();
		}

		if(snake.appleCollision(apple5))
		{
			points++;
			if(AI) reward = 2;
			apple5 = Apple();
		}
		*/

		if(snake.wallCollision() || snake.bodyCollision())
		{
			if(render && !AI)
				deadMenu();
			
			render = false;
			if(count == 20)
			{
				render = true;
				count = 0;
				t++;
			}
			system("clear");
			std::cout << t << " " << count << " " << points << " " << best_score << std::endl;
			count++;
			
			initGame();
			if(AI) reward = -50 / snake.get_size();
			agent.update();
		}

		if(AI)
		{
			new_state = get_state();
			agent.train(state, new_state, action, reward);
		}

		if(render)
        {
            SDL_RenderPresent(renderer);
            SDL_Delay(speed);
        }
	}
}

void Game::handleEvents()
{
    static SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            stop();

        if (event.type == SDL_KEYDOWN && !AI)
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
    		}
    }
}

std::vector<int> Game::get_state()
{
    std::vector<int> v(world_size * world_size, 0);
    v[apple1.x + apple1.y * world_size] = 1;
    /*
    v[apple2.x + apple2.y * world_size] = 1;
    v[apple3.x + apple3.y * world_size] = 1;
    v[apple4.x + apple4.y * world_size] = 1;
    v[apple5.x + apple5.y * world_size] = 1;
    */
    std::list<std::tuple<int, int>> snake_body = snake.get_body();

    for (auto s : snake_body) {
        int index = std::get<0>(s) + std::get<1>(s) * world_size;
        if (index < v.size()) {
            v[index] = 2;
        }
    }

    return v;
}