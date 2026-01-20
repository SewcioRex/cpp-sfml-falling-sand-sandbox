#include "Board.hpp"


void Board::initBoard(int width, int height)
{
    width = width / 10;
    height = height / 10;
    grid = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
}

Board::Board()
{
}

Board::~Board()
{
}
