#include <SFML/Graphics.hpp>

#include <vector>

class Board
{
private:

    //Variables
    //std::vector<std::vector<int>> boardVector;

    //Functions

public:
    //Variables
    enum ElementType
    {
        EMPTY = 0,
        SAND = 1
    };

    struct GridCell
    {
        ElementType elementType;
        sf::Color color;
    };


    std::vector<std::vector<GridCell>> grid;


    //Functions
    void initBoard(int w, int h);


    Board();
    ~Board();
};
