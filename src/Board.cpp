#include "Board.hpp"


void Board::initBoard(int width, int height)
{
    //width = width / 10;
    //height = height / 10;
    grid = std::vector<std::vector<GridCell>>(height / 10, std::vector<GridCell>(width / 10, {ElementType::EMPTY, sf::Color::Black}));

    /*for(int i = 50; i < 51; i++)
    {
        grid[50][i].elementType = ElementType::SAND;
    }*/
}

Board::Board()
{
}

Board::~Board()
{
}
