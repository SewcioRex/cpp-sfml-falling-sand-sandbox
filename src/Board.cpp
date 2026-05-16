#include "Board.hpp"


Board::Board(int width, int height)
{
    max_grid_H = height;
    max_grid_W = width;

    clear_board();

    //Randoms
    gen = std::mt19937(rd());
    dist_bool = std::uniform_int_distribution<>(0, 1);
    dist_1_100 = std::uniform_int_distribution<>(1, 100);
    dist_0_2 = std::uniform_int_distribution<>(0, 2);

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
                case ElementType::WATER: update_water(y, x); break;
                case ElementType::STEAM: update_steam(y, x); break;
                case ElementType::FIRE: update_fire(y, x); break;
                case ElementType::WOOD: update_wood(y, x); break;
                case ElementType::ASH: update_ash(y, x); break;
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

    cell.has_been_updated = true;

    cell.age++;

    int ny = y, nx = x;

    bool moved = powder_gravity(y, x, ny, nx);

    if(moved) std::swap(grid[y][x], grid[ny][nx]);
}

void Board::update_water(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    cell.has_been_updated = true;

    cell.age++;

    int ny = y, nx = x;

    bool moved = liquid_gravity(y, x, ny, nx);

    if(moved) std::swap(grid[y][x], grid[ny][nx]);
}

void Board::update_steam(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    grid[y][x].has_been_updated = true;

    cell.age++;

    int ny = y, nx = x;

    bool moved = gas_gravity(y, x, ny, nx);

    if(moved) std::swap(grid[y][x], grid[ny][nx]);
}

void Board::update_fire(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    cell.has_been_updated = true;

    cell.age++;

    cell.color = {255, static_cast<uint8_t>(std::min(cell.age * 15, 255)), 0, 255};

    if(cell.age > 20)
    {
        cell = {EMPTY, sf::Color::Black, 0, false, 0, 0, true};
        return;
    }

    int ran_y = y + dist_0_2(gen) - 1;
    int ran_x = x + dist_0_2(gen) - 1; // ran val 0-2

    if (in_bounds(ran_y, ran_x))
    {
        auto& ran_cell = grid[ran_y][ran_x];

        if(ran_cell.is_flammable) ran_cell.is_burning = true;
    }

    ran_y = y - dist_bool(gen);
    ran_x = x + dist_0_2(gen) - 1; // ran val 0-2

    if (in_bounds(ran_y, ran_x))
    {
        auto& ran_cell = grid[ran_y][ran_x];

        if (ran_cell.elementType == EMPTY) std::swap(cell, ran_cell);
    }
}

void Board::update_wood(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    cell.has_been_updated = true;

    if(cell.is_burning)
    {

        cell.fuel--;

        int ran_y = y + dist_0_2(gen) - 1;
        int ran_x = x + dist_0_2(gen) - 1;

        if (in_bounds(ran_y, ran_x))
        {
            if (grid[ran_y][ran_x].elementType == EMPTY) grid[ran_y][ran_x] = fire_cell();
        }
    }

    if(cell.fuel <= 0)
    {
        int random = dist_1_100(gen);

        if(random >= 5) cell = empty_cell();
        else cell = ash_cell();
    }
}

void Board::update_ash(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    cell.has_been_updated = true;

    int random = dist_1_100(gen);

    if(random > 80)
    {
        int ran_x = x + dist_0_2(gen) - 1;

        if(can_swap(y, x, y + 1, ran_x)) std::swap(cell, grid[y + 1][ran_x]);
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

bool Board::liquid_gravity(int y, int x, int& ny, int& nx)
{
    bool can_down = can_swap(y, x, y + 1, x);
    if (can_down)
    {
        ny = y + 1;
        return true;
    }

    bool can_left_down = can_swap(y, x, y + 1, x - 1);
    bool can_right_down = can_swap(y, x, y + 1, x + 1);

    if (can_left_down || can_right_down)
    {
        int dir = 0;

        if (can_left_down && !can_right_down)
            dir = -1;
        else if (!can_left_down && can_right_down)
            dir = 1;
        else
            dir = dist_bool(gen) ? -1 : 1;

        ny = y + 1;
        nx = x + dir;

        return true;
    }

    bool can_left = can_swap(y, x, y, x - 1);
    bool can_right = can_swap(y, x, y, x + 1);

    if (can_left || can_right)
    {
        int dir = 0;
        if (can_left && !can_right)
            dir = -1;
        else if (!can_left && can_right)
            dir = 1;
        else
            dir = dist_bool(gen) ? -1 : 1;

        ny = y;
        nx = x + dir;

        return true;
    }

    return false;
}

bool Board::gas_gravity(int y, int x, int& ny, int& nx)
{
    bool can_up = can_swap(y, x, y - 1, nx);

    if(can_up)
    {
        ny = y - 1;
        return true;
    }

    bool can_left_up = can_swap(y, x, y - 1, x - 1);
    bool can_right_up = can_swap(y, x, y - 1, x + 1);

    if (can_left_up || can_right_up)
    {
        int dir = 0;

        if (can_left_up && !can_right_up)
            dir = -1;
        else if (!can_left_up && can_right_up)
            dir = 1;
        else
            dir = dist_bool(gen) ? -1 : 1;

        ny = y - 1;
        nx = x + dir;

        return true;
    }

    bool can_left = can_swap(y, x, y, x - 1);
    bool can_right = can_swap(y, x, y, x + 1);

    if (can_left || can_right)
    {
        int dir = 0;
        if (can_left && !can_right)
            dir = -1;
        else if (!can_left && can_right)
            dir = 1;
        else
            dir = dist_bool(gen) ? -1 : 1;

        ny = y;
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
            if (y + i < 0 || y + i >= max_grid_H)
                continue;
            if (x + j < 0 || x + j >= max_grid_W)
                continue;

            auto &cell = grid[y + i][x + j];

            if (tool == ElementType::EMPTY)
            {
                cell = empty_cell();
                continue;
            }

            if (tool == ElementType::STONE)
            {
                cell = stone_cell();
                continue;
            }
            if (tool == ElementType::WOOD)
            {
                cell = wood_cell();
                continue;
            }

            if (cell.elementType == EMPTY && dist_1_100(gen) == 1)
            {
                if (tool == ElementType::SAND)
                    cell = sand_cell();

                else if (tool == ElementType::WATER)
                    cell = water_cell();

                else if (tool == ElementType::STEAM)
                    cell = steam_cell();

                else if (tool == ElementType::FIRE)
                    cell = fire_cell();
            }
        }
    }
}

void Board::clear_board()
{
    grid = std::vector<std::vector<GridCell>>(max_grid_H, std::vector<GridCell>(max_grid_W, empty_cell()));
}

Board::GridCell Board::empty_cell()
{
    return{
        ElementType::EMPTY,     //Element Type
        sf::Color::Black,       //Color
        0,                      //Density
        false,                  //Is Flammable
        false,                  //Is Burning
        0,                      //Fuel
        false                   //Has Been Updated
    };
}

Board::GridCell Board::sand_cell()
{
    std::array<sf::Color, 3> sand_colors{
        sf::Color::Yellow,
        sf::Color(200, 180, 0, 255),
        sf::Color(255, 255, 120, 255)
    };

    int random = dist_0_2(gen);

    return{
        ElementType::SAND,      //Element Type
        sand_colors[random],      //Color
        10,                     //Density
        false,                  //Is Flammable
        false,                  //Is Burning
        0,                      //Fuel
        false                   //Has Been Updated
    };
}

Board::GridCell Board::water_cell()
{

    return{
        ElementType::WATER,     //Element Type
        {30, 144, 255, 255},        //Color
        5,                      //Density
        false,                  //Is Flammable
        false,                  //Is Burning
        0,                      //Fuel
        false                   //Has Been Updated
    };
}

Board::GridCell Board::stone_cell()
{
    std::array<sf::Color, 3> stone_colors{
        sf::Color(105, 105, 105, 255),
        sf::Color(128, 128, 128, 255),
        sf::Color(169, 169, 169, 255)
    };

    //{93, 93, 93, 255}, Old color
    return{
        ElementType::STONE,     //Element Type
        stone_colors[dist_0_2(gen)],      //Color
        10,                     //Density
        false,                  //Is Flammable
        false,                  //Is Burning
        0,                      //Fuel
        false                   //Has Been Updated
    };
}

Board::GridCell Board::steam_cell()
{
    return{
        ElementType::STEAM,     //Element Type
        {255, 255, 255, 200},       //Color
        1,                      //Density
        false,                  //Is Flammable
        false,                  //Is Burning
        0,                      //Fuel
        false                   //Has Been Updated
    };
}

Board::GridCell Board::fire_cell()
{
    return{
        ElementType::FIRE,      //Element Type
        sf::Color::Red,         //Color
        100,                    //Density
        false,                  //Is Flammable
        false,                  //Is Burning
        0,                      //Fuel
        false                   //Has Been Updated
    };
}

Board::GridCell Board::wood_cell()
{
    return{
        ElementType::WOOD,      //Element Type
        {150, 75, 0, 255},      //Color
        10,                     //Density
        true,                   //Is Flammable
        false,                  //Is Burning
        100,                    //Fuel
        false                   //Has Been Updated
    };
}

Board::GridCell Board::ash_cell()
{
    return{
        ElementType::ASH,       //Element Type
        {211, 211, 211, 255},   //Color
        10,                      //Density
        false,                  //Is Flammable
        false,                  //Is Burning
        0,                      //Fuel
        false                   //Has Been Updated
    };
}
