#include "Board.hpp"


Board::Board(int width, int height)
{
    //Board init
    grid = std::vector<std::vector<GridCell>>(height, std::vector<GridCell>(width, {ElementType::EMPTY, sf::Color::Black, 0, {0, 0}, false}));

    //Grid max size
    max_grid_H = grid.size();
    max_grid_W = grid[0].size();

    //Randoms
    gen = std::mt19937(rd());
    dist_1_2 = std::uniform_int_distribution<>(1, 2);
    dist_1_40 = std::uniform_int_distribution<>(1, 100);

    side_to_update = 1;
}


void Board::gridUpdate(float dt)
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

    for (int y = max_grid_H - 1; y >= 0; y--)
    {
        for (int x = startX; x != endX; x += stepX)
        {
            switch (grid[y][x].elementType)
            {
                case ElementType::SAND: updateSand(y, x, dt); break;
                default: break;
            }
        }
    }

    side_to_update = (side_to_update == 1) ? 2 : 1;
}

void Board::gravity(int y, int x, float dt)
{
    //if updated stop function
    if(grid[y][x].has_been_updated) return;

    //Gravity force calculations
    grid[y][x].velocity.y += GRAVITY_FORCE * dt;
    if(grid[y][x].velocity.y > MAX_FALL_SPEED) grid[y][x].velocity.y = MAX_FALL_SPEED;

    //velocity_y, velocity_x
    int vy = y, vx = x;
    
    //Position to fall (Vertical // Gravity)
    int steps = static_cast<int>(grid[y][x].velocity.y);
    int steps_x = 0;

    //Fall loop
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

            bool can_left_down = (vy + 1 < max_grid_H && vx - 1 >= 0 && grid[vy + 1][vx - 1].density < grid[vy][vx].density && !grid[vy + 1][vx - 1].has_been_updated);
            bool can_right_down = (vy + 1 < max_grid_H && vx + 1 < max_grid_W && grid[vy + 1][vx + 1].density < grid[vy][vx].density && !grid[vy + 1][vx + 1].has_been_updated);

            if (can_left_down || can_right_down)
            {
                int dir;
                if (can_left_down && can_right_down) dir = (rand()%2==0 ? -1 : 1);
                else dir = can_left_down ? -1 : 1;

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

    //Change the remaining part of gravity force to side force
    if(steps_x > 0)
    {
        bool can_left = (vx - 1 >= 0 && grid[vy][vx - 1].density < grid[vy][vx].density && !grid[vy][vx - 1].has_been_updated);
        bool can_right = (vx + 1 < max_grid_W && grid[vy][vx + 1].density < grid[vy][vx].density && !grid[vy][vx + 1].has_been_updated);

        int dir;
        if(can_left || can_right)
        {
            if (can_left && can_right) dir = (rand()%2==0 ? -1 : 1);
            else dir = can_left ? -1 : 1;

            for (int i = 0; i < steps_x; i++)
            {
                if(vx - 1 >= 0 && vx + 1 < max_grid_W && grid[vy][vx + dir].density < grid[vy][vx].density && !grid[vy][vx + dir].has_been_updated)
                {
                    std::swap(grid[vy][vx + dir], grid[vy][vx]);
                    vx += dir;
                }
                else break;
            }
            
        }

        grid[vy][vx].velocity.x = 0.f;
    }

    if(y != vy && x != vx)
    {
        grid[vy][vx].has_been_updated = true;
    }
}

void Board::updateSand(int y, int x, float dt)
{
    gravity(y, x, dt);
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

                if(tool == 0) grid[y + i][x + j] = {ElementType::EMPTY, sf::Color::Black, 0, {0, 0}, false};

                if(random == 1)
                {
                    if(tool == 1) grid[y + i][x + j] = {ElementType::SAND, sf::Color::Yellow, 10, {0, 1}, false};
                    //else if(tool == 2) grid[y + i][x + j] = {ElementType::WATER, sf::Color::Blue, 5, {0, 1}, false};
                }
            }
        }
    }
}

void Board::clearBoard()
{
    grid = std::vector<std::vector<GridCell>>(max_grid_H, std::vector<GridCell>(max_grid_W, {ElementType::EMPTY, sf::Color::Black, 0, {0, 0}, false}));
}
