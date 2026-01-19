#include "Game.hpp"

int main()
{
    Game game;

    while(game.isRunning())
    {
        //Update
        game.update();

        //Render
        game.render();
    }
}