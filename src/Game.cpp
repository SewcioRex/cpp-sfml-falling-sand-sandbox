#include "Game.hpp"

void Game::initVariables(int width, int height)
{
    this->window = nullptr;
    this->sprite = nullptr;

    image.resize(sf::Vector2u(width / 10, height / 10));

    timer = 0.f;
    brushSize = 1;

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

    dt = clock.restart().asSeconds();
    timer += dt;

    if(timer >= 0.05f)
    {
        board.gridUpdate();
        timer = 0.f;
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
            if(key->scancode == sf::Keyboard::Scancode::T)
            {
                int mouse_X = (sf::Mouse::getPosition(*this->window).x) / 10;
                int mouse_Y = (sf::Mouse::getPosition(*this->window).y) / 10;

                std::string element_type;
                if(board.grid[mouse_Y][mouse_X].elementType == Board::ElementType::EMPTY) element_type = "Empty";
                else if(board.grid[mouse_Y][mouse_X].elementType == Board::ElementType::SAND) element_type = "Sand";

                std::string color;
                if(board.grid[mouse_Y][mouse_X].color == sf::Color::Black) color = "Black";
                else if(board.grid[mouse_Y][mouse_X].color == sf::Color::Yellow) color = "Yellow";

                std::cout << "Pos X: " << mouse_X << " Pos Y " << mouse_Y << std::endl;
                std::cout << "Element Type: " << element_type << std::endl << " Color: " << color << std::endl;
            }
        }
        else if(auto* key = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if(key->button == sf::Mouse::Button::Left)
            {
                int mouse_X = (sf::Mouse::getPosition(*this->window).x) / 10;
                int mouse_Y = (sf::Mouse::getPosition(*this->window).y) / 10;

                for(int i = -brushSize / 2; i <= brushSize / 2; i++)
                {
                    for(int j = -brushSize / 2; j <= brushSize / 2; j++)
                    {
                        if((mouse_Y + i >= 0 && mouse_Y + i < board.grid.size()) && (mouse_X + j >= 0 && mouse_X + j < board.grid[mouse_Y].size()))
                        {
                            board.grid[mouse_Y + i][mouse_X + j] = {Board::ElementType::SAND, Board::ElementState::POWDER, sf::Color::Yellow};
                        }
                    }
                }
            }
        }
        else if(auto* mouse = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if(mouse->delta > 0 && brushSize < 5) brushSize += 2;
            else if(mouse->delta < 0 && brushSize > 1) brushSize -= 2;

            std::cout << "Brush Size: " << brushSize << std::endl;
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

    board.initBoard(width, height);
}

Game::~Game()
{
    delete this->window;
    delete this->sprite;
}