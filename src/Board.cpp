#include "Board.hpp"


void Board::initBoard(int width, int height)
{
    grid = std::vector<std::vector<GridCell>>(height, std::vector<GridCell>(width, {ElementType::EMPTY, ElementState::NONE, sf::Color::Black, 0, false}));

    max_grid_H = grid.size();
    max_grid_W = grid[0].size();

    gen = std::mt19937(rd());
    dist_1_2 = std::uniform_int_distribution<>(1, 2);
    dist_1_40 = std::uniform_int_distribution<>(1, 40);

    side = 1;
}


void Board::gridUpdate()
{
    for(auto& row : grid)
    {
        for(auto& cell : row)
        {
            cell.updated = false;
        }
    }


    if(side == 1)
    {
        for (int i = max_grid_H - 1; i >= 0; i--)
        {
            for (int j = max_grid_W - 1; j >= 0; j--)
            {
                if (grid[i][j].state == ElementState::POWDER)
                {
                    powderUpdate(i, j);
                }
            }
        }

        side = 2;
    }
    else if(side == 2)
    {
        for (int i = max_grid_H - 1; i >= 0; i--)
        {
            for (int j = 0; j < max_grid_W; j++)
            {
                if (grid[i][j].state == ElementState::POWDER)
                {
                    powderUpdate(i, j);
                }
            }
        }

        side = 1;
    }
}
void Board::powderUpdate(int y, int x)
{
    if(grid[y][x].updated) return;

    if(y + 1 < max_grid_H && grid[y + 1][x].density < grid[y][x].density && !grid[y + 1][x].updated)
    {
        std::swap(grid[y + 1][x], grid[y][x]);
        grid[y + 1][x].updated = true;
    }
    else
    {
        bool can_left = (y + 1 < max_grid_H && x - 1 >= 0 && grid[y + 1][x - 1].density < grid[y][x].density && !grid[y + 1][x - 1].updated);

        bool can_right = (y + 1 < max_grid_H && x + 1 < max_grid_W && grid[y + 1][x + 1].density < grid[y][x].density && !grid[y + 1][x + 1].updated);

        if(can_left && can_right) random_side = dist_1_2(gen);
        else if(can_left && !can_right) random_side = 1;
        else if(!can_left && can_right) random_side = 2;
        else random_side = 0;

        if(random_side == 1)
        {
            std::swap(grid[y + 1][x - 1], grid[y][x]);
            grid[y + 1][x - 1].updated = true;
        }
        else if(random_side == 2)
        {
            std::swap(grid[y + 1][x + 1], grid[y][x]);
            grid[y + 1][x + 1].updated = true;
        }
        else return;
    }
}

void Board::brushTool(int y, int x, int brush_size, int tool)
{
    for (int i = -brush_size / 2; i <= brush_size / 2; i++)
    {
        for (int j = -brush_size / 2; j <= brush_size / 2; j++)
        {
            if ((y + i >= 0 && y + i < grid.size()) && (x + j >= 0 && x + j < grid[y].size()))
            {
                int random = dist_1_40(gen);

                if(random == 1)
                {
                    if(tool == 0) grid[y + i][x + j] = {ElementType::EMPTY, ElementState::NONE, sf::Color::Black, 0, false};
                    else if(tool == 1) grid[y + i][x + j] = {ElementType::SAND, ElementState::POWDER, sf::Color::Yellow, 10, false};
                }

            }
        }
    }
}

void Board::clearBoard()
{
    grid = std::vector<std::vector<GridCell>>(max_grid_H, std::vector<GridCell>(max_grid_W, {ElementType::EMPTY, ElementState::NONE, sf::Color::Black, 0, false}));
}
