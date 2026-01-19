#include "Game.hpp"

void Game::initVariables()
{
    this->window = nullptr;
}

void Game::initWindow()
{
    this->window = new sf::RenderWindow(sf::VideoMode({800, 600}), "Sandbox", sf::Style::Titlebar | sf::Style::Close);
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
    }
}

Game::Game()
{
    this->initVariables();
    this->initWindow();
}

Game::~Game()
{
    delete this->window;
}