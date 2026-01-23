#include <SFML/Graphics.hpp>

#include <vector>
#include <random>

class Board
{
private:

    //Variables
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist_1_2;
    std::uniform_int_distribution<> dist_1_4;
    int random_side;

    //Functions
    void powderUpdate(int y, int x);

public:
    //Variables
    enum ElementType
    {
        EMPTY = 0,
        SAND
    };

    enum ElementState
    {
        NONE = 0,
        POWDER
    };

    struct GridCell
    {
        ElementType elementType;
        ElementState state;
        sf::Color color;
    };


    std::vector<std::vector<GridCell>> grid;
    std::vector<std::vector<GridCell>> read_grid;
    std::vector<std::vector<GridCell>> template_grid;


    //Functions
    void initBoard(int w, int h);
    void gridUpdate();
    void brushTool(int y, int x, int brush_size, int tool);
};
