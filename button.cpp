#include "button.h"
#include"errors.h"
Button::Button(const char* name, int x, int y, const sf::Color& color) : x_pos(x), y_pos(y)
{
    if(!texture.loadFromFile(name))
        exit(WRONG_PICTURE);
    spr.setTexture(texture);
    spr.setPosition(x_pos, y_pos);
    spr.setColor(color);
}

void Button::SetColor(const sf::Color& c)
{
    spr.setColor(c);
}

void Button::Draw(sf::RenderWindow& window)
{
    window.draw(spr);
}

