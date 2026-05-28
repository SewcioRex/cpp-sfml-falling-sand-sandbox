#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Board.hpp"
#include "Brush.hpp"

#include <iostream>
#include <string>

class Game
{
private:
    int scale;
    sf::Vector2u simulation_size;

    Board board;

    Brush brush;

    //Render
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    //UI
    sf::Font pixel_font;
    sf::Text brush_element_text;

    sf::RenderWindow window;

    //Brush Text
    struct BrushelementTextData
    {
        std::string element_name;
        sf::Color color;
    };
    std::vector<BrushelementTextData> brush_element_text_data_vector;

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

