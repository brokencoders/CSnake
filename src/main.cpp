#include "game.h"

// Global Variabels
int world_size  = 30;
int window_size = 600;

int main()
{
    Game game = Game();
    game.start();
    return 0;
}