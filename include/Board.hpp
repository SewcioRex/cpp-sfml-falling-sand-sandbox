#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <random>
#include <iostream>

class Board
{
private:

    //Variables
        //Random
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist_bool;
    std::uniform_int_distribution<> dist_viscosity;
    std::uniform_int_distribution<> dist_1_100;
    std::uniform_int_distribution<> dist_0_2;

    int side_to_update;

    //Functions

    bool powder_gravity(int y, int x, int& ny, int& nx);
    bool liquid_gravity(int y, int x, int& ny, int& nx);
    bool gas_gravity(int y, int x, int& ny, int& nx);

    bool can_swap(int y, int x, int new_y, int new_x);
        bool in_bounds(int new_y, int new_x);

    void update_sand(int y, int x);
    void update_water(int y, int x);
    void update_steam(int y, int x);
    void update_fire(int y, int x);
    void update_wood(int y, int x);
    void update_ash(int y, int x);


public:
    //Variables
    enum ElementType
    {
        EMPTY = 0,
        SAND,
        ASH,
        STONE,
        WOOD,
        WATER,
        STEAM,
        FIRE,
    };

    struct GridCell
    {
        ElementType elementType;
        sf::Color color;
        int density;
        bool is_flammable;
        bool is_burning;
        int fuel;
        int age;
        bool has_been_updated;
    };

    int max_grid_H;
    int max_grid_W;

    bool is_paused;
    bool can_render_next_frame;

    std::vector<std::vector<GridCell>> grid;

    //Functions
    void grid_update();
    void brush_tool(int y, int x, int brush_size, ElementType elementType);
    void clear_board();

    //EMPTY
    GridCell empty_cell();

    //POWDER
    GridCell sand_cell();
    GridCell ash_cell();

    //SOLID
    GridCell stone_cell();
    GridCell wood_cell();

    //LIQUID
    GridCell water_cell();

    //GAS
    GridCell steam_cell();

    //OTHER
    GridCell fire_cell();

    Board(int width = 10, int height = 10);
};
