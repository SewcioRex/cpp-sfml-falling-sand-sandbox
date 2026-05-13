#include "Board.hpp"


Board::Board(int width, int height)
{
    //Board init
    grid = std::vector<std::vector<GridCell>>(height, std::vector<GridCell>(width, {ElementType::EMPTY, sf::Color::Black, 0, 0, false}));

    //Grid max size
    max_grid_H = grid.size();
    max_grid_W = grid[0].size();

    //Randoms
    gen = std::mt19937(rd());
    dist_bool = std::uniform_int_distribution<>(0, 1);
    dist_1_40 = std::uniform_int_distribution<>(1, 100);

    side_to_update = 1;
}


void Board::grid_update()
{
    //Change all cells into not updated state 
    for(auto& row : grid)
    {
        for(auto& cell : row)
        {
            cell.has_been_updated = false;
        }
    }

    int startX = (side_to_update == 1) ? max_grid_W - 1 : 0;
    int endX = (side_to_update == 1) ? -1 : max_grid_W;
    int stepX = (side_to_update == 1) ? -1 : 1;

    for(int y = max_grid_H - 1; y >= 0; y--)
    {
        for(int x = startX; x != endX; x += stepX)
        {
            switch(grid[y][x].elementType)
            {
                case ElementType::SAND: update_sand(y, x); break;
                //case ElementType::WATER: update_water(y, x); break;
                default: break;
            }
        }
    }

    side_to_update = (side_to_update == 1) ? 2 : 1;
}

void Board::update_sand(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    int ny = y, nx = x;

    bool moved = powder_gravity(y, x, ny, nx);

    if(moved)
    {
        std::swap(grid[y][x], grid[ny][nx]);
        grid[ny][nx].has_been_updated = true;
    }
    else
    {
        grid[y][x].has_been_updated = true;
    }
}

bool Board::powder_gravity(int y, int x, int& ny, int& nx)
{
    bool can_down = can_swap(y, x, y + 1, x);
    if(can_down)
    {
        ny = y + 1;
        return true;
    }

    bool can_left_down = can_swap(y, x, y + 1, x - 1);
    bool can_right_down = can_swap(y, x, y + 1, x + 1);

    if(can_left_down || can_right_down)
    {
        int dir = 0;
        if(can_left_down && can_right_down)
        {
            dir = dist_bool(gen);
        }
        else dir = can_left_down ? -1 : 1;

        ny = y + 1;
        nx = x + dir;

        return true;
    }

    return false;
}

bool Board::can_swap(int y, int x, int new_y, int new_x)
{
    if(!in_bounds(new_y, new_x)) return false;

    auto& cell = grid[y][x];
    auto& new_cell = grid[new_y][new_x];

    return new_cell.density < cell.density;
}

bool Board::in_bounds(int new_y, int new_x)
{
    return  new_y >= 0 &&
            new_y < max_grid_H &&
            new_x >= 0 &&
            new_x < max_grid_W;
}


void Board::brush_tool(int y, int x, int brush_size, ElementType tool)
{
    for (int i = -brush_size / 2; i <= brush_size / 2; i++)
    {
        for (int j = -brush_size / 2; j <= brush_size / 2; j++)
        {
            if (y + i < 0 || y + i >= grid.size())
                continue;
            if (x + j < 0 || x + j >= grid[y].size())
                continue;

            auto &cell = grid[y + i][x + j];

            if (tool == ElementType::EMPTY)
            {
                cell = {EMPTY, sf::Color::Black, 0, 0, false};
                continue;
            }

            if (tool == ElementType::STONE)
            {
                cell = {STONE, {93, 93, 93, 255}, 10, 0, false};
                continue;
            }

            if (cell.elementType == EMPTY && dist_1_40(gen) == 1)
            {
                if (tool == ElementType::SAND)
                    cell = {SAND, sf::Color::Yellow, 10, 1,false};

                else if (tool == ElementType::WATER)
                    cell = {WATER, sf::Color::Blue, 5, 1,false};
            }
        }
    }
}

void Board::clear_board()
{
    grid = std::vector<std::vector<GridCell>>(max_grid_H, std::vector<GridCell>(max_grid_W, {ElementType::EMPTY, sf::Color::Black, 0, 0, false}));
}
