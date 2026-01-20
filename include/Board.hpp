#include <vector>

class Board
{
private:

    //Variables
    //std::vector<std::vector<int>> boardVector;

    //Functions

public:
    //Variables
    std::vector<std::vector<int>> grid;


    //Functions
    void initBoard(int w, int h);


    Board();
    ~Board();
};
