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

    const float GRAVITY_FORCE = 10.f;
    const float MAX_FALL_SPEED = 10.f;

    //Variables
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist_1_2;
    std::uniform_int_distribution<> dist_1_40;

    int side_to_update;

    int max_grid_H;
    int max_grid_W;

    //Functions
    void updateSand(int y, int x, float dt);

    void gravity(int y, int x, float dt);

public:
    //Variables
    enum ElementType
    {
        EMPTY = 0,
        SAND,
        WATER,
    };

    struct GridCell
    {
        ElementType elementType;
        sf::Color color;
        int density;
        sf::Vector2f velocity;
        bool has_been_updated;
    };

    std::vector<std::vector<GridCell>> grid;

    //Functions
    void gridUpdate(float dt);
    void brushTool(int y, int x, int brush_size, int tool);
    void clearBoard();

    Board(int width = 10, int height = 10);
};
