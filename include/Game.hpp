#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Game
{
private:

    //Variables
        //Window
        sf::RenderWindow* window;

    //Functions
    void initVariables();
    void initWindow();

    void event();
    
public:
    //Functions
    void update();
    void render();

    const bool isRunning() const;

    //Constructor // Deconstructor
    Game();
    virtual ~Game();

};

