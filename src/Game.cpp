#include "Game.hpp"

void Game::initVariables(int width, int height)
{
    this->window = nullptr;
    this->sprite = nullptr;

    scale = 2;

    image.resize(sf::Vector2u(width / scale, height / scale));

    timer = 0.f;
    brush_size = 3;
    tool = 0;
    mouse_left_hold = false;

    if(!texture.resize(sf::Vector2u(width / scale, height / scale))) std::cout << "Error: Texture error\n";
    
    this->sprite = new sf::Sprite(texture);
    this->sprite->setScale(sf::Vector2f(scale, scale));

}

void Game::initWindow(unsigned int width, unsigned int height)
{
    this->window = new sf::RenderWindow(sf::VideoMode({width, height}), "Sandbox", sf::Style::Titlebar | sf::Style::Close);
}

const bool Game::isRunning() const
{
    return this->window->isOpen();
}

void Game::update()
{
    event();
    
    board.gridUpdate();

    if(mouse_left_hold)
    {
        int mouse_X = (sf::Mouse::getPosition(*this->window).x) / scale;
        int mouse_Y = (sf::Mouse::getPosition(*this->window).y) / scale;

        board.brushTool(mouse_Y, mouse_X, brush_size, tool);
    }

    /*dt = clock.restart().asSeconds();
    timer += dt;

    if(timer >= 0.001f)
    {
        

        timer = 0.f;
    }*/
    
    
}

void Game::event()
{
    while(std::optional event = this->window->pollEvent())
    {
        if(event->is<sf::Event::Closed>())
        {
            window->close();
        }
        else if(auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if(key->scancode == sf::Keyboard::Scancode::T)
            {
                int mouse_X = (sf::Mouse::getPosition(*this->window).x) / scale;
                int mouse_Y = (sf::Mouse::getPosition(*this->window).y) / scale;

                std::string element_type;
                if(board.grid[mouse_Y][mouse_X].elementType == Board::ElementType::EMPTY) element_type = "Empty";
                else if(board.grid[mouse_Y][mouse_X].elementType == Board::ElementType::SAND) element_type = "Sand";

                std::string color;
                if(board.grid[mouse_Y][mouse_X].color == sf::Color::Black) color = "Black";
                else if(board.grid[mouse_Y][mouse_X].color == sf::Color::Yellow) color = "Yellow";


                std::cout << "Pos X: " << mouse_X << " Pos Y " << mouse_Y << std::endl;
                std::cout << "Element Type: " << element_type << std::endl << "Color: " << color << std::endl << "Updated: " << board.grid[mouse_Y][mouse_X].updated << std::endl;
            }
            else if(key->scancode == sf::Keyboard::Scancode::Num0)
            {
                tool = 0;
            }
            else if(key->scancode == sf::Keyboard::Scancode::Num1)
            {
                tool = 1;
            }
            else if(key->scancode == sf::Keyboard::Scancode::Num2)
            {
                tool = 2;
            }
            else if(key->scancode == sf::Keyboard::Scancode::Delete)
            {
                board.clearBoard();
            }              
        }
        else if(auto* key = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if(key->button == sf::Mouse::Button::Left)
            {
                if(!mouse_left_hold) mouse_left_hold = true;
            }
        }
        else if(auto* key = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if(key->button == sf::Mouse::Button::Left)
            {
                if(mouse_left_hold) mouse_left_hold = false;
            }
        }
        else if(auto* mouse = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if(mouse->delta > 0 && brush_size < 21) brush_size += 2;
            else if(mouse->delta < 0 && brush_size > 3) brush_size -= 2;

            std::cout << "Brush Size: " << brush_size << std::endl;
        }
    }
}

void Game::render()
{
    this->window->clear();

    boardDraw();

    this->window->display();
}

void Game::boardDraw()
{
    for(int i = 0; i < board.grid.size(); i++)
    {
        for(int j = 0; j < board.grid[i].size(); j++)
        {
            image.setPixel(sf::Vector2u(j, i), board.grid[i][j].color);
        }
    }

    texture.update(image);

    this->window->draw(*sprite);

}



Game::Game(int width, int height)
{
    this->initVariables(width, height);
    this->initWindow(width, height);

    board.initBoard(width / scale, height / scale);
}

Game::~Game()
{
    delete this->window;
    delete this->sprite;
}