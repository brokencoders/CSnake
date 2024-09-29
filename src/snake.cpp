#include "snake.h"

int Snake::window_size;
int Snake::world_size;

void Snake::init(int window, int world)
{
	window_size = window;
	world_size = world;
}

Snake::Snake()
    :direction(RIGHT)
{
    body.push_front(std::make_tuple(world_size / 2, world_size / 2));
}


void Snake::update()
{
    std::tuple<int, int> head = body.front();
    body.pop_back();
    
    int x = std::get<0>(head);
    int y = std::get<1>(head);
    
    if (direction == UP) y --;
    else if(direction == DOWN) y ++;
    else if(direction == RIGHT) x ++;
    else if(direction == LEFT) x --;

    body.push_front(std::make_tuple(x, y));
}

void Snake::draw(SDL_Renderer* renderer)
{
    static double block_size = window_size / world_size;
    for (auto const& i : body) 
    {
        int xx = std::get<0>(i);
        int yy = std::get<1>(i);
        SDL_Rect rect = {xx * block_size, yy * block_size, block_size, block_size};
        SDL_SetRenderDrawColor(renderer, 0xed, 0x2b, 0x2a , 0xFF);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Snake::move(Direction d)
{
    if(2 != std::abs(direction - d))
        direction = d;
}

void Snake::moveAi(int move)
{
    move++;
    int m = ((int) direction + (move - 2));
    if(m <= -1) m = 3;
    if(m >= 4) m = 0;
    direction = (Direction) m; 
}

bool Snake::wallCollision()
{
    auto it = body.begin();
    int head_x = std::get<0>(*it);
    int head_y = std::get<1>(*it);

    if(head_x < 0 || head_y < 0)
        return true;

    if(head_x >= world_size || head_y >= world_size)
        return true;

    return false;
}

int Snake::get_size()
{
    return body.size();
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

bool Snake::appleCollision(Apple apl)
{
    std::tuple<int, int> tail = body.front();
    int x = std::get<0>(tail);
    int y = std::get<1>(tail);

    if (x == apl.x && y == apl.y)
    {
    	std::tuple<int, int> tail = body.front();
	    int x = std::get<0>(tail);
	    int y = std::get<1>(tail);

	    if (direction == UP) y --;
	    else if(direction == DOWN) y ++;
	    else if(direction == RIGHT) x ++;
	    else if(direction == LEFT) x --;

	    body.push_front(std::make_tuple(x, y));
        return true;
    }

    return false;
}

std::list<std::tuple<int, int>> Snake::get_body()
{
    return body;
}
