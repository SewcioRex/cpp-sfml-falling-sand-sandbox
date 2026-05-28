#include "Game.hpp"

Game::Game(sf::Vector2u win_size) 
: scale(3)
, brush(scale)
, board(simulation_size.x, simulation_size.y)
, simulation_size(win_size.x / scale, win_size.y / scale)
, texture(sf::Vector2u(simulation_size.x, simulation_size.y))
, sprite(texture)
, brush_element_text(pixel_font)
{
    //Variables
    mouse_left_hold = false;
    mouse_right_hold = false;

    //Window
    window = sf::RenderWindow(sf::VideoMode(win_size), "Sandbox", sf::Style::Titlebar | sf::Style::Close);

    //Render
    image.resize(sf::Vector2u(simulation_size.x, simulation_size.y));
    sprite.setScale(sf::Vector2f(scale, scale));

    init_brush_variable();
}

void Game::init_brush_variable()
{
    //Brush Text Data Vector
    brush_element_text_data_vector = {
        {"ERASE", sf::Color::White},
        {"SAND", sf::Color::Yellow},
        {"ASH", {211, 211, 211, 255}},
        {"GUNPOWDER", {50, 50, 50, 255}},
        {"STONE", {105, 105, 105, 255}},
        {"WOOD", {150, 75, 0, 255}},
        {"WATER", {30, 144, 255, 255}},
        {"GASOLINE", {200, 180, 90, 255}},
        {"ACID", {144, 238, 144, 255}},
        {"HYDROGEN", {200, 220, 255, 100}},
        {"SMOKE", {90, 90, 90, 200}},
        {"FIRE", sf::Color::Red}
    };

    //Font load
    if(!pixel_font.openFromFile("assets/fonts/PixelatedEleganceRegular-ovawB.ttf")) std::cout << "Error: Font error\n";

    //Brush Element Text
    brush_element_text.setCharacterSize(20);
    brush_element_text.setPosition({10, 10});
    brush_element_text_update();
}

const bool Game::is_running() const
{
    return window.isOpen();
}

void Game::update()
{
    event();
    
    board.grid_update();

    update_brush_tool();
}

void Game::update_brush_tool()
{
    sf::Vector2i mouse_pos = {sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y};

    brush.update_outline(mouse_pos, scale);

    //Spawn elements
    if(mouse_left_hold && !mouse_right_hold)
    {
        board.spawn_elements(mouse_pos.y / scale, mouse_pos.x / scale, brush.get_size(), brush.get_element());
    }

    if(!mouse_left_hold && mouse_right_hold)
    {
        board.spawn_elements(mouse_pos.y / scale, mouse_pos.x / scale, brush.get_size(), ElementType::EMPTY);
    }

}

void Game::event()
{
    while(std::optional event = window.pollEvent()) 
    {
        if(event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if(auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if(key->scancode == sf::Keyboard::Scancode::LShift)
            {
                left_shift_down = true;
            }
            else if(key->scancode == sf::Keyboard::Scancode::Space)
            {
                board.is_paused = !board.is_paused;
            }
            else if(key->scancode == sf::Keyboard::Scancode::Right)
            {
                if(board.is_paused) board.can_render_next_frame_in_pause = true;
            }
            else if(key->scancode == sf::Keyboard::Scancode::Delete)
            {
                board.clear_board();
            }
        }
        else if(auto* key = event->getIf<sf::Event::KeyReleased>())
        {
            if(key->scancode == sf::Keyboard::Scancode::LShift) left_shift_down = false;
        }
        else if(auto* key = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if(key->button == sf::Mouse::Button::Left && !mouse_right_hold)
                mouse_left_hold = true;

            else if(key->button == sf::Mouse::Button::Right && !mouse_left_hold)
                mouse_right_hold = true;
        }
        else if(auto* key = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if(key->button == sf::Mouse::Button::Left)
                mouse_left_hold = false;

            else if(key->button == sf::Mouse::Button::Right)
                mouse_right_hold = false;
        }
        else if(auto* mouse = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if(left_shift_down)
            {
                if(mouse->delta > 0)
                {
                    brush.change_element(1);
                } 
                else if(mouse->delta < 0) 
                {
                    brush.change_element(-1);
                }

                brush_element_text_update();
            }
            else
            {
                if (mouse->delta > 0 && brush.get_size() < 31)
                {
                    brush.change_size(2);
                }
                else if (mouse->delta < 0 && brush.get_size() > 5)
                {
                    brush.change_size(-2);
                }

            }
        }
    }
}

void Game::brush_element_text_update()
{
    int brush_element = static_cast<int>(brush.get_element());
    brush_element_text.setString(brush_element_text_data_vector[brush_element].element_name);
    brush_element_text.setFillColor(brush_element_text_data_vector[brush_element].color);
}

void Game::render()
{
    window.clear();

    //Draw
    board_draw();
    brush.draw_outline(window);
    window.draw(brush_element_text);

    window.display();
}

void Game::board_draw()
{
    for(int i = 0; i < board.max_grid_H; i++)
    {
        for(int j = 0; j < board.max_grid_W; j++)
        {
            image.setPixel(sf::Vector2u(j, i), board.grid[i][j].color);
        }
    }

    texture.update(image);

    window.draw(sprite);
}