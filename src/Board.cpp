#include "Board.hpp"


void Board::initBoard(int width, int height)
{
    //width = width / 10;
    //height = height / 10;
    grid = std::vector<std::vector<GridCell>>(height / 10, std::vector<GridCell>(width / 10, {ElementType::EMPTY, ElementState::NONE, sf::Color::Black}));

    //Debug only
    for(int i = 0; i <= 5; i++)
    {
        for(int j = grid[i].size() - 1; j >= grid[i].size() - 6; j--)
        {
            grid[i][j].elementType = ElementType::SAND;
            grid[i][j].state = ElementState::POWDER;
            grid[i][j].color = sf::Color::Yellow;
        }
    }
}


void Board::gridUpdate()
{
    for(int i = grid.size() - 1; i >= 0; i--)
    {
        for(int j = grid[i].size() - 1; j >= 0; j--)
        {
            if(i + 1 < grid.size() && grid[i][j].state == ElementState::POWDER)
            {
                powderUpdate(i, j);
            }

        }
    }
}
void Board::powderUpdate(int y, int x)
{
    if(grid[y + 1][x].elementType == ElementType::EMPTY)
    {
        std::swap(grid[y][x], grid[y + 1][x]);
    }
    else if(x - 1 >= 0 && grid[y + 1][x - 1].elementType == ElementType::EMPTY)
    {
        std::swap(grid[y][x], grid[y + 1][x - 1]);
    }
    else if(x + 1 < grid[y].size() && grid[y + 1][x + 1].elementType == ElementType::EMPTY)
    {
        std::swap(grid[y][x], grid[y + 1][x + 1]);
    }
}

Board::Board()
{
}

Board::~Board()
{
}
