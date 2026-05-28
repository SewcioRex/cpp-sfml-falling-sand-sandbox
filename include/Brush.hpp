#include <SFML/Graphics.hpp>

#include <vector>

#include "ElementType.hpp"

class Brush
{
private:
    int size;

    sf::RectangleShape outline;

    ElementType element;

    void init_outline(int scale);

public:
    Brush(int scale);

    void change_size(int diff);
    void change_element(int diff);

    int get_size();
    ElementType get_element();

    void update_outline(sf::Vector2i mouse_pos, int scale);

    void draw_outline(sf::RenderWindow& window);
};

