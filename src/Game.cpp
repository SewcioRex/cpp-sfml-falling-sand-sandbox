#include "Game.hpp"

void Game::initVariables()
{
    this->window = nullptr;
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

void Game::render()
{
    this->window->clear();

    this->window->display();
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
                for(int i = 0; i < board.boardVector.size(); i++)
                {
                    for(int j = 0; j < board.boardVector[i].size(); j++)
                    {
                        std::cout << board.boardVector[i][j] << " ";
                    }

                    std::cout << std::endl;
                }
            }
        }
    }
}

Game::Game(int width, int height)
{
    this->initVariables();
    this->initWindow(width, height);

    board.initBoard(width, height);
}

Game::~Game()
{
    delete this->window;
}