#include "Game.hpp"

Game::Game(sf::Vector2u win_size) 
: scale(3)
, simulation_size(win_size.x / scale, win_size.y / scale)
, board(simulation_size.x, simulation_size.y)
, texture(sf::Vector2u(simulation_size.x, simulation_size.y))
, sprite(texture)
, brush_element_text(pixel_font)
{
    //Variables
    mouse_left_hold = false;
    mouse_right_hold = false;

    //Window
    window = sf::RenderWindow(sf::VideoMode(win_size), "Sandbox", sf::Style::Titlebar | sf::Style::Close);

    //Render Variable
    image.resize(sf::Vector2u(simulation_size.x, simulation_size.y));
    sprite.setScale(sf::Vector2f(scale, scale));

    init_brush_variable();
}

void Game::init_brush_variable()
{
    //Brush Text Data Vector
    brush_element_text_data_vector = {
        {"EMPTY", sf::Color::White},
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

    //Start Brush Data
    brush_size = 5;
    brush_element = Board::ElementType::SAND;


    //Brush Outline Shape
    brush_outline.setSize({10.f, 10.f});
    brush_outline.setScale(sf::Vector2f(scale, scale));
    brush_outline.setFillColor(sf::Color::Transparent);
    brush_outline.setOutlineColor({212, 217, 219, 150});
    brush_outline.setOutlineThickness(0.5f);

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

    brush_outline.setSize({static_cast<float>(brush_size), static_cast<float>(brush_size)});
    brush_outline.setPosition({static_cast<float>(mouse_pos.x) - (brush_size * scale) / 2, static_cast<float>(mouse_pos.y) - (brush_size * scale) / 2});

    //Spawn elements
    if(mouse_left_hold && !mouse_right_hold) 
        board.brush_tool(mouse_pos.y / scale, mouse_pos.x / scale, brush_size, brush_element);
    
    if(mouse_right_hold && !mouse_left_hold)
    {
        board.brush_tool(mouse_pos.y / scale, mouse_pos.x / scale, brush_size, Board::ElementType::EMPTY);
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
                if(mouse->delta > 0) brush_element++;
                else brush_element--;

                //Fire is always last element
                if(brush_element > Board::ElementType::FIRE) brush_element = Board::ElementType::SAND;

                //Sand is first element to draw by brush
                if(brush_element < Board::ElementType::SAND) brush_element = Board::ElementType::FIRE;

                brush_element_text_update();
            }
            else
            {
                if (mouse->delta > 0 && brush_size < 31)
                {
                    brush_size += 2;
                }
                else if (mouse->delta < 0 && brush_size > 5)
                {
                    brush_size -= 2;
                }

            }
        }
    }
}

void Game::brush_element_text_update()
{
    brush_element_text.setString(brush_element_text_data_vector[brush_element].element_name);
    brush_element_text.setFillColor(brush_element_text_data_vector[brush_element].color);
}

void Game::render()
{
    window.clear();

    board_draw();
    window.draw(brush_outline);
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