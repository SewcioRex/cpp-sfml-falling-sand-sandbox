#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Board.hpp"

#include <iostream>
#include <string>

class Game
{
private:
    int scale;
    sf::Vector2u simulation_size;

    //BoardS
    Board board;

    //Render
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    //UI
    sf::RectangleShape brush_outline;
    sf::Font pixel_font;
    sf::Text brush_element_text;

    //Window
    sf::RenderWindow window;

    //Brush
    struct BrushelementTextData
    {
        std::string element_name;
        sf::Color color;
    };
    std::vector<BrushelementTextData> brush_element_text_data_vector;

    int brush_size;
    int brush_element;


    //Variables
    bool mouse_left_hold;
    bool mouse_right_hold;
    bool left_shift_down;

    //Functions
        //Init functions
    void init_brush_variable();

    void update_brush_tool();


    void event();
    void board_draw();
    void brush_element_text_update();
    
public:
    //Functions
    void update();
    void render();

    const bool is_running() const;

    //Constructor // Deconstructor
    Game(sf::Vector2u win_size = {500, 500});
};

