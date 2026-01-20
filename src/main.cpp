#include "Game.hpp"

int main()
{
    Game game(1080, 720);

    while(game.isRunning())
    {
        //Update
        game.update();

        //Render
        game.render();
    }
}