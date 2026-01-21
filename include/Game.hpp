#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Board.hpp"

#include <iostream>
#include <string>

class Game
{
private:

    //Board
    Board board;

    //Render
    sf::Image image;
    sf::Texture texture;
    sf::Sprite* sprite;

    sf::Clock clock;
    float timer;
    float dt;

    //Window
    sf::RenderWindow* window;


    //Variables

    //Functions
    void initVariables(int width, int height);
    void initWindow(unsigned int width, unsigned int height);

    void event();

    void boardDraw();
    
public:
    //Functions
    void update();
    void render();

    const bool isRunning() const;

    //Constructor // Deconstructor
    Game(int w = 800, int h = 600);
    virtual ~Game();

};

