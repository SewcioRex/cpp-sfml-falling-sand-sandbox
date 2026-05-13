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

    int scale;
    //Board
    Board board;

    //Render
    sf::Image image;
    sf::Texture texture;
    sf::Sprite* sprite;

    //Window
    sf::RenderWindow* window;


    //Variables
    int brush_size;
    Board::ElementType tool;
    bool mouse_left_hold;

    //Functions
    void init_variables(int width, int height);
    void init_window(unsigned int width, unsigned int height);

    void event();

    void board_draw();
    
public:
    //Functions
    void update();
    void render();

    const bool is_running() const;

    //Constructor // Deconstructor
    Game(int w = 800, int h = 600);
    virtual ~Game();

};

