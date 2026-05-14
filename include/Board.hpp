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
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist_bool;
    std::uniform_int_distribution<> dist_viscosity;
    std::uniform_int_distribution<> dist_1_100;

    int side_to_update;

    int max_grid_H;
    int max_grid_W;

    //Functions

    bool powder_gravity(int y, int x, int& ny, int& nx);
    bool liquid_gravity(int y, int x, int& ny, int& nx);
    bool gas_gravity(int y, int x, int& ny, int& nx);

    bool can_swap(int y, int x, int new_y, int new_x);
        bool in_bounds(int new_y, int new_x);

    void update_sand(int y, int x);
    void update_water(int y, int x);
    void update_steam(int y, int x);


public:
    //Variables
    enum ElementType
    {
        EMPTY = 0,
        SAND,
        WATER,
        STONE,
        STEAM,
    };

    struct GridCell
    {
        ElementType elementType;
        sf::Color color;
        int density;
        bool has_been_updated;
    };

    std::vector<std::vector<GridCell>> grid;
    std::vector<std::vector<GridCell>> update_grid;

    //Functions
    void grid_update();
    void brush_tool(int y, int x, int brush_size, ElementType elementType);
    void clear_board();

    Board(int width = 10, int height = 10);
};
