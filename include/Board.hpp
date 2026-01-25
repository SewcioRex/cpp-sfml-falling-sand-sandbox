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
    std::uniform_int_distribution<> dist_1_40;
    int random_side;

    int side;

    int max_grid_H;
    int max_grid_W;

    //Functions
    void powderUpdate(int y, int x);

public:
    //Variables
    enum ElementType
    {
        EMPTY = 0,
        SAND,
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
        int density;
        bool updated;
    };


    std::vector<std::vector<GridCell>> grid;


    //Functions
    void initBoard(int w, int h);
    void gridUpdate();
    void brushTool(int y, int x, int brush_size, int tool);
    void clearBoard();
};
