#include "Board.hpp"


Board::Board(int width, int height)
{
    max_grid_H = height;
    max_grid_W = width;

    is_paused = false;
    can_render_next_frame_in_pause = false;

    clear_board();

    //Randoms
    gen = std::mt19937(rd());
    dist_bool = std::uniform_int_distribution<>(0, 1);
    dist_1_100 = std::uniform_int_distribution<>(1, 100);
    dist_0_2 = std::uniform_int_distribution<>(0, 2);

    side_to_update = 1;
}

void Board::clear_board()
{
    grid = std::vector<std::vector<GridCell>>(max_grid_H, std::vector<GridCell>(max_grid_W, empty_cell()));
}

void Board::grid_update()
{
    if(is_paused && !can_render_next_frame_in_pause) return;

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
                case ElementType::ASH: update_ash(y, x); break;
                case ElementType::GUNPOWDER: update_gunpowder(y, x); break;
                case ElementType::WOOD: update_wood(y, x); break;
                case ElementType::WATER: update_water(y, x); break;
                case ElementType::GASOLINE: update_gasoline(y, x); break;
                case ElementType::ACID: update_acid(y, x); break;
                case ElementType::HYDROGEN: update_hydrogen(y, x); break;
                case ElementType::SMOKE: update_smoke(y, x); break;
                case ElementType::FIRE: update_fire(y, x); break;
                default: break;
            }
        }
    }

    can_render_next_frame_in_pause = false;

    side_to_update = (side_to_update == 1) ? 2 : 1;
}

bool Board::powder_gravity(int y, int x, int& ny, int& nx)
{
    bool can_down = can_swap(y, x, y + 1, x);
    if(can_down)
    {
        if(dist_1_100(gen) > 80)
        {
            int move[] = {-1, 1};

            int dir = move[dist_bool(gen)];

            if(can_swap(y, x, y + 1, x + dir))
            {
                nx = x + dir;
            }
            else nx = x;
        }
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
        if(dist_1_100(gen) > 80)
        {
            int move[] = {-1, 1};

            int dir = move[dist_bool(gen)];

            if(can_swap(y, x, y + 1, x + dir))
            {
                nx = x + dir;
            }
            else nx = x;
        }
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

    bool samd_element_above = true;
    bool any_valid = false;

    for(int i = -1; i <= 1; i++)
    {
        if(!in_bounds(y - 1, x + i)) continue;

        any_valid = true;

        if(grid[y - 1][x + i].elementType != grid[y][x].elementType)
        {
            samd_element_above = false;
            break;
        }
    }

    if(!any_valid || samd_element_above) return false;

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
        if(dist_1_100(gen) > 50)
        {
            int move[] = {-1, 1};

            int dir = move[dist_bool(gen)];

            if(can_swap(y, x, y - 1, x + dir))
            {
                nx = x + dir;
            }
            else nx = x;
        }
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

    bool samd_element_above = true;
    bool any_valid = false;

    for(int i = -1; i <= 1; i++)
    {
        if(!in_bounds(y + 1, x + i)) continue;

        any_valid = true;

        if(grid[y + 1][x + i].elementType != grid[y][x].elementType)
        {
            samd_element_above = false;
            break;
        }
    }

    if(!any_valid || samd_element_above) return false;


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


void Board::brush_tool(int y, int x, int brush_size, int tool)
{

    for(int i = -brush_size / 2; i <= brush_size / 2; i++)
    {
        for(int j = -brush_size / 2; j <= brush_size / 2; j++)
        {
            if (y + i < 0 || y + i >= max_grid_H)
                continue;
            if (x + j < 0 || x + j >= max_grid_W)
                continue;

            auto& cell = grid[y + i][x + j];


            // Empty
            if(tool == ElementType::EMPTY)
            {
                cell = empty_cell();
                continue;
            }

            if(cell.elementType != ElementType::EMPTY) continue;

            //SOLID -> WHOLE SQUARE

            if(tool == ElementType::STONE)
            {
                cell = stone_cell();
                continue;
            }

            if(tool == ElementType::WOOD)
            {
                cell = wood_cell();
                continue;
            }

            //ELSE THEN SOLID -> RANDOM
            if(dist_1_100(gen) != 1) continue;

            if(tool == ElementType::SAND)
            {
                cell = sand_cell();
                continue;
            }

            if(tool == ElementType::ASH)
            {
                cell = ash_cell();
                continue;
            }

            if(tool == ElementType::GUNPOWDER)
            {
                cell = gunpowder_cell();
                continue;
            }

            if(tool == ElementType::WATER)
            {
                cell = water_cell();
                continue;
            }

            if(tool == ElementType::GASOLINE)
            {
                cell = gasoline_cell();
                continue;
            }

            if(tool == ElementType::ACID)
            {
                cell = acid_cell();
                continue;
            }

            if(tool == ElementType::HYDROGEN)
            {
                cell = hydrogen_cell();
                continue;
            }

            if(tool == ElementType::SMOKE)
            {
                cell = smoke_cell();
                continue;
            }

            if(tool == ElementType::FIRE)
            {
                cell = fire_cell();
                continue;
            }
        }
    }
}

//POWDER

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

void Board::update_gunpowder(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    cell.has_been_updated = true;

    cell.age++;

    if(cell.is_burning)
    {
        cell.fuel--;

        int ran_y = y + dist_0_2(gen) - 1;
        int ran_x = x + dist_0_2(gen) - 1;

        if (in_bounds(ran_y, ran_x))
        {
            if (grid[ran_y][ran_x].elementType == ElementType::EMPTY) grid[ran_y][ran_x] = fire_cell();
        }
    }

    if(cell.fuel <= 0)
    {
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i == 0 && j == 0) continue;

                if(!in_bounds(y + i, x + j)) continue;

                if(grid[y + i][x + j].is_flammable) grid[y + i][x + j].is_burning = true;
            }
        }

        cell = empty_cell();
        return;
    }

    int ny = y, nx = x;

    bool moved = powder_gravity(y, x, ny, nx);

    if(moved) std::swap(grid[y][x], grid[ny][nx]);
}

//SOLID

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

        if(in_bounds(ran_y, ran_x))
        {
            if (grid[ran_y][ran_x].elementType == ElementType::EMPTY) grid[ran_y][ran_x] = fire_cell();
        }

        if (dist_0_2(gen) == 0)
        {
            int sy = y + dist_0_2(gen) - 1;
            int sx = x + dist_0_2(gen) - 1;

            if (in_bounds(sy, sx) && grid[sy][sx].elementType == ElementType::EMPTY)
            {
                grid[sy][sx] = smoke_cell();
            }
        }
    }

    if(cell.fuel <= 0)
    {
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i == 0 && j == 0) continue;

                if(!in_bounds(y + i, x + j)) continue;

                if(grid[y + i][x + j].is_flammable) grid[y + i][x + j].is_burning = true;
            }
        }

        int random = dist_1_100(gen);

        if(random >= 5) cell = empty_cell();
        else cell = ash_cell();
    }
}

//LIQUID

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

void Board::update_gasoline(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    cell.has_been_updated = true;

    cell.age++;

    int ny = y, nx = x;

    if(cell.is_burning)
    {
        cell.fuel--;

        int ran_y = y + dist_0_2(gen) - 1;
        int ran_x = x + dist_0_2(gen) - 1;

        if (in_bounds(ran_y, ran_x))
        {
            if (grid[ran_y][ran_x].elementType == ElementType::EMPTY) grid[ran_y][ran_x] = fire_cell();
        }
    }

    if(cell.fuel <= 0)
    {
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i == 0 && j == 0) continue;

                if(!in_bounds(y + i, x + j)) continue;

                if(grid[y + i][x + j].is_flammable) grid[y + i][x + j].is_burning = true;
            }
        }

        cell = empty_cell();
        return;
    }

    bool moved = liquid_gravity(y, x, ny, nx);

    if(moved) std::swap(grid[y][x], grid[ny][nx]);
}

void Board::update_acid(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    cell.has_been_updated = true;

    cell.age++;

    int ran_y = y + dist_0_2(gen) - 1;
    int ran_x = x + dist_0_2(gen) - 1;

    if(in_bounds(ran_y,ran_x))
    {
        int random = dist_1_100(gen);

        if(grid[ran_y][ran_x].elementType == ElementType::SAND || grid[ran_y][ran_x].elementType == ElementType::ASH || grid[ran_y][ran_x].elementType == ElementType::GUNPOWDER || grid[ran_y][ran_x].elementType == ElementType::WOOD)
        {
            if(random > 10) return;

            grid[ran_y][ran_x] = empty_cell();
            cell = empty_cell();
            return;
        }
    }

    int ny = y, nx = x;

    bool moved = liquid_gravity(y, x, ny, nx);

    if(moved) std::swap(grid[y][x], grid[ny][nx]);
}

//GAS

void Board::update_hydrogen(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    grid[y][x].has_been_updated = true;

    cell.age++;

    if(cell.is_burning)
    {
        cell.fuel--;

        int ran_y = y + dist_0_2(gen) - 1;
        int ran_x = x + dist_0_2(gen) - 1;

        if (in_bounds(ran_y, ran_x))
        {
            if (grid[ran_y][ran_x].elementType == ElementType::EMPTY) grid[ran_y][ran_x] = fire_cell();
        }
    }

    if(cell.fuel <= 0)
    {
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i == 0 && j == 0) continue;

                if(!in_bounds(y + i, x + j)) continue;

                if(grid[y + i][x + j].is_flammable) grid[y + i][x + j].is_burning = true;
            }
        }

        cell = empty_cell();
        return;
    }

    int ny = y, nx = x;

    bool moved = gas_gravity(y, x, ny, nx);

    if(moved) std::swap(grid[y][x], grid[ny][nx]);
}

void Board::update_smoke(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    grid[y][x].has_been_updated = true;

    cell.age++;

    int random_max_age = std::uniform_int_distribution<>(60, 100)(gen);

    if(cell.age >= random_max_age)
    {
        cell = empty_cell();
        return;
    }

    int ny = y, nx = x;

    bool moved = gas_gravity(y, x, ny, nx);

    if(moved) std::swap(grid[y][x], grid[ny][nx]);
}

//OTHER

void Board::update_fire(int y, int x)
{
    auto& cell = grid[y][x];

    if(cell.has_been_updated) return;

    cell.has_been_updated = true;

    cell.age++;

    cell.color = {255, static_cast<uint8_t>(std::min(cell.age * 15, 255)), 0, 255};

    if(cell.age > 20)
    {
        cell = {ElementType::EMPTY, sf::Color::Black, 0, false, 0, 0, true};
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
    ran_x = x + dist_0_2(gen) - 1; // ran val -1, 0, -1

    if (in_bounds(ran_y, ran_x))
    {
        auto& ran_cell = grid[ran_y][ran_x];

        if (ran_cell.elementType == ElementType::EMPTY) std::swap(cell, ran_cell);
    }
}



Board::GridCell Board::empty_cell()
{
    GridCell cell = {};

    cell.elementType = ElementType::EMPTY;
    cell.color = sf::Color::Black;
    cell.density = 0;
    cell.age = 0;
    cell.fuel = 0;
    cell.is_flammable = false;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::sand_cell()
{
    std::array<sf::Color, 3> sand_colors{
        sf::Color::Yellow,
        sf::Color(200, 180, 0, 255),
        sf::Color(255, 255, 120, 255)
    };

    GridCell cell = {};

    cell.elementType = ElementType::SAND;
    cell.color = sand_colors[dist_0_2(gen)];
    cell.density = 10;
    cell.age = 0;
    cell.fuel = 0;
    cell.is_flammable = false;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::ash_cell()
{
    std::array<sf::Color, 3> ash_colors{
        sf::Color(211, 211, 211, 255),
        sf::Color(170, 170, 170, 255),
        sf::Color(120, 120, 120, 255)
    };

    GridCell cell = {};

    cell.elementType = ElementType::ASH;
    cell.color = ash_colors[dist_0_2(gen)],
    cell.density = 10;
    cell.age = 0;
    cell.fuel = 0;
    cell.is_flammable = false;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::gunpowder_cell()
{
    std::array<sf::Color, 3> gunpowder_colors{
        sf::Color(50, 50, 50, 255),
        sf::Color(70, 70, 70, 255),
        sf::Color(60, 55, 45, 255)
    };

    GridCell cell = {};

    cell.elementType = ElementType::GUNPOWDER;
    cell.color = gunpowder_colors[dist_0_2(gen)],
    cell.density = 10;
    cell.age = 0;
    cell.fuel = 2;
    cell.is_flammable = true;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::stone_cell()
{
    std::array<sf::Color, 3> stone_colors{
        sf::Color(105, 105, 105, 255),
        sf::Color(128, 128, 128, 255),
        sf::Color(169, 169, 169, 255)
    };

    GridCell cell = {};

    cell.elementType = ElementType::STONE;
    cell.color = stone_colors[dist_0_2(gen)];
    cell.density = 10;
    cell.age = 0;
    cell.fuel = 0;
    cell.is_flammable = false;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::wood_cell()
{
    std::array<sf::Color, 2> wood_colors{
        sf::Color(150, 75, 0, 255),
        sf::Color(90, 45, 0, 255)
    };

    int random_fuel = std::uniform_int_distribution<>(50, 150)(gen);

    GridCell cell = {};

    cell.elementType = ElementType::WOOD;
    cell.color = wood_colors[dist_bool(gen)],
    cell.density = 10;
    cell.age = 0;
    cell.fuel = random_fuel;
    cell.is_flammable = true;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::water_cell()
{
    GridCell cell = {};

    cell.elementType = ElementType::WATER;
    cell.color = {30, 144, 255, 255}; 
    cell.density = 5;
    cell.age = 0;
    cell.fuel = 0;
    cell.is_flammable = false;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::gasoline_cell()
{
    GridCell cell = {};

    cell.elementType = ElementType::GASOLINE;
    cell.color = {200, 180, 90, 255}; 
    cell.density = 4;
    cell.age = 0;
    cell.fuel = 10;
    cell.is_flammable = true;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::acid_cell()
{
    GridCell cell = {};

    cell.elementType = ElementType::ACID;
    cell.color = {144, 238, 144, 255}; 
    cell.density = 5;
    cell.age = 0;
    cell.fuel = 0;
    cell.is_flammable = false;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::hydrogen_cell()
{
    GridCell cell = {};

    cell.elementType = ElementType::HYDROGEN;
    cell.color = {200, 220, 255, 100}; 
    cell.density = 1;
    cell.age = 0;
    cell.fuel = 1;
    cell.is_flammable = true;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::smoke_cell()
{   
    GridCell cell = {};

    cell.elementType = ElementType::SMOKE;
    cell.color = {90, 90, 90, 200}; 
    cell.density = 1;
    cell.age = 0;
    cell.fuel = 0;
    cell.is_flammable = false;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}

Board::GridCell Board::fire_cell()
{
    GridCell cell = {};

    cell.elementType = ElementType::FIRE;
    cell.color = sf::Color::Red; 
    cell.density = 100;
    cell.age = 0;
    cell.fuel = 0;
    cell.is_flammable = false;
    cell.is_burning = false;
    cell.has_been_updated = false;

    return cell;
}
