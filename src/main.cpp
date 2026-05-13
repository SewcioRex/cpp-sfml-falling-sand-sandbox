#include "Game.hpp"

int main()
{
    Game game(1440, 720);

    while(game.is_running())
    {
        //Update
        game.update();

        //Render
        game.render();
    }
}