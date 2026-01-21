#include <SFML/Graphics.hpp>

#include <vector>

class Board
{
private:

    //Variables
    //std::vector<std::vector<int>> boardVector;

    //Functions
    void powderUpdate(int y, int x);

public:
    //Variables
    enum ElementType
    {
        EMPTY = 0,
        SAND = 1
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


    //Functions
    void initBoard(int w, int h);
    void gridUpdate();


    Board();
    ~Board();
};
