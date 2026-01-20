#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Board.hpp"

#include <iostream>

class Game
{
private:

    //Board
    Board board;

    //Variables
        //Window
        sf::RenderWindow* window;

    //Functions
    void initVariables();
    void initWindow(unsigned int width, unsigned int height);

    void event();
    
public:
    //Functions
    void update();
    void render();

    const bool isRunning() const;

    //Constructor // Deconstructor
    Game(int w = 800, int h = 600);
    virtual ~Game();

};

