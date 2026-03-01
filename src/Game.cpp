#include "Game.hpp"

Game::Game(int width, int height) : scale(2), board(width / scale, height / scale)
{
    this->initVariables(width, height);
    this->initWindow(width, height);
}

void Game::initVariables(int width, int height)
{
    this->window = nullptr;
    this->sprite = nullptr;

    brush_size = 3;
    tool = 0;
    mouse_left_hold = false;

    image.resize(sf::Vector2u(width / scale, height / scale));
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
    
    dt = clock.restart().asSeconds();
    board.gridUpdate(dt);

    //Spawn elements
    if(mouse_left_hold)
    {
        int mouse_X = (sf::Mouse::getPosition(*this->window).x) / scale;
        int mouse_Y = (sf::Mouse::getPosition(*this->window).y) / scale;

        board.brushTool(mouse_Y, mouse_X, brush_size, tool);
    }
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
            if(key->scancode == sf::Keyboard::Scancode::Num0)
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
            if(mouse->delta > 0 && brush_size < 101) brush_size += 2;
            else if(mouse->delta < 0 && brush_size > 1) brush_size -= 2;

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

Game::~Game()
{
    delete this->window;
    delete this->sprite;
}