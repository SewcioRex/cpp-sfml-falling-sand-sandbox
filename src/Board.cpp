#include "Board.hpp"


void Board::initBoard(int width, int height)
{
    grid = std::vector<std::vector<GridCell>>(height, std::vector<GridCell>(width, {ElementType::EMPTY, ElementState::NONE, sf::Color::Black, 0, {0, 0}, false}));

    grid[0][100] = {ElementType::SAND, ElementState::POWDER, sf::Color::Yellow, 10, {0, 1}, false};

    max_grid_H = grid.size();
    max_grid_W = grid[0].size();

    gen = std::mt19937(rd());
    dist_1_2 = std::uniform_int_distribution<>(1, 2);
    dist_1_40 = std::uniform_int_distribution<>(1, 100);

    side = 1;
}


void Board::gridUpdate(float dt)
{
    for(auto& row : grid)
    {
        for(auto& cell : row)
        {
            cell.has_been_updated = false;
        }
    }


    if(side == 1)
    {
        for (int i = max_grid_H - 1; i >= 0; i--)
        {
            for (int j = max_grid_W - 1; j >= 0; j--)
            {
                if (grid[i][j].state == ElementState::POWDER || grid[i][j].state == ElementState::LIQUID)
                {
                    gravity(i, j, dt);
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
                if (grid[i][j].state == ElementState::POWDER || grid[i][j].state == ElementState::LIQUID)
                {
                    gravity(i, j, dt);
                }
            }
        }

        side = 1;
    }
}

void Board::gravity(int y, int x, float dt)
{
    //if updated stop function
    if(grid[y][x].has_been_updated) return;

    grid[y][x].velocity.y += GRAVITY_FORCE * dt;
    if(grid[y][x].velocity.y > MAX_FALL_SPEED) grid[y][x].velocity.y = MAX_FALL_SPEED;

    int vy = y, vx = x;
    
    int steps = static_cast<int>(grid[y][x].velocity.y);
    int steps_x = 0;

    for(int s = 0; s < steps; s++)
    {
        if(vy + 1 < max_grid_H && grid[vy + 1][vx].density < grid[vy][vx].density && !grid[vy + 1][vx].has_been_updated)
        {
            std::swap(grid[vy + 1][vx], grid[vy][vx]);
            vy++;
        }
        else
        {

            grid[vy][vx].velocity.x = grid[vy][vx].velocity.y * 0.4f;
            if(grid[vy][vx].velocity.x > 1)
            {
                steps_x = static_cast<int>(grid[vy][vx].velocity.x);
                grid[y][x].velocity.y = 1.f;
                break;
            }

            bool can_left = (vy + 1 < max_grid_H && vx - 1 >= 0 && grid[vy + 1][vx - 1].density < grid[vy][vx].density && !grid[vy + 1][vx - 1].has_been_updated);

            bool can_right = (vy + 1 < max_grid_H && vx + 1 < max_grid_W && grid[vy + 1][vx + 1].density < grid[vy][vx].density && !grid[vy + 1][vx + 1].has_been_updated);

            if (can_left || can_right)
            {
                int dir;
                if (can_left && can_right) dir = (rand()%2==0 ? -1 : 1);
                else dir = can_left ? -1 : 1;

                std::swap(grid[vy+1][vx+dir], grid[vy][vx]);
                vy++;
                vx += dir;
            }
            else
            {
                grid[y][x].velocity.y = 1.f;
                break;
            }
        }
    }

    if(steps_x > 0)
    {
        bool can_left = (vx - 1 >= 0 && grid[vy][vx - 1].density < grid[vy][vx].density && !grid[vy][vx - 1].has_been_updated);
        bool can_right = (vx + 1 < max_grid_W && grid[vy][vx + 1].density < grid[vy][vx].density && !grid[vy][vx + 1].has_been_updated);

        int side;
        if(can_left && can_right) side = dist_1_2(gen);
        else if (can_left && !can_right) side = 1;
        else if(!can_left && can_right) side = 2;
        else side = 0;

        if(side > 0)
        {
            for (int i = 0; i < steps_x; i++)
            {
                if (side == 1)
                {
                    if(vx - 1 >= 0 && grid[vy][vx - 1].density < grid[vy][vx].density && !grid[vy][vx - 1].has_been_updated)
                    {
                        std::swap(grid[vy][vx - 1], grid[vy][vx]);
                        vx--;
                    }
                    else break;
                }
                else
                {
                    if(vx + 1 < max_grid_W && grid[vy][vx + 1].density < grid[vy][vx].density && !grid[vy][vx + 1].has_been_updated)
                    {
                        std::swap(grid[vy][vx + 1], grid[vy][vx]);
                        vx++;
                    }
                    else break;
                }
            }
        }

        grid[vy][vx].velocity.x = 0.f;
    }



    if(y != vy && x != vx)
    {
        grid[vy][vx].has_been_updated = true;
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

                if(tool == 0) grid[y + i][x + j] = {ElementType::EMPTY, ElementState::NONE, sf::Color::Black, 0, {0, 0}, false};

                if(random == 1)
                {
                    if(tool == 1) grid[y + i][x + j] = {ElementType::SAND, ElementState::POWDER, sf::Color::Yellow, 10, {0, 1}, false};
                    //Blue element only for test
                    else if(tool == 2) grid[y + i][x + j] = {ElementType::SAND, ElementState::POWDER, sf::Color::Blue, 5, {0, 1}, false};
                }
                    

            }
        }
    }
}

void Board::clearBoard()
{
    grid = std::vector<std::vector<GridCell>>(max_grid_H, std::vector<GridCell>(max_grid_W, {ElementType::EMPTY, ElementState::NONE, sf::Color::Black, 0, {0, 0}, false}));
}
