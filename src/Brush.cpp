#include "Brush.hpp"

Brush::Brush(int scale)
{
    size = 5;
    element = ElementType::SAND;

    init_outline(scale);
}

void Brush::init_outline(int scale)
{
   outline.setSize({10.f,10.f});
   outline.setScale(sf::Vector2f(scale, scale));
   outline.setFillColor(sf::Color::Transparent);
   outline.setOutlineColor({212, 217, 219, 150});
   outline.setOutlineThickness(0.5f);
}

int Brush::get_size()
{
    return size;
}

ElementType Brush::get_element()
{
    return element;
}

void Brush::change_size(int diff)
{
    size += diff;
}

void Brush::change_element(int diff)
{
    int value = static_cast<int>(element);
    //Sand is always First Element
    int first_element = static_cast<int>(ElementType::SAND);

    //Fire is always Last Element
    int last_element = static_cast<int>(ElementType::FIRE);

    value += diff;

    if(value < first_element) value = last_element;

    if(value > last_element) value = first_element;

    element = static_cast<ElementType>(value);
}

void Brush::update_outline(sf::Vector2i mouse_pos, int scale)
{
    outline.setSize(sf::Vector2f(size, size));
    outline.setPosition({static_cast<float>(mouse_pos.x) - (size * scale) / 2, static_cast<float>(mouse_pos.y) - (size * scale) / 2,});
}

void Brush::draw_outline(sf::RenderWindow& window)
{
    window.draw(outline);
}