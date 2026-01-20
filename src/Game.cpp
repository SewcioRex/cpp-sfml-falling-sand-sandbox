#include "Game.hpp"

void Game::initVariables(int width, int height)
{
    this->window = nullptr;
    this->sprite = nullptr;

    image.resize(sf::Vector2u(width / 10, height / 10));

    if(!texture.resize(sf::Vector2u(width / 10, height / 10))) std::cout << "Error: Texture error\n";
    
    this->sprite = new sf::Sprite(texture);
    this->sprite->setScale(sf::Vector2f(10.f, 10.f));

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
                for(int i = 0; i < board.grid.size(); i++)
                {
                    for(int j = 0; j < board.grid[i].size(); j++)
                    {
                        std::cout << board.grid[i][j] << " ";
                    }

                    std::cout << std::endl;
                }
            }
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
            if(board.grid[i][j] == 1)
            {
                image.setPixel(sf::Vector2u(j, i), sf::Color::Yellow);
            }
            else image.setPixel(sf::Vector2u(j, i), sf::Color::Black);
        }
    }

    texture.update(image);

    this->window->draw(*sprite);

}



Game::Game(int width, int height)
{
    this->initVariables(width, height);
    this->initWindow(width, height);

    board.initBoard(width, height);
}

Game::~Game()
{
    delete this->window;
    delete this->sprite;
}